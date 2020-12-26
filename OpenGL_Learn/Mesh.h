#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>

#include "Math.h"
#include "ResourceObject.h"
#include "BufferObject.h"
#include "VertexArrays.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Material;

    enum class MeshMode
    {
        ARRAYS,// 顶点数组模式
        ELEMENTS, // 索引绘制模式
    };
    enum class PresetMesh
    {
        TRIANGLE,// 三角形
        SQUARE, // 正方形
        BOX, // 正方体
    };

    class Mesh : public ResourceObject
    {
    public:
        // 创建预置网格
        static unique_ptr<Mesh> CreatePresetMesh(PresetMesh name);

        bool Completed;

        /*  网格数据  */
        List<Vector3> vertices;
        List<Vector3> normals;
        List<Vector3> tangents;
        List<Vector4> colors;
        List<Vector2> uv;

        List<unsigned int> indices;

        // 使用指定着色器Pass绘制模型 绘制前需输入变换、材质
        void DrawMesh(Material& mat, unsigned int index);

        inline MeshMode GetMode() const { return this->mode; }
        // 获取顶点数量
        inline unsigned int GetVertexCount() const { return (unsigned int)vertices.size(); }
        // 获取索引数量
        inline unsigned int GetIndiceCount() const { return (unsigned int)indices.size(); }

        void Complete();


        Mesh(const string& name);
        ~Mesh();

    private:

        MeshMode mode;

        /*  渲染数据  */
        // 顶点数组对象：Vertex Array Object，VAO
        VertexArrays VAO;
        // 顶点缓冲对象：Vertex Buffer Object，VBO
        BufferObject VBO;
        // 索引缓冲对象：Element Buffer Object，EBO
        BufferObject EBO;


    };


}