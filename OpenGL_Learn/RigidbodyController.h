#pragma once

#include "../OpenGL_Core/OpenGL_CoreCommon.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class RigidbodyController final : public ScriptBehaviour
    {
    public:
        Rigidbody* rigidbody;

        void Awake() override
        {
            rigidbody = GetComponent<Rigidbody>();

            //rigidbody->GetBtRigidBody()->setFriction(1);

            //rigidbody->SetIsKinematic(true);

            //rigidbody->GetBtRigidBody()->setLinearVelocity(btVector3(1, 0, -10));
        }
        void Update() override
        {
            //auto linearVelocity = Physics::ToVector3(rigidbody->GetBtRigidBody()->getLinearVelocity());
            //auto pushVelocity = Physics::ToVector3(rigidbody->GetBtRigidBody()->getPushVelocity());
            //auto turnVelocity = Physics::ToVector3(rigidbody->GetBtRigidBody()->getTurnVelocity());

            //cout
            //    << "LinearVelocity:" << linearVelocity.ToString() << endl
            //    << "PushVelocity:" << pushVelocity.ToString() << endl
            //    << "TurnVelocity:" << turnVelocity.ToString() << endl

            //    << endl
            //    << endl;
            if (InputSystem::GetKeyDown(KeyCode::P))
            {
                rigidbody->SetIsKinematic(false);
            }
            else if (InputSystem::GetKeyDown(KeyCode::O))
            {
                rigidbody->SetIsKinematic(true);
            }
        }

        RigidbodyController(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
