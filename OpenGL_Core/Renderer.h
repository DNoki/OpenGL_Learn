#pragma once

#include "Behaviour.h"

#include "Material.h"

namespace OpenGL_Core
{
    using namespace std;

    class Mesh;
    class Renderer;

    /// <summary>
    /// 渲染项
    /// </summary>
    struct RenderItem
    {
    public:
        /// <summary>
        /// 使用的渲染器
        /// </summary>
        Renderer* renderer;
        /// <summary>
        /// 使用的材质
        /// </summary>
        Material* material;
        /// <summary>
        /// 到相机距离（渲染透明模型时使用）
        /// </summary>
        float depth;
        /// <summary>
        /// 使用着色器索引
        /// </summary>
        unsigned int index;
        /// <summary>
        /// 渲染队列
        /// </summary>
        unsigned int sequence;

        RenderItem(Renderer* r, Material* m, unsigned int i, unsigned int s) :renderer(r), material(m), depth(), index(i), sequence(s) {}
    };

    /// <summary>
    /// 渲染器（虚类，包含了一个材质）
    /// </summary>
    class Renderer : public Behaviour
    {
    public:
        /// <summary>
        /// 获取材质
        /// </summary>
        /// <returns></returns>
        inline Material* GetMaterial() { return _material; }
        List<unique_ptr<RenderItem>>& GetRenderItems();
        /// <summary>
        /// 绘制
        /// </summary>
        /// <param name="index">绘制指定着色器</param>
        virtual void Draw(unsigned int index) = 0;
        /// <summary>
        /// 使用指定材质的指定着色器绘制
        /// </summary>
        /// <param name="material"></param>
        /// <param name="index"></param>
        virtual void Draw(Material* material, unsigned int index) = 0;

    protected:
        Material* _material;
        List<unique_ptr<RenderItem>> _renderItems;

        void GenerateRenderItems();

        Renderer(GameObject& obj);

    private:
    };

    /// <summary>
    /// 网格渲染器（包含了一对材质和模型）
    /// </summary>
    class MeshRenderer final : public Renderer
    {
    public:
        inline Mesh* GetMesh() { return _mesh; }

        /// <summary>
        /// 绘制模型
        /// </summary>
        /// <param name="index"></param>
        void Draw(unsigned int index) override;
        void Draw(Material* material, unsigned int index) override;

        /// <summary>
        /// 设置一对模型与材质
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="material"></param>
        void Initialize(Mesh& mesh, Material& material);

        MeshRenderer(GameObject& obj) :Renderer(obj), _mesh() {}

    private:
        Mesh* _mesh;
    };
}

