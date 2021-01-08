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

    // 天空盒
    class Skybox : public ResourceObject
    {
    public:
        // 绘制天空盒
        // DepthTest = GL_TRUE;
        // DepthTestMode = GL_LEQUAL;
        void DrawSkyBox();

        Skybox(const string& name, Material* material);

    private:
        // 天空盒绘制所需网格
        static Mesh* skyboxMesh;

        Material* _material;
    };
}