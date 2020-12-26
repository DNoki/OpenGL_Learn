#pragma once

#include <memory>

#include "ScriptBehaviour.h"

namespace OpenGL_Learn
{

    // ����
    class PostProcess final : public ScriptBehaviour
    {
    public:
        RenderTexture* EffectRenderTexture = nullptr;

        // �����Ч
        // @material ����
        // @rt ��Ⱦ��ͼ
        void AddEffect(Material& material)
        {
            if (!EffectRenderTexture)
            {
                EffectRenderTexture = SceneManager::GetActiveScene().AddResourceObject(RenderTexture::CreateRenderTexture("Effect Render Texture",
                    GameSystem::ScreenWidth, GameSystem::ScreenHeight));
                EffectRenderTexture->AttachmentTexture2D(GL_RGBA16F, FormatType::RGBA);
                //EffectRenderTexture->HideFlag = HideFlagType::STATIC;
            }
            _materials.push_back(Tuple2<bool, Material*>(true, &material));
            for (unsigned int i = 0; i < material.GetPassCount(); i++)
            {
                material[i]->BindTexture(*EffectRenderTexture->GetTexture(0), "_MainTexture", 0);
                cout << "LOG:: " << "����Ӻ���Ч����"
                    << " Material: " << material[i]->Name << endl;
            }
        }
        Tuple2<bool, Material*>* GetEffectMaterial(unsigned int index)
        {
            if (index < GetEffectCount())
                return &_materials[index];
            return nullptr;
        }

        // ��ȡ��Ч����
        inline unsigned int GetEffectCount() { return (unsigned int)_materials.size(); }

        void Awake() override
        {
            _camera = this->GetGameObject().GetComponent<Camera>();
        }
        void OnPreRender() override
        {
        }
        void OnPostRender() override
        {
            // ����ǰ�����Ⱦ��ɺ���ĺ�����Ч
            // ���������
            // 1. ��Ⱦ������ָ��Ŀ����Ⱦ��ͼ��
            // 2. ��Ⱦ������Ĭ�϶������Ⱦ��ͼ��
            // 3. ��Ⱦ������Ĭ����Ⱦ��ͼ��
            //for (unsigned int i = 0; i < _materials.size(); i++)
            //{
            //    // ������ѡȾ���������� �� �Ӷ���������м�������
            //    _camera->RenderToTargetTexture(EffectRenderTexture.get());
            //    // ����Ⱦ���
            //    _camera->BindTarget();
            //    // Ӧ����Ч���ʲ���Ⱦ�ı��ε�����
            //    for (unsigned int j = 0; j < _materials[i]->GetPassCount(); j++)
            //        Camera::DefaultTargetTextureMesh->DrawMesh(*_materials[i], j);
            //}
        }

        void OnRenderImage(RenderTexture* targetTex) override
        {
            // ÿ���ű����ᴦ��ĺ�����Ч
            for (unsigned int i = 0; i < _materials.size(); i++)
            {
                if (!_materials[i].Item1) continue;
                targetTex->Blit(*EffectRenderTexture);// ��Ĭ����Ⱦ��ͼ���ݿ�����Ч����Ⱦ��ͼ��
                targetTex->BindFramebuffer();// ��Ⱦ��Ĭ����Ⱦ��ͼ��
                for (unsigned int j = 0; j < _materials[i].Item2->GetPassCount(); j++)
                    Camera::DefaultTargetTextureMesh->DrawMesh(*_materials[i].Item2, j);
            }
        }

        PostProcess(GameObject& obj) :ScriptBehaviour(obj), _camera(), _materials() {}

        Material* operator[](unsigned int i) { return _materials[i].Item2; }

    private:
        Camera* _camera;

        List<Tuple2<bool, Material*>> _materials;
    };
}