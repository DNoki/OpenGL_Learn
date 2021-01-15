#pragma once

#include "Behaviour.h"

//#include "Camera.h"
//#include "Mesh.h"
//#include "Material.h"
//#include "RenderTexture.h"
//#include "Renderer.h"
//#include "Light.h"

#include "InputSystem.h"



namespace OpenGL_Core
{
    class RenderTexture;

    /// <summary>
    /// 脚本行为 每个脚本需要继承该类
    /// </summary>
    class ScriptBehaviour : public Behaviour
    {
    public:
        /// <summary>
        /// 加载脚本实例时将调用。
        /// </summary>
        virtual void Awake() {}
        /// <summary>
        /// 第一次渲染前调用
        /// </summary>
        virtual void Start() {}
        /// <summary>
        /// 会在每一帧调用
        /// </summary>
        virtual void Update() {}
        /// <summary>
        /// 会在每一帧Update()调用结束后调用
        /// </summary>
        virtual void LateUpdate() {}
        /// <summary>
        /// 相机渲染场景后调用。 仅应使用此功能进行绘制
        /// </summary>
        //virtual void OnRenderObject() {}

        /// <summary>
        /// 在完成所有渲染调用。 后处理效果。
        /// </summary>
        /// <param name="targetTex"></param>
        virtual void OnRenderImage(RenderTexture* targetTex) {}
        virtual void OnGui() {}


        /// <summary>
        /// 在摄像机剔除场景之前调用。 剔除确定相机可以看到哪些对象。
        /// </summary>
        virtual void OnPreCull() {}
        /// <summary>
        /// 在照相机开始渲染场景之前调用。 仅当脚本已附加到相机并已启用时，才调用此功能。
        /// </summary>
        virtual void OnPreRender() {}
        /// <summary>
        /// 相机完成场景渲染后调用。 仅当脚本已附加到相机并已启用时，才调用此功能。
        /// </summary>
        virtual void OnPostRender() {}

        inline bool GetStarted() { return _isStarted; }
        inline void SetStarted() { _isStarted = true; }

        ScriptBehaviour(GameObject& obj) :Behaviour(obj),
            _isStarted(false) {}
        virtual ~ScriptBehaviour() {}

    private:
        bool _isStarted;
    };
}

