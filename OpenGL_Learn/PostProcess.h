#pragma once

#include "IConsoleBarScriptBehaviour.hpp"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    /// <summary>
    /// 后处理
    /// </summary>
    class PostProcess final : public ScriptBehaviour, public IConsoleBarScriptBehaviour
    {
    public:
        RenderTexture* EffectRenderTexture = nullptr;

        /// <summary>
        /// 添加特效
        /// </summary>
        /// <param name="material">材质</param>
        void AddEffect(Material& material);
        Tuple2<bool, Material*>* GetEffectMaterial(unsigned int index);

        /// <summary>
        /// 获取特效数量
        /// </summary>
        /// <returns></returns>
        inline unsigned int GetEffectCount() { return (unsigned int)_materials.size(); }

        void Awake() override;
        void OnPreRender() override;
        void OnPostRender() override;

        void OnRenderImage(RenderTexture* targetTex) override;

        void OnConsoleBar(int index, stringstream& fmt) override;

        PostProcess(GameObject& obj) : ScriptBehaviour(obj), _camera(), _materials() {}

        Material* operator[](unsigned int i) { return _materials[i].Item2; }

    private:
        Camera* _camera;

        List<Tuple2<bool, Material*>> _materials;
    };
}