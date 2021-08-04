#include "pch.h"

#include "SampleScene.h"

#include "PostProcess.h"
#include "CameraController.h"
#include "ConsoleBar.h"
#include "FatherGameObject.h"
#include "ChildGameObject.h"
#include "GaussianBlur.h"

namespace OpenGL_Learn
{
    void SampleScene::ImportResource()
    {
        auto texture = AddResourceObject(Texture2D::CreateTexture2D("Checker", "../Asset/Checker.png", true));
        auto transparentTexture = AddResourceObject(Texture2D::CreateTexture2D("Transparent", "../Asset/Transparent.png", true));


        auto unlitColor = AddResourceObject(make_unique<Shader>("Unlit_Color", "../Asset/Shader/Unlit/Color.glsl"));
        //unlitColor.State.CullFace = false;

        auto unlitTexture = AddResourceObject(make_unique<Shader>("Unlit_Texture", "../Asset/Shader/Unlit/Texture.glsl"));
        //shader.State.DepthMask = true;
        //shader.State.Blend = true;
        //shader.State.CullFace = false;
        unlitTexture->BindTexture(*texture, "_MainTexture", 0);

        auto unlitTransparent = AddResourceObject(make_unique<Shader>("Unlit_Transparent", "../Asset/Shader/Unlit/Transparent.glsl"));
        unlitTransparent->State.DepthMask = false;
        unlitTransparent->State.Blend = true;
        unlitTransparent->State.CullFace = false;
        unlitTransparent->BindTexture(*transparentTexture, "_MainTexture", 0);

        auto litLambert = AddResourceObject(make_unique<Shader>("Lit_Lambert", "../Asset/Shader/Lit/Lambert.glsl"));

        auto litBlinn = AddResourceObject(make_unique<Shader>("Lit_Blinn", "../Asset/Shader/Lit/Blinn.glsl"));

        auto litPhong = AddResourceObject(make_unique<Shader>("Lit_Phong", "../Asset/Shader/Lit/Phong.glsl"));


        auto unlitColorMaterial = AddResourceObject(make_unique<Material>("Unlit_Color Material", unlitColor));
        auto unlitTexMaterial = AddResourceObject(make_unique<Material>("UnlitTexture Material", unlitTexture));
        auto transparentMaterial = AddResourceObject(make_unique<Material>("transparent Material", unlitTransparent));
        auto blinnMaterial = AddResourceObject(make_unique<Material>("Blinn Material", litBlinn));
        auto phongMaterial = AddResourceObject(make_unique<Material>("Phong Material", litPhong));


        auto meshTriangle = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::TRIANGLE));
        auto meshSquare = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::SQUARE));
        auto meshBox = AddResourceObject(Mesh::CreatePresetMesh(PresetMesh::BOX));


        // 天空盒
        Skybox* skybox = nullptr;
        {
            auto faces = List<string>(
                {
                    "../Asset/Skybox/right.jpg",
                    "../Asset/Skybox/left.jpg",
                    "../Asset/Skybox/top.jpg",
                    "../Asset/Skybox/bottom.jpg",
                    "../Asset/Skybox/front.jpg",
                    "../Asset/Skybox/back.jpg"
                });
            auto skyboxTexture = AddResourceObject(TextureCube::CreateTextureCube("Cube_SkyCube", faces, true,
                WrapType::CLAMP_TO_EDGE, ScaleFilterType::LINEAR, false));

            auto skyboxShader = AddResourceObject(make_unique<Shader>("Skybox Shader", "../Asset/Shader/Skybox.glsl"));
            skyboxShader->BindTexture(*skyboxTexture, "_CubeTexture", 0);
            skyboxShader->State.DepthTestMode = TestModeType::LEQUAL;
            skyboxShader->State.DepthMask = false;
            skyboxShader->State.CullFaceMode = CullFaceModeType::FRONT;
            auto skyboxMaterial = AddResourceObject(make_unique<Material>("Skybox Material", skyboxShader));
            skybox = AddResourceObject(make_unique<Skybox>("天空盒", skyboxMaterial));
        }

        // 相机
        auto& cameraObj = AddGameObject(make_unique<GameObject>("Camera Object"));
        cameraObj.AddComponent<CameraController>();
        cameraObj.AddComponent<GaussianBlur>().SetEnable(false);
        {
            auto& camera = cameraObj.AddComponent<Camera>();
            //camera.ProjectionMode = ORTHOGRAPHIC;
            camera.State.BackgroundColor = Color(0.3f, 0.5f, 0.4f);
            //camera.BackgroundColor = Color::Green();
            camera.SetSkybox(skybox);
            camera.ClearFlags = CameraClearFlags::SKYBOX;
            camera.IsMSAA = true;

            // 后处理
            auto blurShader = AddResourceObject(make_unique<Shader>("Blur Shader", "../Asset/Shader/Post/Blur.glsl"));
            blurShader->State.DepthTest = false;
            auto edgeDetectionShader = AddResourceObject(make_unique<Shader>("EdgeDetection Shader", "../Asset/Shader/Post/EdgeDetection.glsl"));
            edgeDetectionShader->State.DepthTest = false;
            auto grayscaleShader = AddResourceObject(make_unique<Shader>("Grayscale Shader", "../Asset/Shader/Post/Grayscale.glsl"));
            grayscaleShader->State.DepthTest = false;
            auto inversionShader = AddResourceObject(make_unique<Shader>("Inversion Shader", "../Asset/Shader/Post/Inversion.glsl"));
            inversionShader->State.DepthTest = false;
            auto kernelShader = AddResourceObject(make_unique<Shader>("Kernel Shader", "../Asset/Shader/Post/Kernel.glsl"));
            kernelShader->State.DepthTest = false;
            auto ColorBalance = AddResourceObject(make_unique<Shader>("ColorBalance Shader", "../Asset/Shader/Post/ColorBalance.glsl"));
            ColorBalance->State.DepthTest = false;

            auto& postProcess = cameraObj.AddComponent<PostProcess>();
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("Blur Material", blurShader)));
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("EdgeDetection Material", edgeDetectionShader)));
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("GrayscaleShader Material", grayscaleShader)));
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("Inversion Material", inversionShader)));
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("Kernel Material", kernelShader)));
            for (size_t i = 0; i < postProcess.GetEffectCount(); i++)
                auto effect = postProcess.GetEffectMaterial(i)->Item1 = false;
            postProcess.AddEffect(*AddResourceObject(
                make_unique<Material>("ColorBalance Material", ColorBalance)));
        }

        // 灯光
        auto& lightObj = AddGameObject(make_unique<GameObject>("Directional Light"));
        lightObj.SetActive(false);
        //lightObj._enabled = false;
        lightObj.AddComponent<DirectionalLight>().GenerateShadowMap();
        lightObj.GetTransform().SetPosition(Vector3(10, 10, 10));
        lightObj.GetTransform().LookAt(Vector3::Zero);

        {
            auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light1"));
            auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
            plightRenderer.SetEnable(false);
            plightRenderer.Initialize(*meshBox, *unlitColorMaterial);
            plightObj.GetTransform().SetPosition(Vector3(2, 1, 0));
            plightObj.GetTransform().LocalScale = Vector3::One * 0.1f;
            auto& pointLight = plightObj.AddComponent<PointLight>();
            pointLight.LightColor = Color::Cyan();
        }
        {
            auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light2"));
            auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
            plightRenderer.SetEnable(false);
            plightRenderer.Initialize(*meshBox, *unlitColorMaterial);
            plightObj.GetTransform().SetPosition(Vector3(-1, 1, 1.732f));
            plightObj.GetTransform().LocalScale = Vector3::One * 0.15f;
            auto& pointLight = plightObj.AddComponent<PointLight>();
            pointLight.LightColor = Color::Magenta();
        }
        {
            auto& plightObj = AddGameObject(make_unique<GameObject>("Point Light3"));
            auto& plightRenderer = plightObj.AddComponent<MeshRenderer>();
            plightRenderer.SetEnable(false);
            plightRenderer.Initialize(*meshBox, *unlitColorMaterial);
            plightObj.GetTransform().SetPosition(Vector3(-1.732f, 1, -1));
            plightObj.GetTransform().LocalScale = Vector3::One * 0.2f;
            auto& pointLight = plightObj.AddComponent<PointLight>();
            pointLight.LightColor = Color::Yellow();
        }

        // 游戏对象
        //auto& fatherObj = AddGameObject(make_unique<GameObject>("Father"));
        //fatherObj.AddComponent<MeshRenderer>().Initialize(*meshBox, *unlitTexMaterial);
        //fatherObj.AddComponent<FatherGameObject>();
        //fatherObj.GetTransform().LocalScale = Vector3::One * 0.2f + Vector3::Forward * 0.2f;

        //auto& childObj = AddGameObject(make_unique<GameObject>("Child"));
        //childObj.AddComponent<MeshRenderer>().Initialize(meshBox, unlitColorMaterial);
        //childObj.AddComponent<ChildGameObject>().fatherObj = &fatherObj;

        //auto displayNormalShader = AddResourceObject(make_unique<Shader>("Display Normal Shader", "../Asset/Shader/Auxiliary/DisplayNormal.glsl"));
        //displayNormalShader->SetFloat("_Magnitude", 0.05f);
        //auto displayNormalShaderMaterial = AddResourceObject(make_unique<Material>("displayNormalShader Material", displayNormalShader));

        //auto outlineShader = AddResourceObject(make_unique<Shader>("Display Normal Shader", "../Asset/Shader/Auxiliary/Outline.glsl"));
        //auto displayNormalShaderMaterial = AddResourceObject(make_unique<Material>("outlineShader Material", outlineShader));

        auto& check = AddGameObject(make_unique<GameObject>("Check Box"));
        check.AddComponent<MeshRenderer>().Initialize(*meshBox, *unlitTexMaterial);
        check.GetTransform().SetPosition(Vector3(4.0f, 0.5f, 0.0f));
        //check.GetTransform().SetParent(&cameraObj.GetTransform(), false);

        auto& ground = AddGameObject(make_unique<GameObject>("Ground"));
        ground.AddComponent<MeshRenderer>().Initialize(*meshSquare, *phongMaterial);
        ground.GetTransform().LocalScale = Vector3::One * 200.0f;

        //auto& transparentObj = AddGameObject(make_unique<GameObject>("TransparentObject"));
        //transparentObj.AddComponent<MeshRenderer>().Initialize(*meshBox, *transparentMaterial);
        //transparentObj.GetTransform().SetPosition(Vector3(0.5f, 0, 0), false);

        {
            auto unityChanTex = AddResourceObject(Texture2D::CreateTexture2D("Unity Chan Tex", "../Asset/Model/Textures/utc_all2_light.png", true));
            auto unityChanNormalTex = AddResourceObject(Texture2D::CreateTexture2D("Unity Chan Normal Tex", "../Asset/Model/Textures/SD_Unitychan_normal.png", false));

            auto importer = ModelImporter::ModelLoad("../Asset/Model/UnityChan.fbx");
            for (auto& mat : importer->materials)
            {
                auto unityChanShader = AddResourceObject(make_unique<Shader>(*litPhong));
                unityChanShader->BindTexture(*unityChanTex, "_MainTexture");
                unityChanShader->BindTexture(*unityChanNormalTex, "_NormalTexture");
                mat->AddShaderPass(unityChanShader);
                //mat->AddShaderPass(displayNormalShader);
                //mat->AddShaderPass(outlineShader);
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
        //{
        //    auto importer = ModelImporter::ModelLoad("../Asset/Model/DirectionScene.fbx");
        //    for (auto& mat : importer->materials)
        //    {
        //        auto phoneShader = AddResourceObject(make_unique<Shader>(*litPhong));
        //        mat->AddShaderPass(phoneShader);
        //    }
        //    importer->gameObjects[0]->Name = "Direction Scene";
        //    AssimpImporter::AssingToScene(*this, *importer);
        //}

        //auto normalTex = SceneManager::GetActiveScene()->AddResourceObject(unique_ptr<Texture2D>(Texture2D::CreateTexture2D(
        //    "Normal Tex",
        //    "F:/LocalCGLearn/OpenGL/OpenGL_LEARN/Asset/Model/Textures/SD_Unitychan_normal.png",
        //    false)));
        //auto test = SceneManager::GetActiveScene()->AddResourceObject(unique_ptr<Shader>(new Shader("Test Normal", "../Asset/Shader/TestNormal.glsl")));
        //test->SetVector4("_MainColor", Color::Blue());
        //test->BindTexture(*normalTex, "_MainTexture", 0);

        //auto sphereImporter = ModelImporter::ModelLoad("../Asset/Model/Sphere.fbx", displayNormalShader);
        ////sphereImporter->gameObjects[0]->GetTransform().SetPosition(Vector3::Up);
        ////sphereImporter->gameObjects[0]->GetTransform().Rotate(Vector3::Right, 90);
        //AssimpImporter::AssingToScene(*this, *sphereImporter);

        auto&& consoleBar = AddGameObject(make_unique<GameObject>("Console Bar"));
        consoleBar.AddComponent<ConsoleBar>();
    }
}