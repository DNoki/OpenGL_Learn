﻿#include "pch.h"

#include "PhysiceTestScene.h"

#include "CameraController.h"
#include "ConsoleBar.h"
#include "RigidbodyController.h"

namespace OpenGL_Learn
{
    void PhysiceTestScene::ImportResource()
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

        auto&& consoleBar = AddGameObject(make_unique<GameObject>("Console Bar"));
        consoleBar.AddComponent<ConsoleBar>();


        auto litBlinn = AddResourceObject(make_unique<Shader>("Lit_Blinn", "../Asset/Shader/Lit/Blinn.glsl"));
        auto blinnMaterial = AddResourceObject(make_unique<Material>("Blinn Material", litBlinn));

        auto meshSquare = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::SQUARE));
        auto meshBox = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::BOX));

        // 灯光
        auto& lightObj = AddGameObject(make_unique<GameObject>("Directional Light"));
        auto& light = lightObj.AddComponent<DirectionalLight>();
        light.GenerateShadowMap();
        light.Bias = 0.000001f;
        light.NormalBias = 0.02f;
        lightObj.GetTransform().SetPosition(Vector3(10, 10, 10));
        lightObj.GetTransform().LookAt(Vector3::Zero);

        auto& ground = AddGameObject(make_unique<GameObject>("Ground"));
        {
            auto shader = AddResourceObject(make_unique<Shader>("Lit_Blinn", "../Asset/Shader/Lit/Blinn.glsl"));
            auto material = AddResourceObject(make_unique<Material>("Blinn Material", shader));
            material->GetMainShader()->SetVector4("_MainColor", Vector4(0.5f, 0.5f, 0.5f, 1.0f));

            ground.AddComponent<MeshRenderer>().SetData(*meshSquare, *material);
            auto& transform = ground.GetTransform();
            transform.LocalScale = Vector3(2000.0f, 2000.0f, 2000.0f);

            auto& boxCollider = ground.AddComponent<BoxCollider>();
            boxCollider.SetSize(Vector3(transform.LocalScale.x, 0.01f, transform.LocalScale.z));
            auto& rigid = ground.AddComponent<Rigidbody>();
            rigid.SetCollider(boxCollider);
            rigid.SetMass(0.0f);
        }

        //auto& box = AddGameObject(make_unique<GameObject>("box"));
        //{
        //    box.AddComponent<MeshRenderer>().SetData(*meshBox, *blinnMaterial);
        //    auto& transform = box.GetTransform();
        //    transform.LocalPosition = Vector3(0.0f, 50.0f, 0.0f);

        //    auto& boxCollider = box.AddComponent<BoxCollider>();
        //    auto& rigid = box.AddComponent<Rigidbody>();
        //    rigid.SetCollider(boxCollider);
        //    rigid.SetMass(1.0f);

        //    box.AddComponent<RigidbodyController>();
        //}

        for (size_t i = 0; i < 100; i++)
        {
            auto& box = AddGameObject(make_unique<GameObject>("box"));
            box.AddComponent<MeshRenderer>().SetData(*meshBox, *blinnMaterial);
            {
                auto& transform = box.GetTransform();
                //transform.LocalPosition = Vector3((i % 2) * 2.0f - 1.0f, 2.0f * i, 0.0f);
                transform.SetPosition(Vector3(sin(2.0f * Math::PI * i / 100.0f), 2.0f * i, 0.0f), false);

                auto& boxCollider = box.AddComponent<BoxCollider>();
                auto& rigid = box.AddComponent<Rigidbody>();
                rigid.SetCollider(boxCollider);
                rigid.SetMass(1.0f);
            }
        }
    }

}
