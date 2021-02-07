#pragma once

#include "Behaviour.h"

#include "CollisionObject.h"

namespace OpenGL_Core
{
    using namespace std;

    /// <summary>
    /// 刚体
    /// </summary>
    class Rigidbody : public CollisionObject
    {
    public:
        inline btRigidBody* GetBtRigidBody() { return btRigidBody::upcast(_btCollisionObject.get()); }

        /// <summary>
        /// 设置是否为运动学刚体
        /// </summary>
        /// <param name="isKinematic"></param>
        void SetIsKinematic(bool isKinematic);
        /// <summary>
        /// 设置线性阻尼与旋转阻尼
        /// </summary>
        /// <param name="lin"></param>
        /// <param name="ang"></param>
        void SetDamping(float lin, float ang);
        /// <summary>
        /// 设置是否开启连续碰撞检测(对于快速移动的动态物体需要开启)
        /// </summary>
        /// <param name="value"></param>
        void SetIsContinuousDynamic(bool value);

        /// <summary>
        /// 设置重量
        /// </summary>
        /// <param name="mass"></param>
        void SetMass(float mass);

        void Initialize(Collider& shape, float mass = 1.0f);

        Rigidbody(GameObject& obj);
        ~Rigidbody();

    private:
        unique_ptr<btMotionState> _btMotionState;

        void SetCollider(Collider& shape) override;
    };
}

