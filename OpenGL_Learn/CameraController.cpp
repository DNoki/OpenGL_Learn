﻿#include "pch.h"

#include "CameraController.h"

namespace OpenGL_Learn
{
    void CameraController::Awake()
    {
        GetTransform().SetPosition(Vector3(0.0f, 1.0f, 8.0f), false);
        GetTransform().LookAt(Vector3::Zero);
    }

    void CameraController::Update()
    {
        if (InputSystem::GetKey(KeyCode::Q))
        {
            GetTransform().Rotate(Vector3::Up, -Time::DeltaTime() * 90.0f);
            //GetTransform().RotateAround(Vector3::Zero, Vector3::Up, Time::DeltaTime() * 180.0f);
        }
        else if (InputSystem::GetKey(KeyCode::E))
        {
            GetTransform().Rotate(Vector3::Up, Time::DeltaTime() * 90.0f);
            //GetTransform().RotateAround(Vector3::Zero, Vector3::Up, -Time::DeltaTime() * 180.0f);
        }

        auto& transform = GetTransform();

        auto forward = transform.GetForward();
        forward.y = 0.0f;
        auto forwardRotation = Quaternion::LookRotation(forward);

        auto move = Vector3::Zero;
        if (InputSystem::GetKey(KeyCode::W))
            move.z += 1.0f;
        if (InputSystem::GetKey(KeyCode::S))
            move.z -= 1.0f;
        if (InputSystem::GetKey(KeyCode::D))
            move.x += 1.0f;
        if (InputSystem::GetKey(KeyCode::A))
            move.x -= 1.0f;
        if (InputSystem::GetKey(KeyCode::SPACE))
            move.y += 1.0f;
        if (InputSystem::GetKey(KeyCode::C))
            move.y -= 1.0f;

        move = forwardRotation * move;

        auto position = transform.GetPosition();
        move = move * Time::DeltaTime() * 10.0f;
        position = position + move;

        transform.SetPosition(position);

        // 临时
        // 旋转点光
        auto pointLights = SceneManager::GetActiveScene()->FindComponents<PointLight>();
        for (auto& light : pointLights)
        {
            light->GetTransform().RotateAround(Vector3::Zero, Vector3::Up, -Time::DeltaTime() * 30.0f);
        }

        // 切换场景
        if (InputSystem::GetKeyDown(KeyCode::F1))
            SceneManager::LoadNextScene(0);
        else if (InputSystem::GetKeyDown(KeyCode::F2))
            SceneManager::LoadNextScene(1);
        else if (InputSystem::GetKeyDown(KeyCode::F3))
            SceneManager::LoadNextScene(2);
    }
}