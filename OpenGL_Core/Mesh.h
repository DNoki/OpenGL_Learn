#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>

#include "Math.h"
#include "ResourceObject.h"
#include "BufferObject.h"
#include "VertexArrays.h"

namespace OpenGL_Core
{
    using namespace std;

    class Material;

    enum class MeshMode
    {
        /// <summary>
        /// 顶点数组模式
        /// </summary>
        ARRAYS,
        /// <summary>
        /// 索引绘制模式
        /// </summary>
        ELEMENTS,
    };
    enum class PresetMesh
    {
        /// <summary>
        /// 三角形
        /// </summary>
        TRIANGLE,
        /// <summary>
        /// 正方形
        /// </summary>
        SQUARE,
        /// <summary>
        /// 正方体
        /// </summary>
        BOX,
    };

    class Mesh : public ResourceObject
    {
    public:
        /// <summary>
        /// 创建预置网格
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        static unique_ptr<Mesh> CreatePresetMesh(PresetMesh name);

        bool Completed;

        /*  网格数据  */
        List<Vector3> vertices;
        List<Vector3> normals;
        List<Vector3> tangents;
        List<Vector4> colors;
        List<Vector2> uv;

        List<unsigned int> indices;

        /// <summary>
        /// 使用指定着色器Pass绘制模型 绘制前需输入变换、材质
        /// </summary>
        /// <param name="mat"></param>
        /// <param name="index"></param>
        void DrawMesh(Material& mat, unsigned int index);

        inline MeshMode GetMode() const { return this->mode; }
        /// <summary>
        /// 获取顶点数量
        /// </summary>
        /// <returns></returns>
        inline unsigned int GetVertexCount() const { return (unsigned int)vertices.size(); }
        /// <summary>
        /// 获取索引数量
        /// </summary>
        /// <returns></returns>
        inline unsigned int GetIndiceCount() const { return (unsigned int)indices.size(); }

        void Complete();


        Mesh(const string& name);
        ~Mesh();

    private:

        MeshMode mode;

        /*  渲染数据  */

        /// <summary>
        /// 顶点数组对象：Vertex Array Object，VAO
        /// </summary>
        VertexArrays VAO;
        /// <summary>
        /// 顶点缓冲对象：Vertex Buffer Object，VBO
        /// </summary>
        BufferObject VBO;
        /// <summary>
        /// 索引缓冲对象：Element Buffer Object，EBO
        /// </summary>
        BufferObject EBO;


    };


}