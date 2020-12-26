#pragma once

#include "ScriptBehaviour.h"

namespace OpenGL_Learn
{

    class CameraController final : public ScriptBehaviour
    {
    public:
        void Awake() override
        {
            GetTransform().LocalPosition = Vector3(0.0f, 1.0f, 8.0f);
            //GetTransform().LocalPosition = Vector3(0.0f, 0.0f, -20.0f);
            GetTransform().LookAt(Vector3::Zero);
        }
        void Update() override
        {
            if (InputSystem::GetKey(KeyCode::LEFT))
            {
                GetTransform().RotateAround(Vector3::Zero, Vector3::Up, Time::DeltaTime() * 180.0f);
            }
            else if (InputSystem::GetKey(KeyCode::RIGHT))
            {
                GetTransform().RotateAround(Vector3::Zero, Vector3::Up, -Time::DeltaTime() * 180.0f);
            }
        }

        CameraController(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
