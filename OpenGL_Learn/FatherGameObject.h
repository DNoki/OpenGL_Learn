#pragma once

#include "../OpenGL_Core/OpenGL_CoreCommon.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class FatherGameObject final : public ScriptBehaviour
    {
    public:
        float tempValue;

        void Awake() override
        {
            //GetGameObject().AddComponent<MeshRenderer>().AddData(
            //    *SceneManager::GetActiveScene().FindResourceObject<Mesh>("Box"),
            //    *SceneManager::GetActiveScene().FindResourceObject<Material>("Test Material"));
            GetTransform().SetPosition(Vector3::Right * 2.0f);

            auto light = SceneManager::GetActiveScene().FindComponent<DirectionalLight>();
            tempValue = light->Bias;
            //cout << tempValue * light->ShadowDistance << endl;
        }
        void Update() override
        {
            //GetTransform().Rotate(V3_FORWARD, 45 * Time::DeltaTime());
            //auto mat = GetComponent<Renderer>()->GetMaterial();
            //if (mat)
            //    mat->SetFloat("time", abs(sin(pi<float>() * Time::RunTime())));


            auto lights = SceneManager::GetActiveScene().FindComponents<Light>();

            for (auto light : lights)
            {
                if (InputSystem::GetKey(KeyCode::C))
                    light->GetTransform().RotateAround(Vector3::Zero, Vector3::Up, 180 * Time::DeltaTime());
                if (InputSystem::GetKey(KeyCode::X))
                    light->GetTransform().RotateAround(Vector3::Zero, Vector3::Up, -180 * Time::DeltaTime());
            }
            //GetTransform().SetPosition(dirLight->GetTransform().GetPosition().GetNormalized() * 2.0f);
            //GetTransform().SetRotation(dirLight->GetTransform().GetRotation());

        }

        FatherGameObject(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
