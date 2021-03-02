#include "pch.h"
#include "RayMarching.h"

#include "PostProcess.h"
#include "CameraController.h"
#include "ConsoleBar.h"
#include "FatherGameObject.h"
#include "ChildGameObject.h"
#include "GaussianBlur.h"

namespace OpenGL_Learn
{
    void RayMarching::ImportResource()
    {
        auto& cameraObj = AddGameObject(make_unique<GameObject>("Camera Object"));
        {
            auto& camera = cameraObj.AddComponent<Camera>();
            //camera.ProjectionMode = ORTHOGRAPHIC;
            camera.State.BackgroundColor = Color(0.3f, 0.5f, 0.4f);
            //camera.BackgroundColor = Color::Green();
            //camera.SetSkybox(skybox);
            camera.ClearFlags = CameraClearFlags::SKYBOX;
            camera.IsMSAA = true;
        }
        cameraObj.AddComponent<CameraController>();
        cameraObj.GetTransform().SetPosition(Vector3(0.0f, 5.0f, 25.0f));
        cameraObj.GetTransform().LookAt(Vector3::Zero);

        //Physics::SetDebugEnable(true);


        auto litPhong = AddResourceObject(make_unique<Shader>("Lit_Phong", "../Asset/Shader/Lit/Phong.glsl"));
        auto phongMaterial = AddResourceObject(make_unique<Material>("Phong Material", litPhong));

        auto meshSquare = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::SQUARE));
        auto meshBox = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::BOX));
        auto meshSphere = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::SPHERE));

        // 灯光
        auto& lightObj = AddGameObject(make_unique<GameObject>("Directional Light"));
        //lightObj.SetActive(false);
        lightObj.AddComponent<DirectionalLight>().GenerateShadowMap();
        lightObj.GetTransform().SetPosition(Vector3(10, 10, 10));
        lightObj.GetTransform().LookAt(Vector3::Zero);


        auto& ground = AddGameObject(make_unique<GameObject>("Ground"));
        {
            auto shader = AddResourceObject(make_unique<Shader>("Lit_Blinn", "../Asset/Shader/Lit/Blinn.glsl"));
            auto material = AddResourceObject(make_unique<Material>("Blinn Material", shader));
            material->GetMainShader()->SetVector4("_MainColor", Vector4(0.5f, 0.5f, 0.5f, 1.0f));

            ground.AddComponent<MeshRenderer>().Initialize(*meshSquare, *material);
            auto& transform = ground.GetTransform();
            transform.LocalScale = Vector3(2000.0f, 0.01f, 2000.0f);

            auto& boxCollider = ground.AddComponent<BoxCollider>();
            boxCollider.Initialize(Vector3(transform.LocalScale.x, 0.01f, transform.LocalScale.z), Vector3::Zero);
            auto& collision = ground.AddComponent<CollisionObject>();
            collision.Initialize(boxCollider);
        }

        for (size_t i = 0; i < 4; i++)
        {
            auto& wall = AddGameObject(make_unique<GameObject>("wall" + i));
            {
                auto& transform = wall.GetTransform();
                auto distance = 3.0f;
                switch (i)
                {
                case 0:
                    transform.LocalScale = Vector3(10.0f, 10.0f, 1.0f);
                    transform.SetPosition(Vector3(0.0f, 5.0f, distance));
                    break;
                case 1:
                    transform.LocalScale = Vector3(10.0f, 10.0f, 1.0f);
                    transform.SetPosition(Vector3(0.0f, 5.0f, -distance));
                    break;
                case 2:
                    transform.LocalScale = Vector3(1.0f, 10.0f, 10.0f);
                    transform.SetPosition(Vector3(distance, 5.0f, 0.0f));
                    break;
                case 3:
                    transform.LocalScale = Vector3(1.0f, 10.0f, 10.0f);
                    transform.SetPosition(Vector3(-distance, 5.0f, 0.0f));
                    break;
                }

                auto& boxCollider = wall.AddComponent<BoxCollider>();
                boxCollider.Initialize(transform.LocalScale, Vector3::Zero);
                auto& collision = wall.AddComponent<CollisionObject>();
                collision.Initialize(boxCollider);
            }
        }

        for (size_t i = 0; i < 200; i++)
        {
            auto& sphere = AddGameObject(make_unique<GameObject>("sphere"));
            //sphere.AddComponent<MeshRenderer>().Initialize(*meshSphere, *phongMaterial);

            auto& transform = sphere.GetTransform();
            //transform.LocalPosition = Vector3((i % 2) * 2.0f - 1.0f, 2.0f * i, 0.0f);
            //transform.SetPosition(Vector3(sin(2.0f * Math::PI * i / 100.0f), 2.0f * i, 0.0f), false);
            transform.SetPosition(Vector3(sin(2.0f * Math::PI * i / 10.0f), 3.5f * i + 5.0f, 0.0f), false);

            auto& sphereCollider = sphere.AddComponent<SphereCollider>();
            sphereCollider.Initialize(0.5f, Vector3::Zero);

            auto& rigid = sphere.AddComponent<Rigidbody>();
            rigid.Initialize(sphereCollider, 1.0f);
            rigid.SetIsContinuousDynamic(false);
            rigid.SetFriction(1.5f);
        }


        auto&& consoleBar = AddGameObject(make_unique<GameObject>("Console Bar"));
        consoleBar.AddComponent<ConsoleBar>();
    }
}