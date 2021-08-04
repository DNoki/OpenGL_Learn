#include "pch.h"
#include "ToonShaderTestScene.h"

#include "PostProcess.h"
#include "CameraController.h"
#include "ConsoleBar.h"

namespace OpenGL_Learn
{
    void ToonShaderTestScene::ImportResource()
    {
        auto& cameraObj = AddGameObject(make_unique<GameObject>("Camera Object"));
        {
            auto& camera = cameraObj.AddComponent<Camera>();
            camera.State.BackgroundColor = Color(0.3f, 0.5f, 0.4f);
            camera.ClearFlags = CameraClearFlags::SKYBOX;
            camera.IsMSAA = true;

            auto ColorBalance = AddResourceObject(make_unique<Shader>("ColorBalance Shader", "../Asset/Shader/Post/ColorBalance.glsl"));
            ColorBalance->State.DepthTest = false;

            auto& postProcess = cameraObj.AddComponent<PostProcess>();
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("ColorBalance Material", ColorBalance)));
        }
        cameraObj.AddComponent<CameraController>();
        cameraObj.GetTransform().SetPosition(Vector3(0.0f, 2.2f, 2.0f));
        cameraObj.GetTransform().SetEulerAngles(Vector3(0.0f, 180.0f, 0.0f));

        auto simpleToon = AddResourceObject(make_unique<Shader>("SampleToon", "../Asset/Shader/Toon/SimpleToon.glsl"));
        auto outline = AddResourceObject(make_unique<Shader>("Outline", "../Asset/Shader/Toon/Outline.glsl"));

        // 灯光
        auto& lightObj = AddGameObject(make_unique<GameObject>("Directional Light"));
        //lightObj.SetActive(false);
        auto& dirLight = lightObj.AddComponent<DirectionalLight>();
        dirLight.GenerateShadowMap();
        dirLight.ShadowStrength = 0.0f;
        lightObj.GetTransform().SetPosition(Vector3(10, 10, 10));
        lightObj.GetTransform().LookAt(Vector3::Zero);

        {
            auto unityChanTex = AddResourceObject(Texture2D::CreateTexture2D("Unity Chan Tex", "../Asset/Model/Textures/utc_all2_light.png", true));
            auto unityChanNormalTex = AddResourceObject(Texture2D::CreateTexture2D("Unity Chan Normal Tex", "../Asset/Model/Textures/SD_Unitychan_normal.png", false));

            auto importer = ModelImporter::ModelLoad("../Asset/Model/UnityChan_Change.fbx");
            for (auto& mat : importer->materials)
            {
                auto unityChanShader = AddResourceObject(make_unique<Shader>(*simpleToon));
                auto outlineShader = AddResourceObject(make_unique<Shader>(*outline));
                unityChanShader->BindTexture(*unityChanTex, "_MainTexture");
                unityChanShader->BindTexture(*unityChanNormalTex, "_NormalTexture");
                //mat->AddShaderPass(unityChanShader);
                //mat->AddShaderPass(displayNormalShader);
                mat->AddShaderPass(outlineShader);
            }

            // 临时功能，需要改进生成渲染项调用位置
            for (auto& obj : importer->gameObjects)
            {
                auto renderer = obj->GetComponent<Renderer>();
                if (renderer) renderer->GenerateRenderItems();
            }

            //importer->gameObjects[0]->GetTransform().Rotate(Vector3::Up, 180.0f);
            importer->gameObjects[0]->GetTransform().LocalScale = Vector3::One * 0.03f;
            importer->gameObjects[0]->Name = "Unity Chan";
            ModelImporter::AssingToScene(*this, *importer);
        }

        auto&& consoleBar = AddGameObject(make_unique<GameObject>("Console Bar"));
        consoleBar.AddComponent<ConsoleBar>();
    }
}