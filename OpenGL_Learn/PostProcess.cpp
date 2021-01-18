#include "../OpenGL_Core/ScriptBehaviour.h"
#include "../OpenGL_Core/GameSystem.h"
#include "../OpenGL_Core/SceneManager.h"
#include "../OpenGL_Core/RenderTexture.h"
#include "../OpenGL_Core/Mesh.h"
#include "../OpenGL_Core/Material.h"
#include "../OpenGL_Core/Camera.h"

#include "PostProcess.h"


namespace OpenGL_Learn
{
    void PostProcess::AddEffect(Material& material)
    {
        if (!EffectRenderTexture)
        {
            EffectRenderTexture = SceneManager::GetActiveScene().AddResourceObject(RenderTexture::CreateRenderTexture("Effect Render Texture",
                GameSystem::ScreenWidth, GameSystem::ScreenHeight));
            EffectRenderTexture->AttachmentTexture2D(GL_RGBA16F, FormatType::RGBA);
            //EffectRenderTexture->HideFlag = HideFlagType::STATIC;
        }
        _materials.push_back(Tuple2<bool, Material*>(true, &material));
        for (unsigned int i = 0; i < material.GetPassCount(); i++)
        {
            material[i]->BindTexture(*EffectRenderTexture->GetTexture(0), "_MainTexture", 0);
            cout << "LOG:: " << "已添加后处理效果："
                << " Material: " << material[i]->Name << endl;
        }
    }
    Tuple2<bool, Material*>* PostProcess::GetEffectMaterial(unsigned int index)
    {
        if (index < GetEffectCount())
            return &_materials[index];
        return nullptr;
    }

    void PostProcess::Awake()
    {
        _camera = this->GetGameObject().GetComponent<Camera>();
    }

    void PostProcess::OnPreRender() {}

    void PostProcess::OnPostRender()
    {
        // 仅当前相机渲染完成后处理的后期特效
        // 三种情况：
        // 1. 渲染数据在指定目标渲染贴图上
        // 2. 渲染数据在默认多采样渲染贴图上
        // 3. 渲染数据在默认渲染贴图上
        //for (unsigned int i = 0; i < _materials.size(); i++)
        //{
        //    // 加载已选染场景到纹理 或 从多采样纹理中加载纹理
        //    _camera->RenderToTargetTexture(EffectRenderTexture.get());
        //    // 绑定渲染相机
        //    _camera->BindTarget();
        //    // 应用特效材质并渲染四边形到窗口
        //    for (unsigned int j = 0; j < _materials[i]->GetPassCount(); j++)
        //        Camera::DefaultTargetTextureMesh->DrawMesh(*_materials[i], j);
        //}
    }

    void PostProcess::OnRenderImage(RenderTexture* targetTex)
    {
        // 每个脚本都会处理的后期特效
        for (unsigned int i = 0; i < _materials.size(); i++)
        {
            if (!_materials[i].Item1) continue;
            targetTex->Blit(*EffectRenderTexture);// 将默认渲染贴图数据拷贝到效果渲染贴图上
            targetTex->BindFramebuffer();// 渲染到默认渲染贴图上
            for (unsigned int j = 0; j < _materials[i].Item2->GetPassCount(); j++)
                Camera::DefaultTargetTextureMesh->DrawMesh(*_materials[i].Item2, j);
        }
    }

    void PostProcess::OnConsoleBar(int index, stringstream& fmt)
    {
        auto* postProcess = this;
        fmt.str(""); fmt << index;
        auto enable = postProcess->GetEnable();
        ImGui::Checkbox(fmt.str().c_str(), &enable); ImGui::SameLine();
        postProcess->SetEnable(enable);
        fmt.str(""); fmt << "PostProcess" << "  0x" << postProcess;
        if (ImGui::TreeNode(fmt.str().c_str()))
        {
            for (size_t i = 0; i < postProcess->GetEffectCount(); i++)
            {
                ImGui::NextColumn(); ImGui::NextColumn();
                auto effect = postProcess->GetEffectMaterial(i);
                ImGui::Checkbox(to_string(i).c_str(), &effect->Item1); ImGui::SameLine();
                ConsoleBar::CheckMaterial(i, fmt, effect->Item2);
                //ImGui::Text(effect->Item2->Name.c_str());
            }
            ImGui::TreePop();
        }
    }
}
