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
        ARRAYS,// ��������ģʽ
        ELEMENTS, // ��������ģʽ
    };
    enum class PresetMesh
    {
        TRIANGLE,// ������
        SQUARE, // ������
        BOX, // ������
    };

    class Mesh : public ResourceObject
    {
    public:
        // ����Ԥ������
        static unique_ptr<Mesh> CreatePresetMesh(PresetMesh name);

        bool Completed;

        /*  ��������  */
        List<Vector3> vertices;
        List<Vector3> normals;
        List<Vector3> tangents;
        List<Vector4> colors;
        List<Vector2> uv;

        List<unsigned int> indices;

        // ʹ��ָ����ɫ��Pass����ģ�� ����ǰ������任������
        void DrawMesh(Material& mat, unsigned int index);

        inline MeshMode GetMode() const { return this->mode; }
        // ��ȡ��������
        inline unsigned int GetVertexCount() const { return (unsigned int)vertices.size(); }
        // ��ȡ��������
        inline unsigned int GetIndiceCount() const { return (unsigned int)indices.size(); }

        void Complete();


        Mesh(const string& name);
        ~Mesh();

    private:

        MeshMode mode;

        /*  ��Ⱦ����  */
        // �����������Vertex Array Object��VAO
        VertexArrays VAO;
        // ���㻺�����Vertex Buffer Object��VBO
        BufferObject VBO;
        // �����������Element Buffer Object��EBO
        BufferObject EBO;


    };


}