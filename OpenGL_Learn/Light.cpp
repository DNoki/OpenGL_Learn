#include "Light.h"

#include "UniformManager.h"

#include "GameSystem.h"

#include "Renderer.h"


namespace OpenGL_Learn
{
    Material* DirectionalLight::DirectionalLightShadowmapMaterial;
    Material* PointLight::PointLightShadowmapMaterial;

    Matrix4x4 DirectionalLight::GetLightSpaceMatrix(Camera* camera) const
    {
        // 视图矩阵
        auto lightRotate = this->GetTransform().GetRotationMatrix();
        auto lightPosition = Matrix4x4::Translate(camera->GetTransform().GetPosition()
            + camera->GetTransform().GetForward() * ShadowDistance * 0.5f
            + -this->GetTransform().GetForward() * ShadowDistance * 0.5f
        );
        Matrix4x4 lightView = (lightPosition * lightRotate).Inverse();
        //Matrix4x4 lightView = Matrix4x4::ZInverse * (lightPosition * lightRotate).Inverse();

        // 投影矩阵
        auto size = ShadowDistance * 0.5f;
        Matrix4x4 lightProjection = Matrix4x4::Ortho(-size, size, -size, size, ShadowNearPlaneOffset, 2 * ShadowDistance);

        return lightProjection * lightView;
    }
    void DirectionalLight::GenerateShadowMap(UINT width, UINT height)
    {
        //ShadowMap = SceneManager::GetActiveScene().AddResourceObject(
        //    RenderTexture::CreateRenderTexture("Shadowmap", width, height,
        //        GL_DEPTH_COMPONENT, FormatType::DEPTH_COMPONENT, TextureType::FLOAT,
        //        WrapType::CLAMP_TO_BORDER, ScaleFilterType::NEAREST, AttachmentType::DEPTH_ATTACHMENT, false));
        ShadowMap = SceneManager::GetActiveScene().AddResourceObject(
            RenderTexture::CreateRenderTexture("Shadowmap", width, height));
        ShadowMap->AttachmentTexture2D(GL_DEPTH_COMPONENT, FormatType::DEPTH_COMPONENT, TextureType::FLOAT,
            WrapType::CLAMP_TO_BORDER, ScaleFilterType::NEAREST, AttachmentType::DEPTH_ATTACHMENT);
        ShadowMap->CheckFramebufferSuccess();

        ShadowMap->GetTexture(0)->BindTexture();
        GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
    void DirectionalLight::RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays)
    {
        this->ShadowMap->BindFramebuffer();
        Camera::UseRenderState(this->State);
        glViewport(0, 0, this->ShadowMap->GetTexture(0)->GetWidth(), this->ShadowMap->GetTexture(0)->GetHeight());
        glClear(GL_DEPTH_BUFFER_BIT);
        for (size_t i = 0; i < DirectionalLightShadowmapMaterial->GetPassCount(); i++)
        {
            (*DirectionalLightShadowmapMaterial)[i]->SetFloat("_Bias", this->Bias * ShadowDistance);
            (*DirectionalLightShadowmapMaterial)[i]->SetFloat("_NormalBias", this->NormalBias);
        }

        if (geometrys)
            for (auto& item : *geometrys)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr()); // 模型变换数据
                item->renderer->Draw(DirectionalLightShadowmapMaterial, item->index);
                // 绑定阴影贴图
                (*item->material)[item->index]->BindTexture(*this->ShadowMap->GetTexture(0), "_ShadowMap", 10);
            }
    }
    DirectionalLight::DirectionalLight(GameObject& obj) :Light(obj), ShadowMap()
    {
        if (!DirectionalLightShadowmapMaterial)
        {
            auto shader = SceneManager::GetActiveScene().AddResourceObject(
                make_unique<Shader>("DirectionalLight ShadowMap Shader", "../Asset/Shader/Shadow/DirectionalLightShadowMap.glsl"));
            shader->HideFlag = HideFlagType::STATIC;
            shader->State.DepthTestMode = TestModeType::LESS;
            shader->State.CullFace = true;
            shader->State.CullFaceMode = CullFaceModeType::FRONT;

            DirectionalLightShadowmapMaterial = SceneManager::GetActiveScene().AddResourceObject(
                unique_ptr<Material>(new Material("DirectionalLight ShadowMap Material", { shader })));
            DirectionalLightShadowmapMaterial->HideFlag = HideFlagType::STATIC;
        }
        Bias = 0.00002f;
        NormalBias = 0.02f;
    }


    unique_ptr<List<Matrix4x4>> PointLight::GetLightSpaceMatrix() const
    {
        // 视图矩阵
        auto shadowTransforms = make_unique<List<Matrix4x4>>();
        auto lightPos = this->GetTransform().GetPosition();
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Right, Vector3::Down)); // 右
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Left, Vector3::Down)); // 左
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Up, Vector3::Forward)); // 上
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Down, Vector3::Back)); // 下
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Forward, Vector3::Down)); // 前
        shadowTransforms->push_back(Matrix4x4::LookAt(lightPos, lightPos + Vector3::Back, Vector3::Down)); // 后

        // 投影矩阵
        auto aspect = (float)this->CubeShadowMap->GetTexture(0)->GetWidth() / this->CubeShadowMap->GetTexture(0)->GetHeight();
        auto shadowProj = Matrix4x4::Perspective(90.0f, aspect, ShadowNearPlaneOffset, ShadowDistance);

        for (auto& transform : *shadowTransforms)
        {
            //transform = shadowProj * transform;
            transform = shadowProj 
#ifdef GLM_FORCE_LEFT_HANDED
                * Matrix4x4::XInverse
#endif // GLM_FORCE_LEFT_HANDED
                * transform;
        }

        return shadowTransforms;
    }
    void PointLight::RenderShadowMap(UINT index, List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays)
    {
        this->CubeShadowMap->BindFramebuffer();
        Camera::UseRenderState(this->State);
        glViewport(0, 0, this->CubeShadowMap->GetTexture(0)->GetWidth(), this->CubeShadowMap->GetTexture(0)->GetHeight());
        glClear(GL_DEPTH_BUFFER_BIT);

        auto pointLightSpaceMatrix = GetLightSpaceMatrix();
        for (size_t i = 0; i < PointLightShadowmapMaterial->GetPassCount(); i++)
        {
            for (GLuint j = 0; j < 6; j++)
                (*PointLightShadowmapMaterial)[i]->SetMatrix4x4(
                    "_ShadowMatrices[" + to_string(j) + "]", (*pointLightSpaceMatrix)[j]);
            (*PointLightShadowmapMaterial)[i]->SetVector4("_PointLightPosition", Vector4(this->GetTransform().GetPosition(), 1.0f));
            (*PointLightShadowmapMaterial)[i]->SetFloat("_FarPlane", ShadowDistance);
            (*PointLightShadowmapMaterial)[i]->SetFloat("_Bias", Bias);
            (*PointLightShadowmapMaterial)[i]->SetFloat("_NormalBias", NormalBias);
        }

        if (geometrys)
            for (auto& item : *geometrys)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr()); // 模型变换数据
                item->renderer->Draw(PointLightShadowmapMaterial, item->index);
                // 绑定阴影贴图
                (*item->material)[item->index]->BindTexture(*this->CubeShadowMap->GetTexture(0), "_PointLight4ShadowMap[" + to_string(index) + "]", 11 + index);
            }
    }
    PointLight::PointLight(GameObject& obj) :Light(obj),
        //Attenuation(0, 0), CubeShadowMap(),
        Attenuation(0.22f, 0.2f), CubeShadowMap()
    {
        if (!PointLightShadowmapMaterial)
        {
            auto shader = SceneManager::GetActiveScene().AddResourceObject(
                make_unique<Shader>("PointLight ShadowMap Shader", "../Asset/Shader/Shadow/PointLightShadowMap.glsl"));
            shader->HideFlag = HideFlagType::STATIC;
            shader->State.DepthTestMode = TestModeType::LESS;
            shader->State.CullFace = true;
            shader->State.CullFaceMode = CullFaceModeType::FRONT;

            PointLightShadowmapMaterial = SceneManager::GetActiveScene().AddResourceObject(
                unique_ptr<Material>(new Material("PointLight ShadowMap Material", { shader })));
            PointLightShadowmapMaterial->HideFlag = HideFlagType::STATIC;
        }
        CubeShadowMap = SceneManager::GetActiveScene().AddResourceObject(RenderTexture::CreateRenderTexture("Cube Shadowmap", 1024, 1024));
        //CubeShadowMap = SceneManager::GetActiveScene().AddResourceObject(CubeRenderTexture::CreateRenderTexture("Cube Shadowmap", 1024, 1024,
        //    GL_DEPTH_COMPONENT, FormatType::DEPTH_COMPONENT, TextureType::FLOAT,
        //    WrapType::CLAMP_TO_EDGE, ScaleFilterType::NEAREST, AttachmentType::DEPTH_ATTACHMENT));
        CubeShadowMap->AttachmentTextureCube(GL_DEPTH_COMPONENT, FormatType::DEPTH_COMPONENT, TextureType::FLOAT,
            WrapType::CLAMP_TO_EDGE, ScaleFilterType::NEAREST, AttachmentType::DEPTH_ATTACHMENT);
        CubeShadowMap->CheckFramebufferSuccess();

        Bias = 0.01f;
        NormalBias = 0.01f;
    }
}
