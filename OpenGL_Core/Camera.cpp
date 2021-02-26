#include "pch.h"

#include "Camera.h"

#include "GameSystem.h"
#include "SceneManager.h"
#include "UniformManager.h"
#include "Graphics.h"

#include "Skybox.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderTexture.h"

#include "Renderer.h"

namespace OpenGL_Core
{
    Mesh* Camera::DefaultTargetTextureMesh = nullptr;
    Shader* Camera::DefaultTargetTextureShader = nullptr;
    Material* Camera::DefaultTargetTextureMaterial = nullptr;
    RenderTexture* Camera::DefaultTargetTexture = nullptr;
    RenderTexture* Camera::DefaultTargetMultisampleTexture = nullptr;

    unique_ptr<RenderState> Camera::realRenderState = nullptr;


    Camera* Camera::GetMain()
    {
        return SceneManager::GetActiveScene().FindComponent<Camera>();
    }

    void Camera::UseRenderState(const RenderState& state)
    {
        if (realRenderState->BackgroundColor != state.BackgroundColor)
        {
            realRenderState->BackgroundColor = state.BackgroundColor;
            glClearColor(realRenderState->BackgroundColor.r, realRenderState->BackgroundColor.g, realRenderState->BackgroundColor.b, realRenderState->BackgroundColor.a);
        }
        if (realRenderState->ClearDepth != state.ClearDepth)
        {
            realRenderState->ClearDepth = state.ClearDepth;
            glClearDepth(realRenderState->ClearDepth);
        }
        if (realRenderState->ClearStencil != state.ClearStencil)
        {
            realRenderState->ClearStencil = state.ClearStencil;
            glClearStencil(realRenderState->ClearStencil);
        }
    }
    void Camera::RenderToWindow()
    {
        UniformManager::Transform->SetSubData(0, sizeof(Matrix4x4), Matrix4x4::Identity.GetPtr());
        UniformManager::Transform->SetSubData(sizeof(Matrix4x4), sizeof(Matrix4x4), Matrix4x4::Identity.GetPtr());
        UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), Matrix4x4::Identity.GetPtr());

        RenderTexture::UnBindFramebuffer();
        Graphics::DrawMesh(*DefaultTargetTextureMesh, *DefaultTargetTextureMaterial, 0);
    }

    void Camera::DebugRenderTexture(RenderTexture& tex, const float& startX, const float& startY, const float& widthScale, const float& heightScale, Shader* shader)
    {
        unique_ptr<Shader> _shader;
        if (!shader)
        {
            _shader = make_unique<Shader>(*DefaultTargetTextureShader);
            shader = _shader.get();
        }
        shader->BindTexture(*tex.GetTexture(0), "_MainTexture", 0);
        auto material = Material("Temp Debug Material", { shader });


        auto transform = Matrix4x4::Translate(Vector3(startX, startY)) * Matrix4x4::Scale(Vector3(widthScale, heightScale, 1.0f));
        UniformManager::Transform->SetSubData(0, sizeof(Matrix4x4), Matrix4x4::Identity.GetPtr());
        UniformManager::Transform->SetSubData(sizeof(Matrix4x4), sizeof(Matrix4x4), Matrix4x4::Identity.GetPtr());
        UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), transform.GetPtr());

        DefaultTargetTexture->BindFramebuffer();
        Graphics::DrawMesh(*Camera::DefaultTargetTextureMesh, material, 0);
    }

    void Camera::BindTarget() const
    {
        // 渲染到指定目标
        if (this->TargetTexture)
        {
            this->TargetTexture->BindFramebuffer();
            return;
        }

        // 渲染到默认贴图上 （直接渲染到窗口会出错）
        if (this->IsMSAA)
            DefaultTargetMultisampleTexture->BindFramebuffer();
        else DefaultTargetTexture->BindFramebuffer();
    }
    void Camera::RenderToTargetTexture(RenderTexture* targetTex)
    {
        if (targetTex) // 渲染到指定贴图存在
        {
            if (this->TargetTexture) // 相机指定贴图数据拷贝
                Graphics::Blit(*TargetTexture, *targetTex);
            else if (this->IsMSAA) // 默认多采样渲染贴图数据拷贝
                Graphics::Blit(*Camera::DefaultTargetMultisampleTexture, *targetTex);
            else // 默认渲染贴图数据拷贝
                Graphics::Blit(*Camera::DefaultTargetTexture, *targetTex);
        }
        else
        {
            if (this->TargetTexture) return; // 相机指定贴图不修改
            else if (this->IsMSAA) // 默认多采样渲染贴图数据拷贝到默认渲染贴图
                Graphics::Blit(*Camera::DefaultTargetMultisampleTexture, *Camera::DefaultTargetTexture);
            else return; // 默认渲染贴图不修改
        }
    }

    void Camera::ExcuteRender(List<RenderItem*>* backgrounds, List<RenderItem*>* geometrys, List<RenderItem*>* alphaTests, List<RenderItem*>* transparents, List<RenderItem*>* overlays)
    {
        glViewport(0, 0, GameSystem::ScreenWidth, GameSystem::ScreenHeight);
        // Rendering
        this->Clear();

        // 0~1000 Background
        if (backgrounds)
            for (auto item : *backgrounds)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr());
                item->renderer->Draw(item->index);
            }
        // 1001~2332 Geometry
        if (geometrys)
            for (auto item : *geometrys)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr());
                item->renderer->Draw(item->index);
            }
        // 2333 Skybox
        this->DrawSkybox();

        // 2334~2999 AlphaTest
        if (alphaTests)
            for (auto item : *alphaTests)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr());
                item->renderer->Draw(item->index);
            }
        // 3000~3999 Transparent 
        // 按离相机距离由大到小排序
        if (transparents)
        {
            auto cameraPos = this->GetTransform().GetPosition();
            for (auto item : *transparents)
            {
                auto pos = item->renderer->GetTransform().GetPosition() - cameraPos;
                item->depth = Vector3::Dot(pos, pos);// 仅做比较时无需开方
            }
            sort(transparents->begin(), transparents->end(), [](const RenderItem* left, const RenderItem* right) { return left->depth > right->depth; });
            for (auto item : *transparents)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr());
                item->renderer->Draw(item->index);
            }
        }
        // 4000+ Overlay
        if (overlays)
            for (auto item : *overlays)
            {
                auto m = item->renderer->GetTransform().GetTransformMatrix();
                UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), m.GetPtr());
                item->renderer->Draw(item->index);
            }
    }

    void Camera::Clear()
    {
        if (this->ClearFlags == CameraClearFlags::SKYBOX || this->ClearFlags == CameraClearFlags::SOLID_COLOR)
        {
            Camera::UseRenderState(this->State);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //状态使用函数
        }
        else if (this->ClearFlags == CameraClearFlags::DEPTH)
        {
            Camera::UseRenderState(this->State);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
    }

    void Camera::ClearDepth()
    {
        Camera::UseRenderState(this->State);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void Camera::DrawSkybox()
    {
        if (ClearFlags == CameraClearFlags::SKYBOX && _skybox)
            _skybox->DrawSkyBox();
    }

    Matrix4x4 Camera::GetViewMatrix() const
    {
        // 将世界中的相机的基变换到OpenGl视口基
        /*相机空间与OpenGL约定相匹配：相机的前向是负Z轴。
        在世界坐标系中，使用左手坐标系，
        在视口中，使用右手坐标系，屏幕对着你的方向是+Z
        */
        auto T = Matrix4x4::Translate(-this->GetTransform().GetPosition());
        auto R = this->GetTransform().GetRotationMatrix().Transpose();
        return (R * T);
        //return Matrix4x4::ZInverse * (R * T);
        //return Matrix4x4::ZInverse * (this->GetTransform().GetPositionMatrix() * this->GetTransform().GetRotationMatrix()).Inverse();
    }

    Matrix4x4 Camera::GetProjectionMatrix() const
    {
        Matrix4x4 projectionMatrix = Matrix4x4::Identity;
        auto aspect = (GLfloat)GameSystem::ScreenWidth / GameSystem::ScreenHeight;
        if (this->ProjectionMode == ProjectionMode::PERSPECTIVE)
        {

            projectionMatrix = Matrix4x4::Perspective(this->FieldOfView, aspect, this->_nearClipPlane, this->_farClipPlane);
        }
        else
        {
            auto width = aspect * this->OrthographicSize;
            auto height = this->OrthographicSize;
            projectionMatrix = Matrix4x4::Ortho(-width, width, -height, height, this->_nearClipPlane, this->_farClipPlane);
        }
        return projectionMatrix;
    }

    Vector4 Camera::GetOrthoParams()
    {
        auto aspect = (GLfloat)GameSystem::ScreenWidth / GameSystem::ScreenHeight;
        auto width = aspect * this->OrthographicSize;
        auto height = this->OrthographicSize;
        return Vector4(width, height, aspect, (float)ProjectionMode);
    }

    Camera::Camera(GameObject& obj) :Behaviour(obj), State(),
        ClearFlags(CameraClearFlags::SOLID_COLOR), ProjectionMode(ProjectionMode::PERSPECTIVE),
        TargetTexture(nullptr), Depth(0), IsMSAA(false),
        FieldOfView(60.0f), OrthographicSize(5.0f),
        _nearClipPlane(0.1f), _farClipPlane(10000.0f),
        _skybox(nullptr)
    {
        if (!DefaultTargetTextureMesh)
        {
            DefaultTargetTextureMesh = SceneManager::GetActiveScene().AddResourceObject(make_unique<Mesh>("Default TargetTexture Mesh"));
            DefaultTargetTextureMesh->HideFlag = HideFlagType::STATIC;

            // 初始化特效要使用的平面
            DefaultTargetTextureMesh->vertices.push_back(Vector3(-1.0f, -1.0f, 0.0f));
            DefaultTargetTextureMesh->vertices.push_back(Vector3(-1.0f, 1.0f, 0.0f));
            DefaultTargetTextureMesh->vertices.push_back(Vector3(1.0f, -1.0f, 0.0f));
            DefaultTargetTextureMesh->vertices.push_back(Vector3(-1.0f, 1.0f, 0.0f));
            DefaultTargetTextureMesh->vertices.push_back(Vector3(1.0f, 1.0f, 0.0f));
            DefaultTargetTextureMesh->vertices.push_back(Vector3(1.0f, -1.0f, 0.0f));

            DefaultTargetTextureMesh->uv.push_back(Vector2(0.0f, 0.0f));
            DefaultTargetTextureMesh->uv.push_back(Vector2(0.0f, 1.0f));
            DefaultTargetTextureMesh->uv.push_back(Vector2(1.0f, 0.0f));
            DefaultTargetTextureMesh->uv.push_back(Vector2(0.0f, 1.0f));
            DefaultTargetTextureMesh->uv.push_back(Vector2(1.0f, 1.0f));
            DefaultTargetTextureMesh->uv.push_back(Vector2(1.0f, 0.0f));

            DefaultTargetTextureMesh->indices.push_back(0);
            DefaultTargetTextureMesh->indices.push_back(1);
            DefaultTargetTextureMesh->indices.push_back(2);
            DefaultTargetTextureMesh->indices.push_back(3);
            DefaultTargetTextureMesh->indices.push_back(4);
            DefaultTargetTextureMesh->indices.push_back(5);

            DefaultTargetTextureMesh->Complete();
        }

        if (!DefaultTargetMultisampleTexture)
        {
            DefaultTargetMultisampleTexture = SceneManager::GetActiveScene().AddResourceObject(
                RenderTexture::CreateRenderTexture("Default Multisample TargetTexture", GameSystem::ScreenWidth, GameSystem::ScreenHeight, 4));
            DefaultTargetMultisampleTexture->HideFlag = HideFlagType::STATIC;
            DefaultTargetMultisampleTexture->AttachmentMultisampleTexture(GL_RGBA16F);
            DefaultTargetMultisampleTexture->AttachmentRenderBuffer();
            DefaultTargetMultisampleTexture->CheckFramebufferSuccess();
        }
        if (!DefaultTargetTexture)
        {
            DefaultTargetTexture = SceneManager::GetActiveScene().AddResourceObject(
                RenderTexture::CreateRenderTexture("Default TargetTexture", GameSystem::ScreenWidth, GameSystem::ScreenHeight));
            //RenderTexture::CreateRenderTexture("Default TargetTexture", GameSystem::ScreenWidth, GameSystem::ScreenHeight, GL_RGBA16F, FormatType::RGBA));
            DefaultTargetTexture->HideFlag = HideFlagType::STATIC;
            DefaultTargetTexture->AttachmentTexture2D(GL_RGBA16F, FormatType::RGBA);
            DefaultTargetTexture->AttachmentRenderBuffer();
            DefaultTargetTexture->CheckFramebufferSuccess();
        }

        if (!DefaultTargetTextureShader)
        {
            DefaultTargetTextureShader = SceneManager::GetActiveScene().AddResourceObject(make_unique<Shader>("Default TargetTexture Shader", "../Asset/Shader/Unlit/Texture.glsl"));
            DefaultTargetTextureShader->State.DepthTest = false;
            DefaultTargetTextureShader->State.Blend = false;
            DefaultTargetTextureShader->State.CullFace = false;
            DefaultTargetTextureShader->HideFlag = HideFlagType::STATIC;
            //auto shader = make_unique<Shader>("Default TargetTexture Shader", "../Asset/Shader/Shadow/ShowDepth.glsl");

            DefaultTargetTextureMaterial = SceneManager::GetActiveScene().AddResourceObject(unique_ptr<Material>(new Material("Default TargetTexture Material", { DefaultTargetTextureShader })));
            DefaultTargetTextureMaterial->GetMainShader()->BindTexture(*DefaultTargetTexture->GetTexture(0), "_MainTexture", 0);
            DefaultTargetTextureMaterial->HideFlag = HideFlagType::STATIC;
        }

        if (!realRenderState)
        {
            realRenderState = make_unique<RenderState>();
            glClearColor(realRenderState->BackgroundColor.r, realRenderState->BackgroundColor.g, realRenderState->BackgroundColor.b, realRenderState->BackgroundColor.a);
            glClearDepth(realRenderState->ClearDepth);
            glClearStencil(realRenderState->ClearStencil);
        }
    }
}
