#include "pch.h"

#include "GaussianBlur.h"

namespace OpenGL_Learn
{
    void GaussianBlur::Awake()
    {
        if (!EffectRenderTexture)
        {
            //EffectRenderTexture = SceneManager::GetActiveScene()->AddResourceObject(RenderTexture::CreateRenderTexture("GaussianBlur Render Texture",
            //    GameSystem::ScreenWidth >> DownSample, GameSystem::ScreenHeight >> DownSample, GL_RGBA16F, FormatType::RGBA));
            EffectRenderTexture = SceneManager::GetActiveScene()->AddResourceObject(RenderTexture::CreateRenderTexture("GaussianBlur Render Texture",
                GameSystem::ScreenWidth >> DownSample, GameSystem::ScreenHeight >> DownSample));
            EffectRenderTexture->AttachmentTexture2D(GL_RGBA16F, FormatType::RGBA);
            EffectRenderTexture->CheckFramebufferSuccess();
        }
        if (!gaussianBlurMaterial)
        {
            auto blurShader = SceneManager::GetActiveScene()->AddResourceObject(make_unique<Shader>("GaussianBlur Shader", "../Asset/Shader/Post/GaussianBlur.glsl"));
            blurShader->State.DepthTest = false;
            blurShader->BindTexture(*EffectRenderTexture->GetTexture(0), "_MainTexture", 0);

            gaussianBlurMaterial = SceneManager::GetActiveScene()->AddResourceObject(make_unique<Material>(
                "Gaussian Blur Material", blurShader));
            //blurShader = SceneManager::GetActiveScene()->AddResourceObject(make_unique<Shader>(*blurShader));
            //blurShader->SetVector4("_Offset", Vector4(Vector3::Up, 0.0f));
            //gaussianBlurMaterial->AddShaderPass(blurShader);
        }
    }
    void GaussianBlur::OnRenderImage(RenderTexture* targetTex)
    {
        for (unsigned int i = 0; i < Iteration; i++)
        {
            Graphics::Blit(*targetTex, *EffectRenderTexture, GL_COLOR_BUFFER_BIT, ScaleFilterType::LINEAR);
            targetTex->BindFramebuffer();
            gaussianBlurMaterial->GetMainShader()->SetVector4("_Offset", Vector4(Vector3::Right, 0.0f));
            Graphics::DrawMesh(*Camera::DefaultTargetTextureMesh, *gaussianBlurMaterial, 0);
            Graphics::Blit(*targetTex, *EffectRenderTexture, GL_COLOR_BUFFER_BIT, ScaleFilterType::LINEAR);
            targetTex->BindFramebuffer();
            gaussianBlurMaterial->GetMainShader()->SetVector4("_Offset", Vector4(Vector3::Up, 0.0f));
            Graphics::DrawMesh(*Camera::DefaultTargetTextureMesh, *gaussianBlurMaterial, 0);
        }
    }
    void GaussianBlur::OnConsoleBar(int index, stringstream& fmt)
    {
        fmt.str(""); fmt << index;
        auto enable = GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        SetEnable(enable);
        fmt.str(""); fmt << "GaussianBlur";
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            //ImGui::NextColumn(); ImGui::NextColumn();
            ImGui::NextColumn();

            int iteration = Iteration;
            ImGui::InputInt("Iteration", &iteration);
            Iteration = iteration < 0 ? 0 : iteration;

            int downSample = DownSample;
            ImGui::InputInt("DownSample", &downSample);
            if (downSample < 0) downSample = 0;
            if (downSample != DownSample)
            {
                DownSample = downSample;
                ResourceObject::Destroy(*EffectRenderTexture);
                cout << "EffectRenderTexture释放" << endl;
                EffectRenderTexture = SceneManager::GetActiveScene()->AddResourceObject(RenderTexture::CreateRenderTexture("GaussianBlur Render Texture",
                    GameSystem::ScreenWidth >> DownSample, GameSystem::ScreenHeight >> DownSample));
                EffectRenderTexture->AttachmentTexture2D(GL_RGBA16F, FormatType::RGBA);
                EffectRenderTexture->CheckFramebufferSuccess();
                for (auto shader : gaussianBlurMaterial->Shaders)
                    shader->BindTexture(*EffectRenderTexture->GetTexture(0), "_MainTexture", 0);
            }
            ImGui::NextColumn();
            ConsoleBar::CheckMaterial(0, fmt, gaussianBlurMaterial);
            ImGui::TreePop();
        }
    }
    GaussianBlur::GaussianBlur(GameObject& obj) : ScriptBehaviour(obj), EffectRenderTexture(), gaussianBlurMaterial(),
        DownSample(1), Iteration(1) {}
}