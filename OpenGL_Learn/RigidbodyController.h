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

            rigidbody->GetBtRigidBody()->setLinearVelocity(btVector3(10, 0, 10));
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
        }

        RigidbodyController(GameObject& obj) :ScriptBehaviour(obj) {}
    };

}
