#pragma once

#include "Collider.h"

namespace OpenGL_Core
{
    /// <summary>
    /// 盒体形状的原始碰撞体。
    /// </summary>
    class BoxCollider : public Collider
    {
    public:

        btCollisionShape* GetBtCollisionShape() override;

        /// <summary>
        /// 设置盒体大小
        /// </summary>
        /// <param name="size"></param>
        void SetSize(const Vector3& size);

        void Initialize(const Vector3& size, const Vector3& center);

        BoxCollider(GameObject& obj);

    private:
        unique_ptr<btBoxShape> _btBoxShape;
    };
}

