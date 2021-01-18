#include "pch.h"

#include "Physics.h"


#pragma comment(lib,"Bullet/LinearMath_Debug.lib")
#pragma comment(lib,"Bullet/BulletCollision_Debug.lib")
#pragma comment(lib,"Bullet/BulletDynamics_Debug.lib")
#pragma comment(lib,"Bullet/BulletSoftBody_Debug.lib")

#include "Time.h"

namespace OpenGL_Core
{
    using namespace OpenGL_Core;

    Vector3 ToVector3(const btVector3& v)
    {
        return Vector3(v.getX(), v.getY(), v.getZ());
    }
    btVector3 ToVector3(const Vector3& v)
    {
        return btVector3(v.x, v.t, v.z);
    }
    Quaternion ToQuaternion(const btQuaternion& q)
    {
        return Quaternion(q.getX(), q.getY(), q.getZ(), q.getW());
    }
    btQuaternion ToQuaternion(const Quaternion& q)
    {
        return btQuaternion(q.x, q.y, q.z, q.w);
    }
    void SetTransform(Transform& transform, btTransform& bt_transform)
    {
        transform.SetPosition(ToVector3(bt_transform.getOrigin()));

        auto& btRotMatrix = bt_transform.getBasis();
        auto column0 = ToVector3(btRotMatrix.getColumn(0));
        auto column1 = ToVector3(btRotMatrix.getColumn(1));
        auto column2 = ToVector3(btRotMatrix.getColumn(2));
        auto rotMatrix = Matrix4x4(
            column0.x, column0.y, column0.z, 0,
            column1.x, column1.y, column1.z, 0,
            column2.x, column2.y, column2.z, 0,
            0, 0, 0, 1
        );
        transform.SetRotation(rotMatrix.QuaternionCast());
    }
    void SetBtTransform(btTransform& bt_transform, Transform& transform)
    {
        bt_transform.setOrigin(ToVector3(transform.GetPosition()));

        auto rotMatrix = transform.GetRotationMatrix();
        auto btRotMatrix = btMatrix3x3(
            rotMatrix[0].x, rotMatrix[0].y, rotMatrix[0].z,
            rotMatrix[1].x, rotMatrix[1].y, rotMatrix[1].z,
            rotMatrix[2].x, rotMatrix[2].y, rotMatrix[2].z
        );
        bt_transform.setBasis(btRotMatrix);
    }

    void Physics::ExcuteUpdate()
    {
        DynamicsWorld->stepSimulation(Time::DeltaTime(), 0);

        for (size_t i = 0; i < RigidbodyList.size(); i++)
        {
            auto* rigid = RigidbodyList[i];
            SetTransform(rigid->GetTransform(), rigid->GetBtRigidBody()->getWorldTransform());
        }
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
    Physics::Physics()
    {
        CollisionConfiguration = unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
        Dispatcher = unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(CollisionConfiguration.get()));
        OverlappingPairCache = unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());
        Solver = unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
        DynamicsWorld = unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(Dispatcher.get(), OverlappingPairCache.get(), Solver.get(), CollisionConfiguration.get()));
        DynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

        RigidbodyList = List<Rigidbody*>();
    }
    Physics::~Physics()
    {
        //for (auto i = DynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        //{
        //    auto* obj = DynamicsWorld->getCollisionObjectArray()[i];
        //    auto* body = btRigidBody::upcast(obj);
        //    if (body && body->getMotionState())
        //    {
        //        delete body->getMotionState();
        //    }
        //    DynamicsWorld->removeCollisionObject(obj);
        //    delete obj;
        //}
        for (int i = RigidbodyList.size() - 1; i >= 0; i--)
        {
            DynamicsWorld->removeCollisionObject(RigidbodyList[i]->GetBtRigidBody());
            RigidbodyList.Remove(RigidbodyList[i]);
        }

        DynamicsWorld.reset();
        Solver.reset();
        OverlappingPairCache.reset();
        Dispatcher.reset();
        CollisionConfiguration.reset();
    }
}