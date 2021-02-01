#include "pch.h"

#include "BoxCollider.h"

#include "GameObject.h"
#include "Physics.h"

namespace OpenGL_Core
{
    btCollisionShape* BoxCollider::GetBtCollisionShape()
    {
        return _btBoxShape.get();
    }

    void BoxCollider::SetSize(Vector3 size)
    {
        _btBoxShape->setLocalScaling(btVector3(size.x, size.y, size.z));
    }

    BoxCollider::BoxCollider(GameObject& obj) : Collider(obj)
    {
        Center = Vector3::Zero;
        _btBoxShape = unique_ptr<btBoxShape>(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
        SetSize(obj.GetTransform().GetLossyScale());
    }
}

