#pragma once

#include <memory>

#include "ScriptBehaviour.h"

namespace OpenGL_Learn
{

    // 后处理
    class PostProcess final : public ScriptBehaviour
    {
    public:
        RenderTexture* EffectRenderTexture = nullptr;

        // 添加特效
        // @material 材质
        // @rt 渲染贴图
        void AddEffect(Material& material)
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
        Tuple2<bool, Material*>* GetEffectMaterial(unsigned int index)
        {
            if (index < GetEffectCount())
                return &_materials[index];
            return nullptr;
        }

        // 获取特效数量
        inline unsigned int GetEffectCount() { return (unsigned int)_materials.size(); }

        void Awake() override
        {
            _camera = this->GetGameObject().GetComponent<Camera>();
        }
        void OnPreRender() override
        {
        }
        void OnPostRender() override
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

        void OnRenderImage(RenderTexture* targetTex) override
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

        PostProcess(GameObject& obj) :ScriptBehaviour(obj), _camera(), _materials() {}

        Material* operator[](unsigned int i) { return _materials[i].Item2; }

    private:
        Camera* _camera;

        List<Tuple2<bool, Material*>> _materials;
    };
}