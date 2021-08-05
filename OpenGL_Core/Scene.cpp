#include "pch.h"

#include "Scene.h"

#include "Graphics.h"
#include "GameSystem.h"
#include "UniformManager.h"

#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include "ScriptBehaviour.h"

namespace OpenGL_Core
{
    List<GameObject*> Scene::GetAllRootGameObjects(bool isUnActiveInclude)
    {
        auto objs = List<GameObject*>();
        for (auto& obj : _hierarchy)
        {
            if (!isUnActiveInclude && !obj->GetActive()) continue;
            if (obj->IsRootObject())
                objs.push_back(obj.get());
        }
        return objs;
    }
    GameObject& Scene::AddGameObject(unique_ptr<GameObject>&& gameObj)
    {
        return AddGameObject(gameObj);
    }
    GameObject& Scene::AddGameObject(unique_ptr<GameObject>& gameObj)
    {
        auto result = gameObj.get();
        _hierarchy.push_back(move(gameObj));
        return *result;
    }
    GameObject* Scene::FindGameObject(const string& name) const
    {
        for (auto& obj : _hierarchy)
        {
            if (!obj->IsRootObject() || !obj->GetActive()) continue;
            if (obj->Name == name)
                return obj.get();
        }
        return nullptr;
    }

    void Scene::Initialize()
    {
        _physics = make_unique<Physics>();
        ImportResource();
    }

    void Scene::ExcuteUpdate()
    {
        GetDestroyedObjects().clear();

        // 取得所有脚本对象
        auto scripts = FindComponents<ScriptBehaviour>(true);

        // 取得所有已对象的已开启脚本组件
        auto activeObjScript = FindComponents<ScriptBehaviour>();
        auto activeScripts = List<ScriptBehaviour*>();
        for (auto script : activeObjScript)
        {
            if (script->GetEnable())
                activeScripts.push_back(script);
        }

        // Start
        for (auto script : activeScripts)
            if (!script->GetStarted())
            {
                script->SetStarted();
                script->Start();
            }
        // Update
        for (auto script : activeScripts)
            script->Update();
        // LateUpdate
        for (auto script : activeScripts)
            script->LateUpdate();

        // 更新物理
        _physics->ExcuteUpdate();

        // 取得所有已开启对象的相机组件
        auto cameras = this->FindComponents<Camera>();
        // 取得第一个已开启对象的平行光组件
        auto dirLight = this->FindComponent<DirectionalLight>();
        auto pointLights = this->FindComponents<PointLight>();

        // 取得所有已开启对象的渲染组件
        auto renderers = this->FindComponents<Renderer>();
        // 按渲染顺序排序
        List<RenderItem*> backgrounds = List<RenderItem*>();
        List<RenderItem*> geometrys = List<RenderItem*>();
        List<RenderItem*> alphaTests = List<RenderItem*>();
        List<RenderItem*> transparents = List<RenderItem*>();
        List<RenderItem*> overlays = List<RenderItem*>();
        for (auto renderer : renderers)
        {
            if (!renderer->GetEnable()) continue; // 若组件未开启则跳过
            auto& items = renderer->GetRenderItems();
            for (auto& item : items)
            {
                // 0~1000 Background
                if (item->sequence < 1001) backgrounds.push_back(item.get());
                // 1001~2332 Geometry
                else if (1001 <= item->sequence && item->sequence < 2333) geometrys.push_back(item.get());
                // 2334~2999 AlphaTest
                else if (2334 <= item->sequence && item->sequence < 3000) alphaTests.push_back(item.get());
                // 3000~3999 Transparent
                else if (3000 <= item->sequence && item->sequence < 4000) transparents.push_back(item.get());
                // 4000+ Overlay
                else if (4000 <= item->sequence) overlays.push_back(item.get());
            }
        }
        backgrounds.Sort([](const RenderItem* left, const RenderItem* right) {return left->sequence < right->sequence; });
        geometrys.Sort([](const RenderItem* left, const RenderItem* right) {return left->sequence < right->sequence; });
        alphaTests.Sort([](const RenderItem* left, const RenderItem* right) {return left->sequence < right->sequence; });
        overlays.Sort([](const RenderItem* left, const RenderItem* right) {return left->sequence < right->sequence; });


        // 按相机深度排序
        if (cameras.size() > 0)
        {
            cameras.Sort([](const Camera* left, const Camera* right) { return left->Depth < right->Depth; });
            auto renderCount = 0;
            for (auto camera : cameras)
            {
                if (!camera->GetEnable()) continue;


                // 获取该相机的所有脚本组件
                auto cameraScripts = camera->GetComponents<ScriptBehaviour>();

                // OnPreCull 剔除前调用
                for (auto script : cameraScripts)
                    script->OnPreCull();

                // 传送相机数据
                UniformManager::Transform->SetSubData(0, sizeof(Matrix4x4), camera->GetProjectionMatrix().GetPtr()); // 相机投影数据
                UniformManager::Transform->SetSubData(sizeof(Matrix4x4), sizeof(Matrix4x4), camera->GetViewMatrix().GetPtr()); // 相机视角数据
                UniformManager::CameraData->SetSubData(0 * sizeof(float), sizeof(Vector4), Vector4(camera->GetTransform().GetPosition(), 1.0f).GetPtr()); // 相机世界位置
                UniformManager::CameraData->SetSubData(4 * sizeof(float), sizeof(Vector4), camera->GetOrthoParams().GetPtr()); // 相机正交数据
                UniformManager::CameraData->SetSubData(8 * sizeof(float), sizeof(Vector4),
                    Vector4(GameSystem::ScreenWidth, GameSystem::ScreenHeight, 1.0f / GameSystem::ScreenWidth, 1.0f / GameSystem::ScreenHeight).GetPtr()); // 屏幕数据

#pragma message("实现相机外对象剔除")

            // OnPerRender
                for (auto script : cameraScripts)
                    script->OnPreRender();

                if (dirLight && dirLight->GetEnable())
                {
                    // 传送光照数据
                    UniformManager::LightingData->SetSubData(0 * sizeof(float), sizeof(Vector4),
                        (static_cast<Vector4>(dirLight->LightColor) * dirLight->Intensity).GetPtr());
                    UniformManager::LightingData->SetSubData(4 * sizeof(float), sizeof(Vector4),
                        Vector4(dirLight->GetTransform().GetForward(), 0.0f).GetPtr());
                    UniformManager::LightingData->SetSubData(8 * sizeof(float), sizeof(Matrix4x4),
                        dirLight->GetLightSpaceMatrix(camera).GetPtr());
                    UniformManager::LightingData->SetSubData(24 * sizeof(float), sizeof(float),
                        &dirLight->ShadowStrength);
                    UniformManager::LightingData->SetSubData(25 * sizeof(float), sizeof(float),
                        &dirLight->Bias);

                    dirLight->RenderShadowMap(nullptr, &geometrys, nullptr, nullptr, nullptr);
                }
                else
                {
                    UniformManager::LightingData->SetSubData(0 * sizeof(float), sizeof(Vector4), Vector4::Zero.GetPtr());
                    UniformManager::LightingData->SetSubData(24 * sizeof(float), sizeof(float), 0x00);
                }
                for (size_t i = 0; i < 4; i++)
                {
                    if (i < pointLights.Count())
                    {
                        UniformManager::LightingData->SetSubData((28 + i * 4) * sizeof(float), sizeof(Vector4),
                            Vector4(pointLights[i]->GetTransform().GetPosition(), 1.0f).GetPtr());
                        UniformManager::LightingData->SetSubData((44 + i * 4) * sizeof(float), sizeof(Vector4),
                            (static_cast<Vector4>(pointLights[i]->LightColor) * pointLights[i]->Intensity).GetPtr());
                        UniformManager::LightingData->SetSubData((60 + i * 4) * sizeof(float), sizeof(Vector4),
                            pointLights[i]->GetPointLightInfo().GetPtr());

                        pointLights[i]->RenderShadowMap(i, nullptr, &geometrys, nullptr, nullptr, nullptr);
                    }
                    else
                        UniformManager::LightingData->SetSubData((28 + i * 4) * sizeof(float), sizeof(Vector4),
                            Vector4::Zero.GetPtr());
                }

                renderCount++;
                camera->BindTarget(); // 渲染到目标渲染贴图
                camera->ExcuteRender(&backgrounds, &geometrys, &alphaTests, &transparents, &overlays);

                // OnPostRender
                for (auto script : cameraScripts)
                    script->OnPostRender();

                // 若是多采样渲染贴图，则将数据拷贝到默认渲染贴图
                camera->RenderToTargetTexture();
                // 相机结束后将内容将保存在 默认渲染贴图 或 指定目标渲染贴图 上
            }
            //if (renderCount == 0) Camera::DefaultClear();

            // OnRenderImage
            for (auto script : activeScripts)
                script->OnRenderImage(Camera::DefaultTargetTexture.get());

            // 调试显示渲染贴图
            if (dirLight)
                Camera::DebugRenderTexture(*dirLight->ShadowMap,
                    0.75f, 0.5f,
                    dirLight->ShadowMap->GetTexture(0)->GetWidth() * 0.15f / GameSystem::ScreenWidth,
                    dirLight->ShadowMap->GetTexture(0)->GetHeight() * 0.15f / GameSystem::ScreenHeight
                    //0, 0, 1, 1
                );

            Camera::DefaultTargetTexture->BindFramebuffer();

            // 物理调试显示需要优化
            {
                auto camera = cameras[0];
                camera->ClearDepth();
                UniformManager::Transform->SetSubData(0, sizeof(Matrix4x4), camera->GetProjectionMatrix().GetPtr()); // 相机投影数据
                UniformManager::Transform->SetSubData(sizeof(Matrix4x4), sizeof(Matrix4x4), camera->GetViewMatrix().GetPtr()); // 相机视角数据
                UniformManager::CameraData->SetSubData(0 * sizeof(float), sizeof(Vector4), Vector4(camera->GetTransform().GetPosition(), 1.0f).GetPtr()); // 相机世界位置
                UniformManager::CameraData->SetSubData(4 * sizeof(float), sizeof(Vector4), camera->GetOrthoParams().GetPtr()); // 相机正交数据
                UniformManager::CameraData->SetSubData(8 * sizeof(float), sizeof(Vector4),
                    Vector4(GameSystem::ScreenWidth, GameSystem::ScreenHeight, 1.0f / GameSystem::ScreenWidth, 1.0f / GameSystem::ScreenHeight).GetPtr()); // 屏幕数据
                _physics->DebugDraw();
            }

            // OnGui
            for (auto script : activeScripts)
                script->OnGui();

            // 渲染到窗口
            Camera::RenderToWindow();
        }
        else
        {
            auto state = RenderState();
            state.BackgroundColor = Color(0.0f, 0.0f, 0.0f);
            Camera::UseRenderState(state);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // OnGui
            for (auto script : activeScripts)
                script->OnGui();
        }
    }

    void Scene::Close()
    {
        for (auto& obj : _resourceObjects)
        {
            for (auto& r : obj.second)
                r.reset();
        }
        _resourceObjects.clear();

        for (auto& obj : _destroyedObjects)
            obj.reset();
        _destroyedObjects.clear();

        for (auto& obj : _hierarchy)
            obj.reset();
        _hierarchy.clear();

        _physics.reset();
    }

    Scene::Scene(const string& name) :Name(name), _hierarchy(), _resourceObjects(), _destroyedObjects(), _physics()
    {

    }

    Scene::~Scene()
    {

    }
}