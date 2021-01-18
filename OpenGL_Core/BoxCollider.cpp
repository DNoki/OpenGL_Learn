#include "pch.h"

#include "BoxCollider.h"

#include "GameObject.h"

namespace OpenGL_Core
{
    extern Vector3 ToVector3(const btVector3& v);
    extern btVector3 ToVector3(const Vector3& v);
    extern Quaternion ToQuaternion(const btQuaternion& q);
    extern btQuaternion ToQuaternion(const Quaternion& q);
    extern void SetTransform(Transform& transform, btTransform& bt_transform);
    extern void SetBtTransform(btTransform& bt_transform, Transform& transform);

    btCollisionShape* BoxCollider::GetBtCollisionShape()
    {
        return _btBoxShape.get();
    }

    void BoxCollider::SetSize(Vector3 size)
    {
        _btBoxShape->setLocalScaling(ToVector3(size));
    }

    BoxCollider::BoxCollider(GameObject& obj) : Collider(obj)
    {
        Center = Vector3::Zero;
        _btBoxShape = unique_ptr<btBoxShape>(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
        SetSize(obj.GetTransform().GetLossyScale());
    }
}

