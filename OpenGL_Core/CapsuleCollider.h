#pragma once

#include "Collider.h"

namespace OpenGL_Core
{
    class CapsuleCollider : public Collider
    {
    public:

        btCollisionShape* GetBtCollisionShape() override;

        void Initialize(const Vector3& center, float radius, float height, int direction = 1);

        CapsuleCollider(GameObject& obj);

    private:
        unique_ptr<btCapsuleShape> _btCapsuleShape;
    };
}


