﻿#include "pch.h"

#pragma comment(lib,"Bullet/LinearMath_Debug.lib")
#pragma comment(lib,"Bullet/BulletCollision_Debug.lib")
#pragma comment(lib,"Bullet/BulletDynamics_Debug.lib")
#pragma comment(lib,"Bullet/BulletSoftBody_Debug.lib")

#include "SceneManager.h"
#include "Rigidbody.h"

namespace OpenGL_Core
{
    struct CustomBtMotionState : public btMotionState
    {
        virtual void getWorldTransform(btTransform& centerOfMassWorldTrans) const
        {
            auto pos = _transform.GetPosition();
            centerOfMassWorldTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
            auto rot = _transform.GetRotation();
            centerOfMassWorldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        }

        virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
        {
            auto& pos = centerOfMassWorldTrans.getOrigin();
            _transform.SetPosition(Vector3(pos.x(), pos.y(), pos.z()));
            auto rot = centerOfMassWorldTrans.getRotation();
            _transform.SetRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
        }

        CustomBtMotionState(Transform& trans) : _transform(trans)
        {}

    private:
        Transform& _transform;
    };

    void Rigidbody::SetCollider(Collider& shape)
    {
        _collider = &shape;

        if (_btRigidbody)
        {
            SceneManager::GetActiveScene().GetPhysicsEngine().RemoveRigidbody(*this);

            _btRigidbody->setCollisionShape(_collider->GetBtCollisionShape());
            if (!_btMotionState)
            {
                _btMotionState = unique_ptr<btMotionState>(new CustomBtMotionState(GetTransform()));
                _btRigidbody->setMotionState(_btMotionState.get());
            }
            SceneManager::GetActiveScene().GetPhysicsEngine().AddRigidbody(*this);
        }
        else
        {
            _btMotionState = unique_ptr<btMotionState>(new CustomBtMotionState(GetTransform()));
            _btRigidbody = unique_ptr<btRigidBody>(new btRigidBody(1.0f, _btMotionState.get(), _collider->GetBtCollisionShape()));

            // 设置为运动学(Kinematic)刚体
            //_btRigidbody->setCollisionFlags(_btRigidbody->getCollisionFlags() 
            //    | btCollisionObject::CF_KINEMATIC_OBJECT);
            //_btRigidbody->setActivationState(DISABLE_DEACTIVATION);

            SceneManager::GetActiveScene().GetPhysicsEngine().AddRigidbody(*this);
        }
    }

    void Rigidbody::SetMass(float mass)
    {
        bool isDynamic = (mass != 0.0f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            _btRigidbody->getCollisionShape()->calculateLocalInertia(mass, localInertia);

        _btRigidbody->setMassProps(mass, localInertia);
    }

    Rigidbody::Rigidbody(GameObject& obj) : Behaviour(obj)
    {
        _collider = nullptr;
        _btRigidbody = nullptr;
        _btMotionState = nullptr;

        //auto bt_transform = btTransform();
        //bt_transform.setIdentity();
        //SetBtTransform(bt_transform, obj.GetTransform());

        //auto* motionState = new btDefaultMotionState(bt_transform);

        //auto* shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
        ////auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(1.0f, motionState, nullptr);
        //auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(1.0f, motionState, shape);
        //_btRigidbody = new btRigidBody(rbInfo);

        //_btRigidbody = new btRigidBody(1.0f, motionState, nullptr);

        //SceneManager::GetActiveScene().GetPhysicsEngine().AddRigidbody(*this);
    }
    Rigidbody::~Rigidbody()
    {
        SceneManager::GetActiveScene().GetPhysicsEngine().RemoveRigidbody(*this);
        if (_btMotionState)
            _btMotionState.reset();
        if (_btRigidbody)
            _btRigidbody.reset();
    }
}


//void TestBullet()
//{
//    // collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
//    // 碰撞配置包含内存的默认设置，即碰撞设置。 高级用户可以创建自己的配置。
//    auto collisionConfiguration = unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
//
//    // use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
//    // 使用默认的碰撞调度程序。 对于并行处理，您可以使用其他调度程序（请参阅Extras / Bullet MultiThreaded）
//    auto dispatcher = unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(collisionConfiguration.get()));
//
//    // btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
//    // btDbvtBroadphase是一个很好的通用广义阶段。 您也可以尝试btAxis3Sweep。
//    auto overlappingPairCache = unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());
//
//    // the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
//    // 默认约束求解器。 对于并行处理，可以使用其他求解器（请参见Extras / BulletMultiThreaded）
//    auto solver = unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
//
//    auto dynamicsWorld = unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get()));
//    dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
//
//    btAlignedObjectArray<btCollisionShape*> collisionShapes;
//
//    {
//        //the ground is a cube of side 100 at position y = -56.
//        // 地面是位置y = -56的方100的立方体。
//        //the sphere will hit it at y = -6, with center at -5
//        // 球体将以y = -6击中球，中心为-5
//        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
//        collisionShapes.push_back(groundShape);
//
//        btTransform groundTransform;
//        groundTransform.setIdentity();
//        groundTransform.setOrigin(btVector3(0, -56, 0));
//
//        btScalar mass(0.);
//
//        //rigidbody is dynamic if and only if mass is non zero, otherwise static
//        // 当且仅当质量非零时，刚体是动态的，否则是静态的
//        bool isDynamic = (mass != 0.f);
//
//        btVector3 localInertia(0, 0, 0);
//        if (isDynamic)
//            groundShape->calculateLocalInertia(mass, localInertia);
//
//        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
//        // 使用motionstate是可选的，它提供插值功能，并且仅同步“活动”对象
//        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
//        btRigidBody* body = new btRigidBody(rbInfo);
//
//        //add the body to the dynamics world
//        dynamicsWorld->addRigidBody(body);
//    }
//    {
//        // create a dynamic rigidbody
//        // 创建动态刚体
//
//        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
//        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
//        collisionShapes.push_back(colShape);
//
//        // Create Dynamic Objects
//        // 创建动态对象
//        btTransform startTransform;
//        startTransform.setIdentity();
//
//        btScalar mass(1.f);
//
//        // rigidbody is dynamic if and only if mass is non zero, otherwise static
//        // 当且仅当质量非零时，刚体是动态的，否则是静态的
//        bool isDynamic = (mass != 0.f);
//
//        btVector3 localInertia(0, 0, 0);
//        if (isDynamic)
//            colShape->calculateLocalInertia(mass, localInertia);
//
//        startTransform.setOrigin(btVector3(0, 100, 0));
//
//        // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//        // 建议使用motionstate，它提供插值功能，并且仅同步“活动”对象
//        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//        btRigidBody* body = new btRigidBody(rbInfo);
//
//        dynamicsWorld->addRigidBody(body);
//    }
//
//    for (auto i = 0; i < 20; i++)
//    {
//        dynamicsWorld->stepSimulation(1, 60, 1.0f / 60.0f);
//
//        //print positions of all objects
//        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
//        {
//            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
//            btRigidBody* body = btRigidBody::upcast(obj);
//            btTransform trans;
//            if (body && body->getMotionState())
//            {
//                body->getMotionState()->getWorldTransform(trans);
//            }
//            else
//            {
//                trans = obj->getWorldTransform();
//            }
//            if (j == 0)continue;
//            printf("%d  world pos object %d = %f,%f,%f\n", i, j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
//        }
//    }
//
//    // remove the rigidbodies from the dynamics world and delete them
//    // 从动力学世界中删除刚体并删除它们
//    for (auto i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
//    {
//        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
//        btRigidBody* body = btRigidBody::upcast(obj);
//        if (body && body->getMotionState())
//        {
//            delete body->getMotionState();
//        }
//        dynamicsWorld->removeCollisionObject(obj);
//        delete obj;
//    }
//
//    //delete collision shapes
//    for (int j = 0; j < collisionShapes.size(); j++)
//    {
//        btCollisionShape* shape = collisionShapes[j];
//        collisionShapes[j] = 0;
//        delete shape;
//    }
//}