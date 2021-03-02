#pragma once

#include "Collider.h"

namespace OpenGL_Core
{
    /// <summary>
    /// 球形碰撞体
    /// </summary>
    class SphereCollider : public Collider
    {
    public:
        btCollisionShape* GetBtCollisionShape() override;

        void SetRadius(float radius);
        void Initialize(float radius, const Vector3& center);

        SphereCollider(GameObject& obj);

    private:
        unique_ptr<btSphereShape> _btSphereShape;
    };
}

