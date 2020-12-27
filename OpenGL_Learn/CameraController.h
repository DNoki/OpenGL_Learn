#pragma once

#include "ScriptBehaviour.h"

namespace OpenGL_Learn
{

    class CameraController final : public ScriptBehaviour
    {
    public:
        void Awake() override;
        void Update() override;

        CameraController(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
