#pragma once

#include <memory>
#include <Bullet/btBulletDynamicsCommon.h>

#include "Rigidbody.h"

namespace OpenGL_Core
{
    using namespace std;

    class Physics
    {
    public :
        void ExcuteUpdate();

        /// <summary>
        /// 添加刚体
        /// </summary>
        /// <param name="rigidbody"></param>
        void AddRigidbody(Rigidbody& rigidbody);
        /// <summary>
        /// 移除刚体
        /// </summary>
        /// <param name="rigidbody"></param>
        void RemoveRigidbody(Rigidbody& rigidbody);

        Physics();
        ~Physics();

    private:
        unique_ptr<btDefaultCollisionConfiguration> CollisionConfiguration;
        unique_ptr<btCollisionDispatcher> Dispatcher;
        unique_ptr<btBroadphaseInterface> OverlappingPairCache;
        unique_ptr<btSequentialImpulseConstraintSolver> Solver;
        unique_ptr<btDiscreteDynamicsWorld> DynamicsWorld;

        List<Rigidbody*> RigidbodyList;
    };
}