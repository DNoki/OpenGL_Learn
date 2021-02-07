#pragma once

#include <Bullet/btBulletDynamicsCommon.h>

#include "Component.h"
#include "Math.h"

namespace OpenGL_Core
{
    /// <summary>
    /// 所有碰撞体的基类。
    /// </summary>
    class Collider : public Component
    {
    public:
        /// <summary>
        /// 在该对象本地空间中测量的盒体中心。
        /// </summary>
        Vector3 Center;

        virtual btCollisionShape* GetBtCollisionShape() = 0;

        Collider(GameObject& obj) : Component(obj) {}
    };
}

