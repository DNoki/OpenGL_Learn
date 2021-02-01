#include "pch.h"

#include "PointLightScene.h"

#include "PostProcess.h"
#include "CameraController.h"
#include "ConsoleBar.h"

void OpenGL_Learn::PointLightScene::ImportResource()
{
    auto&& consoleBar = AddGameObject(make_unique<GameObject>("Console Bar"));
    consoleBar.AddComponent<ConsoleBar>();


    auto unlitColor = AddResourceObject(make_unique<Shader>("Unlit_Color", "../Asset/Shader/Unlit/Color.glsl"));
    auto unlitColorMaterial = AddResourceObject(make_unique<Material>("Unlit_Color Material", unlitColor));
    auto litPhongShader = AddResourceObject(make_unique<Shader>("LitPhongShader", "../Asset/Shader/Lit/Phong.glsl"));
    auto litPhongMaterial = AddResourceObject(make_unique<Material>("LitPhongMaterial", litPhongShader));

    auto worldPositionShader = AddResourceObject(make_unique<Shader>("worldPositionShader", "../Asset/Shader/DiplayWorldPosition.glsl"));
    auto worldPositionMaterial = AddResourceObject(make_unique<Material>("worldPositionMaterial", worldPositionShader));

    auto meshTriangle = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::TRIANGLE));
    auto meshSquare = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::SQUARE));
    auto meshBox = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::BOX));

    auto& cameraObj = AddGameObject(make_unique<GameObject>("Camera Object"));
    cameraObj.AddComponent<CameraController>();
    {
        auto& camera = cameraObj.AddComponent<Camera>();
        //camera.ProjectionMode = ORTHOGRAPHIC;
        camera.State.BackgroundColor = Color(0.3f, 0.5f, 0.4f);
        //camera.BackgroundColor = Color::Green();
        camera.ClearFlags = CameraClearFlags::SKYBOX;
        camera.IsMSAA = true;

        // 后处理
        auto ColorBalance = AddResourceObject(make_unique<Shader>("ColorBalance Shader", "../Asset/Shader/Post/ColorBalance.glsl"));
        ColorBalance->State.DepthTest = false;

        auto& postProcess = cameraObj.AddComponent<PostProcess>();
        postProcess.AddEffect(*AddResourceObject(
            make_unique<Material>("ColorBalance Material", ColorBalance)));

        {
            auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light1"));
            auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
            plightRenderer.SetEnable(false);
            plightRenderer.SetData(*meshBox, *unlitColorMaterial);
            plightObj.GetTransform().SetPosition(Vector3(0, 0, 0));
            plightObj.GetTransform().LocalScale = Vector3::One * 0.1f;
            auto& pointLight = plightObj.AddComponent<PointLight>();
            pointLight.LightColor = Color::White();
            pointLight.ShadowStrength = 1.0f;
        }
        //{
        //    auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light1"));
        //    auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
        //    plightRenderer.Enabled = false;
        //    plightRenderer.SetData(*meshBox, *unlitColorMaterial);
        //    plightObj.GetTransform().SetPosition(Vector3(2, 1, 0));
        //    plightObj.GetTransform().LocalScale = Vector3::One * 0.1f;
        //    auto& pointLight = plightObj.AddComponent<PointLight>();
        //    pointLight.LightColor = Color::Magenta();
        //}
        //{
        //    auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light2"));
        //    auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
        //    plightRenderer.Enabled = false;
        //    plightRenderer.SetData(*meshBox, *unlitColorMaterial);
        //    plightObj.GetTransform().SetPosition(Vector3(-2, 1, 0));
        //    plightObj.GetTransform().LocalScale = Vector3::One * 0.15f;
        //    auto& pointLight = plightObj.AddComponent<PointLight>();
        //    pointLight.LightColor = Color::Yellow();
        //}
        //{
        //    auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light3"));
        //    auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
        //    plightRenderer.Enabled = false;
        //    plightRenderer.SetData(*meshBox, *unlitColorMaterial);
        //    plightObj.GetTransform().SetPosition(Vector3(0, 1, 2));
        //    plightObj.GetTransform().LocalScale = Vector3::One * 0.2f;
        //    auto& pointLight = plightObj.AddComponent<PointLight>();
        //    pointLight.LightColor = Color::Cyan();
        //}
        //{
        //    auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light4"));
        //    auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
        //    plightRenderer.Enabled = false;
        //    plightRenderer.SetData(*meshBox, *unlitColorMaterial);
        //    plightObj.GetTransform().SetPosition(Vector3(0, 1, -2));
        //    plightObj.GetTransform().LocalScale = Vector3::One * 0.25f;
        //    auto& pointLight = plightObj.AddComponent<PointLight>();
        //    pointLight.LightColor = Color::Red();
        //}

        auto& ground = AddGameObject(make_unique<GameObject>("Ground"));
        ground.AddComponent<MeshRenderer>().SetData(*meshBox, *worldPositionMaterial);
        //ground.GetTransform().LocalScale = Vector3::One * 200.0f;
        ground.GetTransform().SetPosition(Vector3::Forward * 10.0f, false);

        {
            auto importer = ModelImporter::ModelLoad("../Asset/Model/PointLightTestScene.fbx");
            for (auto& mat : importer->materials)
            {
                mat->AddShaderPass(litPhongShader);
            }
            importer->gameObjects[0]->Name = "PointLightTestScene";
            ModelImporter::AssingToScene(*this, *importer);
        }
    }
}
