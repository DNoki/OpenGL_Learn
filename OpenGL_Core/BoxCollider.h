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
        /// <summary>
        /// 在该对象本地空间中测量的盒体中心。
        /// </summary>
        Vector3 Center;

        btCollisionShape* GetBtCollisionShape() override;

        /// <summary>
        /// 设置盒体大小
        /// </summary>
        /// <param name="size"></param>
        void SetSize(Vector3 size);

        BoxCollider(GameObject& obj);

    private:
        unique_ptr<btBoxShape> _btBoxShape;
    };
}

