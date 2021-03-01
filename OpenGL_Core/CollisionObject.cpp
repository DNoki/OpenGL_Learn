#include "pch.h"

#include "CollisionObject.h"

#include "SceneManager.h"

namespace OpenGL_Core
{
    btTransform ToTransform(const Transform& transform, const  Vector3& offset)
    {
        btTransform bt_transform = btTransform::getIdentity();
        auto pos = transform.GetPosition() + transform.GetLossyScale() * offset;
        bt_transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
        auto rot = transform.GetRotation();
        bt_transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        return bt_transform;
    }

    void CollisionObject::SetFriction(float friction)
    {
        _btCollisionObject->setFriction(friction);
    }
    float CollisionObject::GetFriction()
    {
        return _btCollisionObject->getFriction();
    }
    void CollisionObject::SetRestitution(float restitution)
    {
        _btCollisionObject->setRestitution(restitution);
    }
    float CollisionObject::GetRestitution()
    {
        return _btCollisionObject->getRestitution();
    }

    void CollisionObject::Initialize(Collider& collider)
    {
        SetCollider(collider);

        auto flag = _btCollisionObject->getCollisionFlags();
        flag &= ~(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_DYNAMIC_OBJECT);
        flag |= btCollisionObject::CF_STATIC_OBJECT;
        _btCollisionObject->setCollisionFlags(flag);
    }
    void CollisionObject::OnEnable()
    {
        SceneManager::GetActiveScene()->GetPhysicsEngine().AddCollisionObject(*this);
        //if (_btCollisionObject->isKinematicObject())
        //    _btCollisionObject->setActivationState(WANTS_DEACTIVATION);
        //else
        //    _btCollisionObject->setActivationState(ACTIVE_TAG);
    }
    void CollisionObject::OnDisable()
    {
        SceneManager::GetActiveScene()->GetPhysicsEngine().RemoveCollisionObject(*this);
        //_btCollisionObject->setActivationState(DISABLE_SIMULATION);
    }

    CollisionObject::CollisionObject(GameObject& obj) : Behaviour(obj), _collider(), _btCollisionObject() {}

    CollisionObject::~CollisionObject()
    {
        SceneManager::GetActiveScene()->GetPhysicsEngine().RemoveCollisionObject(*this);
        if (_btCollisionObject)
            _btCollisionObject.reset();
    }

    void CollisionObject::SetCollider(Collider& shape)
    {
        _collider = &shape;

        if (_btCollisionObject)
        {
            SceneManager::GetActiveScene()->GetPhysicsEngine().RemoveCollisionObject(*this);
            _btCollisionObject->setCollisionShape(_collider->GetBtCollisionShape());
            _btCollisionObject->setWorldTransform(ToTransform(GetTransform(), _collider->Center));

            SceneManager::GetActiveScene()->GetPhysicsEngine().AddCollisionObject(*this);
        }
        else
        {
            _btCollisionObject = unique_ptr<btCollisionObject>(new btCollisionObject());
            _btCollisionObject->setCollisionShape(shape.GetBtCollisionShape());
            _btCollisionObject->setWorldTransform(ToTransform(GetTransform(), _collider->Center));

            SceneManager::GetActiveScene()->GetPhysicsEngine().AddCollisionObject(*this);
        }
    }
}