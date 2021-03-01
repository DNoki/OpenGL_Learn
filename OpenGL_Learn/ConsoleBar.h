#pragma once

#include "../OpenGL_Core/OpenGL_CoreCommon.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class ConsoleBar final : public ScriptBehaviour
    {
    public:
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        static void CheckGameObject(int index, stringstream& fmt, GameObject& obj);
        static bool CheckCamera(int index, stringstream& fmt, Component* component);
        //static bool CheckPostProcess(int index, stringstream& fmt, Component* component);
        static bool CheckDirLight(int index, stringstream& fmt, Component* component);
        static bool CheckPointLight(int index, stringstream& fmt, Component* component);
        static bool CheckRenderer(int index, stringstream& fmt, Component* component);

        static void CheckResources(map<string, List<unique_ptr<ResourceObject>>>& objs);
        static bool CheckShader(int index, stringstream& fmt, ResourceObject* obj);
        static bool CheckMaterial(int index, stringstream& fmt, ResourceObject* obj);
        static bool CheckMesh(int index, stringstream& fmt, ResourceObject* obj);
        static bool CheckTexture(int index, stringstream& fmt, ResourceObject* obj);
        static bool CheckRenderTexture(int index, stringstream& fmt, ResourceObject* obj);

        void Awake() override;

        void Update() override;

        void OnGui() override;

        ConsoleBar(GameObject& obj) :ScriptBehaviour(obj) {}
        ~ConsoleBar();
    };


}
