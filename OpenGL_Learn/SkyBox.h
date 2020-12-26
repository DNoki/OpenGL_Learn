#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include "ResourceObject.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Material;
    class Mesh;

    // ��պ�
    class Skybox : public ResourceObject
    {
    public:
        // ������պ�
        // DepthTest = GL_TRUE;
        // DepthTestMode = GL_LEQUAL;
        void DrawSkyBox();

        Skybox(const string& name, Material* material);

    private:
        // ��պл�����������
        static Mesh* skyboxMesh;

        Material* _material;
    };
}