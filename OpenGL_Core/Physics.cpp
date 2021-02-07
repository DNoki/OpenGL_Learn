#include "pch.h"

#include "Physics.h"


#pragma comment(lib,"Bullet/LinearMath_Debug.lib")
#pragma comment(lib,"Bullet/BulletCollision_Debug.lib")
#pragma comment(lib,"Bullet/BulletDynamics_Debug.lib")
#pragma comment(lib,"Bullet/BulletSoftBody_Debug.lib")

#include "Time.h"

namespace OpenGL_Core
{

    //Vector3 Physics::ToVector3(const btVector3& v)
    //{
    //    return Vector3(v.getX(), v.getY(), v.getZ());
    //}
    //btVector3 Physics::ToVector3(const Vector3& v)
    //{
    //    return btVector3(v.x, v.t, v.z);
    //}
    //Quaternion Physics::ToQuaternion(const btQuaternion& q)
    //{
    //    return Quaternion(q.getX(), q.getY(), q.getZ(), q.getW());
    //}
    //btQuaternion Physics::ToQuaternion(const Quaternion& q)
    //{
    //    return btQuaternion(q.x, q.y, q.z, q.w);
    //}

    void Physics::ExcuteUpdate()
    {
        DynamicsWorld->stepSimulation(Time::DeltaTime(), 50, 0.02f);
    }
    void Physics::AddRigidbody(Rigidbody& rigidbody)
    {
        if (RigidbodyList.Exists(&rigidbody))
            return;
        RigidbodyList.push_back(&rigidbody);
        DynamicsWorld->addRigidBody(rigidbody.GetBtRigidBody());
    }
    void Physics::RemoveRigidbody(Rigidbody& rigidbody)
    {
        if (RigidbodyList.Exists(&rigidbody))
        {
            RigidbodyList.Remove(&rigidbody);
            DynamicsWorld->removeCollisionObject(rigidbody.GetBtRigidBody());
        }
    }
    void Physics::AddCollisionObject(CollisionObject& collisionObject)
    {
        if (ColliderList.Exists(&collisionObject))
            return;
        ColliderList.push_back(&collisionObject);
        DynamicsWorld->addCollisionObject(collisionObject.GetBtCollisionObject());
    }
    void Physics::RemoveCollisionObject(CollisionObject& collisionObject)
    {
        if (ColliderList.Exists(&collisionObject))
        {
            ColliderList.Remove(&collisionObject);
            DynamicsWorld->removeCollisionObject(collisionObject.GetBtCollisionObject());
        }
    }
    Physics::Physics()
    {
        CollisionConfiguration = unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
        Dispatcher = unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(CollisionConfiguration.get()));
        OverlappingPairCache = unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());
        Solver = unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
        DynamicsWorld = unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(Dispatcher.get(), OverlappingPairCache.get(), Solver.get(), CollisionConfiguration.get()));
        DynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

        ColliderList = List<CollisionObject*>();
    }
    Physics::~Physics()
    {
        for (int i = DynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            DynamicsWorld->removeCollisionObject(DynamicsWorld->getCollisionObjectArray()[i]);
        }
        RigidbodyList.clear();
        ColliderList.clear();

        DynamicsWorld.reset();
        Solver.reset();
        OverlappingPairCache.reset();
        Dispatcher.reset();
        CollisionConfiguration.reset();
    }
}