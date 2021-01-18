#pragma once

#include "../OpenGL_Core/OpenGL_CoreCommon.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class ChildGameObject final : public ScriptBehaviour
    {
    public:
        GameObject* fatherObj;

        void Awake() override
        {
            fatherObj = SceneManager::GetActiveScene().FindGameObject("Father");

            //GetGameObject().AddComponent<MeshRenderer>().AddData(
            //    *SceneManager::GetActiveScene().FindResourceObject<Mesh>("Box"),
            //    *SceneManager::GetActiveScene().FindResourceObject<Material>("Test Material"));


            //childObj->GetTransform().LocalRotation = angleAxis(3.1415926f / 4.0f, V3_FORWARD);
            GetTransform().SetParent(&Camera::GetMain()->GetTransform(), false);
            //GetTransform().SetPosition(vec3(0, 0, 40), false);
            //GetTransform().LocalScale = V3_ONE * 0.2f;
        }
        void Update() override
        {

            //GetTransform().Rotate(V3_UP, 360 * Time::DeltaTime());
            GetTransform().SetPosition(Camera::GetMain()->GetTransform().GetPosition()
                + Camera::GetMain()->GetTransform().GetForward() * 20.0f
            );
        }

        ChildGameObject(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
