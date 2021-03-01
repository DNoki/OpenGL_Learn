#include "pch.h"

#include "Physics.h"


#pragma comment(lib,"Bullet/LinearMath_Debug.lib")
#pragma comment(lib,"Bullet/BulletCollision_Debug.lib")
#pragma comment(lib,"Bullet/BulletDynamics_Debug.lib")
#pragma comment(lib,"Bullet/BulletSoftBody_Debug.lib")

#include "Time.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Graphics.h"

namespace OpenGL_Core
{
    class PhysicsDebugDraw : public btIDebugDraw
    {
    public:

        virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
        {
            auto pos = Vector3(from.x(), from.y(), from.z());
            _mesh->vertices.push_back(pos);
            pos = Vector3(to.x(), to.y(), to.z());
            _mesh->vertices.push_back(pos);

            auto c = Vector4(color.x(), color.y(), color.z(), 1.0f);
            _mesh->colors.push_back(c);
            _mesh->colors.push_back(c);
        }
        virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {}
        virtual void reportErrorWarning(const char* warningString) override {}
        virtual void draw3dText(const btVector3& location, const char* textString) override {}
        virtual void setDebugMode(int debugMode) override { _debugMode = debugMode; }
        virtual int getDebugMode() const override { return _debugMode; }

        void PreDebugDraw()
        {
            _mesh->Clear();
            _mesh->SetRenderMode(RenderMode::LINES);
        }
        void OnDebugDraw()
        {
            if (_mesh->vertices.size() > 0)
            {
                _mesh->Complete();
                Graphics::DrawMesh(*_mesh, *_material, 0);
            }
        }

        PhysicsDebugDraw()
        {
            _debugMode = btIDebugDraw::DBG_NoDebug;
            //_debugMode = btIDebugDraw::DBG_DrawWireframe;
            _shader = unique_ptr<Shader>(new Shader("Debug Shader", "../Asset/Shader/Auxiliary/DebugDraw.glsl"));
            _material = unique_ptr<Material>(new Material("Debug Material", _shader.get()));
            _mesh = nullptr;
            _mesh = unique_ptr<Mesh>(new Mesh("Debug Mesh"));
            _mesh->SetRenderMode(RenderMode::LINES);
        }
        ~PhysicsDebugDraw() {}

    private:
        int _debugMode;
        unique_ptr<Shader> _shader;
        unique_ptr<Material> _material;
        unique_ptr<Mesh> _mesh;
    };

    unique_ptr<PhysicsDebugDraw> DebugDrawInstance;

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

    void Physics::SetDebugEnable(bool enable)
    {
        if (enable)
            DebugDrawInstance->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        else DebugDrawInstance->setDebugMode(btIDebugDraw::DBG_NoDebug);
    }

    void Physics::ExcuteUpdate()
    {
        DynamicsWorld->stepSimulation(Time::DeltaTime(), 50, 0.02f);
    }

    void Physics::DebugDraw()
    {
        DebugDrawInstance->PreDebugDraw();
        DynamicsWorld->debugDrawWorld();
        DebugDrawInstance->OnDebugDraw();
    }

    void Physics::AddCollisionObject(CollisionObject& collisionObject)
    {
        if (CollisionList.Exists(&collisionObject))
            return;
        CollisionList.push_back(&collisionObject);

        btRigidBody* rigidbody = btRigidBody::upcast(collisionObject.GetBtCollisionObject());
        if (rigidbody)
            DynamicsWorld->addRigidBody(rigidbody);
        else DynamicsWorld->addCollisionObject(collisionObject.GetBtCollisionObject());
    }
    void Physics::RemoveCollisionObject(CollisionObject& collisionObject)
    {
        if (CollisionList.Exists(&collisionObject))
        {
            CollisionList.Remove(&collisionObject);
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

        DebugDrawInstance = unique_ptr<PhysicsDebugDraw>(new PhysicsDebugDraw());
        DynamicsWorld->setDebugDrawer(DebugDrawInstance.get());

        CollisionList = List<CollisionObject*>();
    }
    Physics::~Physics()
    {
        for (auto collider : CollisionList)
        {
            DynamicsWorld->removeCollisionObject(collider->GetBtCollisionObject());
        }
        CollisionList.clear();

        DynamicsWorld.reset();
        Solver.reset();
        OverlappingPairCache.reset();
        Dispatcher.reset();
        CollisionConfiguration.reset();
    }
}