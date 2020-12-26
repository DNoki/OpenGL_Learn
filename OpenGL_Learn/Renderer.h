#pragma once

#include "Behaviour.h"

#include "Material.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Mesh;
    class Renderer;

    // 渲染项
    struct RenderItem
    {
    public:
        // 使用的渲染器
        Renderer* renderer;
        // 使用的材质
        Material* material;
        // 到相机距离（渲染透明模型时使用）
        float depth;
        // 使用着色器索引
        unsigned int index;
        // 渲染队列
        unsigned int sequence;

        RenderItem(Renderer* r, Material* m, unsigned int i, unsigned int s) :renderer(r), material(m), depth(), index(i), sequence(s) {}
    };

    // 渲染器（虚类，包含了一个材质）
    class Renderer : public Behaviour
    {
    public:
        // 获取材质
        inline Material* GetMaterial() { return _material; }
        unique_ptr<List<unique_ptr<RenderItem>>> GetRenderItems();
        // 绘制
        // @index 绘制指定着色器
        virtual void Draw(unsigned int index) = 0;
        // 使用指定材质的指定着色器绘制
        virtual void Draw(Material* material, unsigned int index) = 0;

    protected:
        Material* _material;

        Renderer(GameObject& obj) :Behaviour(obj), _material() {}

    private:
    };

    // 网格渲染器（包含了一对材质和模型）
    class MeshRenderer final : public Renderer
    {
    public:
        // 设置一对模型与材质
        void SetData(Mesh& mesh, Material& material);
        inline Mesh* GetMesh() { return _mesh; }

        // 绘制模型
        void Draw(unsigned int index) override;
        void Draw(Material* material, unsigned int index) override;

        MeshRenderer(GameObject& obj) :Renderer(obj), _mesh() {}

    private:
        Mesh* _mesh;
    };
}

