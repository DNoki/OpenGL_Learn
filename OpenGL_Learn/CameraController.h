﻿#pragma once

#include "../OpenGL_Core/ScriptBehaviour.h"


namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class CameraController final : public ScriptBehaviour
    {
    public:
        void Awake() override;
        void Update() override;

        CameraController(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}