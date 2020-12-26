#pragma once

#include "Behaviour.h"

#include "Material.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Mesh;
    class Renderer;

    // ��Ⱦ��
    struct RenderItem
    {
    public:
        // ʹ�õ���Ⱦ��
        Renderer* renderer;
        // ʹ�õĲ���
        Material* material;
        // ��������루��Ⱦ͸��ģ��ʱʹ�ã�
        float depth;
        // ʹ����ɫ������
        unsigned int index;
        // ��Ⱦ����
        unsigned int sequence;

        RenderItem(Renderer* r, Material* m, unsigned int i, unsigned int s) :renderer(r), material(m), depth(), index(i), sequence(s) {}
    };

    // ��Ⱦ�������࣬������һ�����ʣ�
    class Renderer : public Behaviour
    {
    public:
        // ��ȡ����
        inline Material* GetMaterial() { return _material; }
        unique_ptr<List<unique_ptr<RenderItem>>> GetRenderItems();
        // ����
        // @index ����ָ����ɫ��
        virtual void Draw(unsigned int index) = 0;
        // ʹ��ָ�����ʵ�ָ����ɫ������
        virtual void Draw(Material* material, unsigned int index) = 0;

    protected:
        Material* _material;

        Renderer(GameObject& obj) :Behaviour(obj), _material() {}

    private:
    };

    // ������Ⱦ����������һ�Բ��ʺ�ģ�ͣ�
    class MeshRenderer final : public Renderer
    {
    public:
        // ����һ��ģ�������
        void SetData(Mesh& mesh, Material& material);
        inline Mesh* GetMesh() { return _mesh; }

        // ����ģ��
        void Draw(unsigned int index) override;
        void Draw(Material* material, unsigned int index) override;

        MeshRenderer(GameObject& obj) :Renderer(obj), _mesh() {}

    private:
        Mesh* _mesh;
    };
}

