#pragma once

#include "Behaviour.h"

#include "Collider.h"

namespace OpenGL_Core
{
    using namespace std;

    /// <summary>
    /// 刚体
    /// </summary>
    class Rigidbody : public Behaviour
    {
    public:
        btRigidBody* GetBtRigidBody();

        /// <summary>
        /// 设置碰撞体
        /// </summary>
        /// <param name="shape"></param>
        void SetCollider(Collider& shape);
        /// <summary>
        /// 设置重量
        /// </summary>
        /// <param name="mass"></param>
        void SetMass(float mass);

        Rigidbody(GameObject& obj);
        ~Rigidbody();

    private:
        Collider* _collider;
        btRigidBody* _btRigidbody;
    };
}

