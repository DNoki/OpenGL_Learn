#include "Mesh.h"

#include <iostream>


#include "GameSystem.h"
#include "Material.h"

namespace OpenGL_Learn
{
    Vector3 CalcTangent(Vector3 pos1, Vector3 pos2, Vector3 pos3, Vector2 uv1, Vector2 uv2, Vector2 uv3)
    {
        auto edge1 = pos2 - pos1;
        auto edge2 = pos3 - pos1;
        auto deltaUV1 = uv2 - uv1;
        auto deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        auto result = Vector3::Right;
        result.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        result.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        result.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        return result.GetNormalized();
    }


    unique_ptr<Mesh> Mesh::CreatePresetMesh(PresetMesh name)
    {
        unique_ptr<Mesh> mesh;

        if (name == PresetMesh::TRIANGLE)// 边长为1的三角形
        {
            mesh = make_unique<Mesh>("Triangle");
            mesh->vertices.push_back(Vector3(-0.5f, 0.0f, -0.288675f));
            mesh->vertices.push_back(Vector3(0.5f, 0.0f, -0.288675f));
            mesh->vertices.push_back(Vector3(0.0f, 0.0f, 0.57735f));

            mesh->uv.push_back(Vector2(0.0f, 0.0f));
            mesh->uv.push_back(Vector2(1.0f, 0.0f));
            mesh->uv.push_back(Vector2(0.5f, 1.0f));

            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

            mesh->tangents.push_back(Vector3::Right);
            mesh->tangents.push_back(Vector3::Right);
            mesh->tangents.push_back(Vector3::Right);

            mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(1);
        }
        else if (name == PresetMesh::SQUARE)// 边长为1的正方形
        {
            mesh = make_unique<Mesh>("Square");
            mesh->vertices.push_back(Vector3(-0.5f, 0.0f, -0.5f));
            mesh->vertices.push_back(Vector3(-0.5f, 0.0f, 0.5f));
            mesh->vertices.push_back(Vector3(0.5f, 0.0f, -0.5f));
            mesh->vertices.push_back(Vector3(0.5f, 0.0f, 0.5f));

            mesh->uv.push_back(Vector2(0.0f, 0.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(1.0f, 0.0f));
            mesh->uv.push_back(Vector2(1.0f, 1.0f));

            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));

            mesh->tangents.push_back(Vector3::Right);
            mesh->tangents.push_back(Vector3::Right);
            mesh->tangents.push_back(Vector3::Right);
            mesh->tangents.push_back(Vector3::Right);

            mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
            mesh->indices.push_back(2); mesh->indices.push_back(1); mesh->indices.push_back(3);
            //mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(1);
            //mesh->indices.push_back(2); mesh->indices.push_back(3); mesh->indices.push_back(1);
        }
        else if (name == PresetMesh::BOX)// 边长为1的正方体
        {
            mesh = make_unique<Mesh>("Box");

            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));  // 0
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));  // 1
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));  // 2
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));  // 2
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));  // 1
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));  // 3

            mesh->vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));  // 1
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));  // 5
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));  // 3
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));  // 3
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));  // 5
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));  // 7

            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));  // 4
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));  // 5
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));  // 0
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));  // 0
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));  // 5
            mesh->vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));  // 1

            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));  // 4
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));  // 0
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));  // 6
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));  // 6
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));  // 0
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));  // 2

            mesh->vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));  // 5
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));  // 4
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));  // 7
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));  // 7
            mesh->vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));  // 4
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));  // 6

            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));  // 2
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));  // 3
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));  // 6
            mesh->vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));  // 6
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));  // 3
            mesh->vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));  // 7

            mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(1);
            mesh->indices.push_back(3); mesh->indices.push_back(5); mesh->indices.push_back(4);
            mesh->indices.push_back(6); mesh->indices.push_back(8); mesh->indices.push_back(7);
            mesh->indices.push_back(9); mesh->indices.push_back(11); mesh->indices.push_back(10);
            mesh->indices.push_back(12); mesh->indices.push_back(14); mesh->indices.push_back(13);
            mesh->indices.push_back(15); mesh->indices.push_back(17); mesh->indices.push_back(16);
            mesh->indices.push_back(18); mesh->indices.push_back(20); mesh->indices.push_back(19);
            mesh->indices.push_back(21); mesh->indices.push_back(23); mesh->indices.push_back(22);
            mesh->indices.push_back(24); mesh->indices.push_back(26); mesh->indices.push_back(25);
            mesh->indices.push_back(27); mesh->indices.push_back(29); mesh->indices.push_back(28);
            mesh->indices.push_back(30); mesh->indices.push_back(32); mesh->indices.push_back(31);
            mesh->indices.push_back(33); mesh->indices.push_back(35); mesh->indices.push_back(34);

            mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(-1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f));
            mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, -1.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f));
            mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->tangents.push_back(Vector3(0.0f, 0.0f, 1.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));


            mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
            mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
            mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
            mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
            mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f)); mesh->normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
            mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f)); mesh->normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
            mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f)); mesh->normals.push_back(Vector3(0.0f, 1.0f, 0.0f));


            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(0.0f, 1.0f));
            mesh->uv.push_back(Vector2(0.0f, 1.0f)); mesh->uv.push_back(Vector2(1.0f, 0.0f)); mesh->uv.push_back(Vector2(1.0f, 1.0f));
        }

        mesh->Complete();
        return move(mesh);
    }

    void Mesh::DrawMesh(Material& mat, unsigned int index)
    {
        if (!mat.UseShaderPass(index)) return;

        GameSystem::PrintError("绘制模型前：");
        VAO.BindVertexArray();
        if (GetMode() == MeshMode::ARRAYS)
            glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
        else if (GetMode() == MeshMode::ELEMENTS)
            glDrawElements(GL_TRIANGLES, GetIndiceCount(), GL_UNSIGNED_INT, 0);
        VAO.UnBindVertexArray();
        GameSystem::PrintError("绘制模型后：");
        GameSystem::DrawCallCounter++;
    }

    void Mesh::Complete()
    {
        GLsizeiptr size;
        auto vertexCount = vertices.size();
        auto normalCount = normals.size();
        auto tangentCount = tangents.size();
        auto uvCount = uv.size();
        auto colorCount = colors.size();

        if (vertexCount < 3)
        {
            cout << "LOG:: " << "MESH :: 输的的顶点数量低于3个。" << endl;
        }

        if (normalCount != vertexCount)
        {
            normalCount = 0;
            normals.clear();
        }
        if (tangentCount != vertexCount)
        {
            tangentCount = 0;
            tangents.clear();
        }
        if (colorCount != vertexCount)
        {
            colorCount = 0;
            colors.clear();
        }
        if (uvCount != vertexCount)
        {
            uvCount = 0;
            uv.clear();
        }

        size = vertexCount * sizeof(Vector3) + normalCount * sizeof(Vector3) + tangentCount * sizeof(Vector3) + colorCount * sizeof(Vector4) + uvCount * sizeof(Vector2);

        VAO.BindVertexArray(); // 绑定顶点数组
        VBO.BufferData(size, NULL);// 先创造size大小的空间，后输入数据

        GLintptr offset = 0;

        {
            VBO.BufferSubData(offset, vertexCount * sizeof(Vector3), &(vertices[0]));
            VAO.VertexAttribPointer(0, 3, sizeof(Vector3), (void*)(offset));
            offset += vertexCount * sizeof(Vector3);
        }

        if (normalCount != 0)
        {
            VBO.BufferSubData(offset, normalCount * sizeof(Vector3), &(normals[0]));
            VAO.VertexAttribPointer(1, 3, sizeof(Vector3), (void*)(offset));
            offset += normalCount * sizeof(Vector3);
        }
        if (tangentCount != 0)
        {
            VBO.BufferSubData(offset, tangentCount * sizeof(Vector3), &(tangents[0]));
            VAO.VertexAttribPointer(2, 3, sizeof(Vector3), (void*)(offset));
            offset += tangentCount * sizeof(Vector3);
        }
        if (colorCount != 0)
        {
            VBO.BufferSubData(offset, colorCount * sizeof(Vector4), &(colors[0]));
            VAO.VertexAttribPointer(3, 4, sizeof(Vector4), (void*)(offset));
            offset += colorCount * sizeof(Vector4);
        }
        if (uvCount != 0)
        {
            VBO.BufferSubData(offset, uvCount * sizeof(Vector2), &(uv[0]));
            VAO.VertexAttribPointer(4, 2, sizeof(Vector2), (void*)(offset));
            offset += uvCount * sizeof(Vector2);
        }

        if (indices.size() > 0)
        {
            EBO.BufferData(indices.size() * sizeof(unsigned int), &(indices[0]));
            mode = MeshMode::ELEMENTS;
        }

        Completed = true;
    }

    Mesh::Mesh(const string& name) :ResourceObject(name), Completed(false), mode(MeshMode::ARRAYS),
        vertices(), normals(), tangents(), colors(), uv(), indices(),
        VAO(), VBO(GL_ARRAY_BUFFER), EBO(GL_ELEMENT_ARRAY_BUFFER)
    {
        cout << "LOG:: " << "网格:: Name：" << this->Name << " 已生成"
            << " VAO_ID=" << VAO.ID
            << " VBO_ID=" << VBO.ID
            << " EBO_ID=" << EBO.ID
            << endl << endl;
    }

    Mesh::~Mesh()
    {
        cout << "LOG:: " << "网格:: Name：" << this->Name << " 已释放"
            << " VAO_ID=" << VAO.ID
            << " VBO_ID=" << VBO.ID
            << " EBO_ID=" << EBO.ID
            << endl << endl;
    }

    //void Mesh::setupMesh()
    //{
    //    // 索引绘制模式基本流程
    //
    //    //glGenVertexArrays(1, &VAO);// 顶点数组对象
    //    //glGenBuffers(1, &VBO.ID);// 顶点缓冲对象
    //    //glGenBuffers(1, &EBO.ID);// 索引缓冲对象
    //
    //    //glBindVertexArray(VAO);// 绑定VAO
    //
    //    // 复制顶点数组到缓冲中供OpenGL使用    
    //    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    //    // 复制索引数组到缓冲中
    //    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //
    //    // 链接顶点属性
    //    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    //    //glEnableVertexAttribArray(0);
    //    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //    //glEnableVertexAttribArray(1);
    //    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    //    //glEnableVertexAttribArray(2);
    //    //glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    //    //glEnableVertexAttribArray(3);
    //    //glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    //    //glEnableVertexAttribArray(4);
    //
    //    //glBindVertexArray(0);// 解绑VAO
    //}



}