#pragma once

#include "Rigidbody.h"

namespace OpenGL_Core
{
    using namespace std;

    class Physics
    {
    public :
        static void SetDebugEnable(bool enable);
        void ExcuteUpdate();
        void DebugDraw();

        /// <summary>
        /// 添加碰撞器
        /// </summary>
        /// <param name="collider"></param>
        void AddCollisionObject(CollisionObject& collider);
        /// <summary>
        /// 移除碰撞器
        /// </summary>
        /// <param name="collider"></param>
        void RemoveCollisionObject(CollisionObject& collider);

        Physics();
        ~Physics();

    private:
        unique_ptr<btDefaultCollisionConfiguration> CollisionConfiguration;
        unique_ptr<btCollisionDispatcher> Dispatcher;
        unique_ptr<btBroadphaseInterface> OverlappingPairCache;
        unique_ptr<btSequentialImpulseConstraintSolver> Solver;
        unique_ptr<btDiscreteDynamicsWorld> DynamicsWorld;

        List<CollisionObject*> CollisionList;
    };
}