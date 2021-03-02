#include "pch.h"
#include "SphereCollider.h"

#include "GameObject.h"
#include "Physics.h"

namespace OpenGL_Core
{
    btCollisionShape* SphereCollider::GetBtCollisionShape()
    {
        return _btSphereShape.get();
    }
    void SphereCollider::SetRadius(float radius)
    {
        _btSphereShape->setUnscaledRadius(radius);
    }
    void SphereCollider::Initialize(float radius, const Vector3& center)
    {
        Center = center;
        SetRadius(radius);
    }
    SphereCollider::SphereCollider(GameObject& obj) : Collider(obj)
    {
        Center = Vector3::Zero;
        _btSphereShape = unique_ptr<btSphereShape>(new btSphereShape(0.5f));
        auto size = obj.GetTransform().GetLossyScale();
        _btSphereShape->setLocalScaling(btVector3(size.x, size.y, size.z));
    }
}

