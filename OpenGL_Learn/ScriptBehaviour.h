#pragma once

#include <sstream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Behaviour.h"

#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Light.h"

#include "InputSystem.h"



namespace OpenGL_Learn
{
    // 脚本行为 每个脚本需要继承该类
    class ScriptBehaviour : public Behaviour
    {
    public:
        // 加载脚本实例时将调用。
        virtual void Awake() {}
        virtual void Start() {}
        // 会在每一帧调用
        virtual void Update() {}
        // 会在每一帧Update()调用结束后调用
        virtual void LateUpdate() {}
        // 相机渲染场景后调用。 仅应使用此功能进行绘制
        //virtual void OnRenderObject() {}
        // 在完成所有渲染调用。 后处理效果。
        virtual void OnRenderImage(RenderTexture* targetTex) {}
        virtual void OnGui() {}


        // 在摄像机剔除场景之前调用。 剔除确定相机可以看到哪些对象。
        virtual void OnPreCull() {}
        // 在照相机开始渲染场景之前调用。 仅当脚本已附加到相机并已启用时，才调用此功能。
        virtual void OnPreRender() {}
        // 相机完成场景渲染后调用。 仅当脚本已附加到相机并已启用时，才调用此功能。
        virtual void OnPostRender() {}


        virtual void OnConsoleBar(int index, stringstream& fmt)
        {
            fmt.str(""); fmt << index << " " << typeid(*this).name() << "  0x" << this;
            if (ImGui::TreeNode(fmt.str().c_str()))
            {
                ImGui::TreePop();
            }
        }

        inline bool GetStarted() { return _isStarted; }
        inline void SetStarted() { _isStarted = true; }
        inline bool GetAwaked() { return _isAwaked; }
        inline void SetAwaked() { _isAwaked = true; }

        ScriptBehaviour(GameObject& obj) :Behaviour(obj),
            _isStarted(false), _isAwaked(false) {}
        virtual ~ScriptBehaviour() {}

    private:
        bool _isAwaked;
        bool _isStarted;
    };
}

