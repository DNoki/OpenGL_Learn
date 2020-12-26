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
    // �ű���Ϊ ÿ���ű���Ҫ�̳и���
    class ScriptBehaviour : public Behaviour
    {
    public:
        // ���ؽű�ʵ��ʱ�����á�
        virtual void Awake() {}
        virtual void Start() {}
        // ����ÿһ֡����
        virtual void Update() {}
        // ����ÿһ֡Update()���ý��������
        virtual void LateUpdate() {}
        // �����Ⱦ��������á� ��Ӧʹ�ô˹��ܽ��л���
        //virtual void OnRenderObject() {}
        // �����������Ⱦ���á� ����Ч����
        virtual void OnRenderImage(RenderTexture* targetTex) {}
        virtual void OnGui() {}


        // ��������޳�����֮ǰ���á� �޳�ȷ��������Կ�����Щ����
        virtual void OnPreCull() {}
        // ���������ʼ��Ⱦ����֮ǰ���á� �����ű��Ѹ��ӵ������������ʱ���ŵ��ô˹��ܡ�
        virtual void OnPreRender() {}
        // �����ɳ�����Ⱦ����á� �����ű��Ѹ��ӵ������������ʱ���ŵ��ô˹��ܡ�
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

