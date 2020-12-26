#include "Scene.h"

#include "GameSystem.h"
#include "UniformManager.h"

#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include "ScriptBehaviour.h"

namespace OpenGL_Learn
{
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

    void Scene::ExcuteUpdate()
    {
        GetDestroyedObjects().clear();

        // ȡ�����нű�����
        auto scripts = FindComponents<ScriptBehaviour>(true);
        // Awake ��������ʱ��ӵĽű�����
        for (auto script : scripts)
            if (!script->GetAwaked())
            {
                script->SetAwaked();
                script->Awake();
            }


        // ȡ�������Ѷ�����ѿ����ű����
        auto activeObjScript = FindComponents<ScriptBehaviour>();
        auto activeScripts = List<ScriptBehaviour*>();
        for (auto script : activeObjScript)
        {
            if (script->Enabled)
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


        // ȡ�������ѿ��������������
        auto cameras = this->FindComponents<Camera>();
        // ȡ�õ�һ���ѿ��������ƽ�й����
        auto dirLight = this->FindComponent<DirectionalLight>();
        auto pointLights = this->FindComponents<PointLight>();

        // ȡ�������ѿ����������Ⱦ���
        auto renderers = this->FindComponents<Renderer>();
        // ����Ⱦ˳������  
        List<unique_ptr<RenderItem>> backgrounds = List<unique_ptr<RenderItem>>();
        List<unique_ptr<RenderItem>> geometrys = List<unique_ptr<RenderItem>>();
        List<unique_ptr<RenderItem>> alphaTests = List<unique_ptr<RenderItem>>();
        List<unique_ptr<RenderItem>> transparents = List<unique_ptr<RenderItem>>();
        List<unique_ptr<RenderItem>> overlays = List<unique_ptr<RenderItem>>();
        for (auto renderer : renderers)
        {
            if (!renderer->Enabled) continue; // �����δ����������
            auto items = renderer->GetRenderItems();
            for (auto& item : *items)
            {
                // 0~1000 Background
                if (item->sequence < 1001) backgrounds.push_back(move(item));
                // 1001~2332 Geometry
                else if (1001 <= item->sequence && item->sequence < 2333) geometrys.push_back(move(item));
                // 2334~2999 AlphaTest
                else if (2334 <= item->sequence && item->sequence < 3000) alphaTests.push_back(move(item));
                // 3000~3999 Transparent
                else if (3000 <= item->sequence && item->sequence < 4000) transparents.push_back(move(item));
                // 4000+ Overlay
                else if (4000 <= item->sequence) overlays.push_back(move(item));
            }
        }
        backgrounds.Sort([](const unique_ptr<RenderItem>& left, const unique_ptr<RenderItem>& right) {return left->sequence < right->sequence; });
        geometrys.Sort([](const unique_ptr<RenderItem>& left, const unique_ptr<RenderItem>& right) {return left->sequence < right->sequence; });
        alphaTests.Sort([](const unique_ptr<RenderItem>& left, const unique_ptr<RenderItem>& right) {return left->sequence < right->sequence; });
        overlays.Sort([](const unique_ptr<RenderItem>& left, const unique_ptr<RenderItem>& right) {return left->sequence < right->sequence; });


        // ������������
        cameras.Sort([](const Camera* left, const Camera* right) { return left->Depth < right->Depth; });
        auto renderCount = 0;
        for (auto camera : cameras)
        {
            if (!camera->Enabled) continue;


            // ��ȡ����������нű����
            auto cameraScripts = camera->GetComponents<ScriptBehaviour>();

            // OnPreCull �޳�ǰ����
            for (auto script : cameraScripts)
                script->OnPreCull();

            // �����������
            UniformManager::Transform->SetSubData(0, sizeof(Matrix4x4), camera->GetProjectionMatrix().GetPtr()); // ���ͶӰ����
            UniformManager::Transform->SetSubData(sizeof(Matrix4x4), sizeof(Matrix4x4), camera->GetViewMatrix().GetPtr()); // ����ӽ�����
            UniformManager::CameraData->SetSubData(0 * sizeof(float), sizeof(Vector4), Vector4(camera->GetTransform().GetPosition(), 1.0f).GetPtr()); // �������λ��
            UniformManager::CameraData->SetSubData(4 * sizeof(float), sizeof(Vector4), camera->GetOrthoParams().GetPtr()); // �����������
            UniformManager::CameraData->SetSubData(8 * sizeof(float), sizeof(Vector4),
                Vector4(GameSystem::ScreenWidth, GameSystem::ScreenHeight, 1.0f / GameSystem::ScreenWidth, 1.0f / GameSystem::ScreenHeight).GetPtr()); // ��Ļ����

#pragma message("ʵ�����������޳�")

            // OnPerRender
            for (auto script : cameraScripts)
                script->OnPreRender();

            if (dirLight && dirLight->Enabled)
            {
                // ���͹�������
                UniformManager::LightingData->SetSubData(0 * sizeof(float), sizeof(Vector4),
                    (static_cast<Vector4>(dirLight->LightColor) * dirLight->Intensity).GetPtr());
                UniformManager::LightingData->SetSubData(4 * sizeof(float), sizeof(Vector4),
                    Vector4(dirLight->GetTransform().GetForward(), 0.0f).GetPtr());
                UniformManager::LightingData->SetSubData(8 * sizeof(float), sizeof(Matrix4x4),
                    dirLight->GetLightSpaceMatrix(camera).GetPtr());
                UniformManager::LightingData->SetSubData(24 * sizeof(float), sizeof(float),
                    &dirLight->ShadowStrength);

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
            camera->BindTarget(); // ��Ⱦ��Ŀ����Ⱦ��ͼ
            camera->ExcuteRender(&backgrounds, &geometrys, &alphaTests, &transparents, &overlays);

            // OnPostRender
            for (auto script : cameraScripts)
                script->OnPostRender();

            // ���Ƕ������Ⱦ��ͼ�������ݿ�����Ĭ����Ⱦ��ͼ
            camera->RenderToTargetTexture();
            // ������������ݽ������� Ĭ����Ⱦ��ͼ �� ָ��Ŀ����Ⱦ��ͼ ��
        }
        if (renderCount == 0) Camera::DefaultClear();

        // OnRenderImage
        for (auto script : activeScripts)
            script->OnRenderImage(Camera::DefaultTargetTexture);

        // ������ʾ��Ⱦ��ͼ
        if (dirLight)
            Camera::DebugRenderTexture(*dirLight->ShadowMap,
                0.75f, 0.5f,
                dirLight->ShadowMap->GetTexture(0)->GetWidth() * 0.15f / GameSystem::ScreenWidth,
                dirLight->ShadowMap->GetTexture(0)->GetHeight() * 0.15f / GameSystem::ScreenHeight
                //0, 0, 1, 1
            );

        Camera::DefaultTargetTexture->BindFramebuffer();
        // OnGui
        for (auto script : activeScripts)
            script->OnGui();

        // ��Ⱦ������
        Camera::RenderToWindow();

    }

    void Scene::BindMainObject()
    {
        Camera::Main = FindComponent<Camera>();
    }
}