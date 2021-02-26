#include "pch.h"
#include "CapsuleCollider.h"

#include "Physics.h"

namespace OpenGL_Core
{
    btCollisionShape* CapsuleCollider::GetBtCollisionShape()
    {
        return _btCapsuleShape.get();
    }

    void CapsuleCollider::Initialize(const Vector3& center, float radius, float height, int direction)
    {
        Center = center;

        switch (direction)
        {
        case 0:
            _btCapsuleShape = unique_ptr<btCapsuleShapeX>(new btCapsuleShapeX(radius, height));
            break;
        case 1:
            _btCapsuleShape = unique_ptr<btCapsuleShape>(new btCapsuleShape(radius, height));
            break;
        case 2:
            _btCapsuleShape = unique_ptr<btCapsuleShapeZ>(new btCapsuleShapeZ(radius, height));
            break;
        default: break;
        }
    }

    CapsuleCollider::CapsuleCollider(GameObject& obj) : Collider(obj)
    {
        Center = Vector3::Zero;
        _btCapsuleShape = nullptr;
        //_btCapsuleShape = unique_ptr<btCapsuleShape>(new btCapsuleShape(0.5f, 2.0f));
    }
}