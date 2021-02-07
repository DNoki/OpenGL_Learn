#pragma once

#include "Behaviour.h"

#include "Collider.h"

namespace OpenGL_Core
{
    /// <summary>
    /// 碰撞器
    /// </summary>
    class CollisionObject : public Behaviour
    {
    public:
        inline btCollisionObject* GetBtCollisionObject() { return _btCollisionObject.get(); }

        /// <summary>
        /// 设置摩擦系数
        /// </summary>
        /// <param name="friction"></param>
        virtual void SetFriction(float friction);
        virtual float GetFriction();
        /// <summary>
        /// 设置弹性系数
        /// </summary>
        /// <param name="restitution"></param>
        virtual void SetRestitution(float restitution);
        virtual float GetRestitution();

        void Initialize(Collider& collider);
        void OnEnable() override;
        void OnDisable() override;

        CollisionObject(GameObject& obj);
        ~CollisionObject();

    protected:
        Collider* _collider;
        unique_ptr<btCollisionObject> _btCollisionObject;

        /// <summary>
        /// 设置碰撞体
        /// </summary>
        /// <param name="shape"></param>
        virtual void SetCollider(Collider& shape);

    private:


    };

}

