#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include "ResourceObject.h"

namespace OpenGL_Core
{
    using namespace std;

    class Material;
    class Mesh;

    /// <summary>
    /// 天空盒
    /// </summary>
    class Skybox : public ResourceObject
    {
    public:
        /// <summary>
        /// 绘制天空盒
        /// DepthTest = GL_TRUE;
        /// DepthTestMode = GL_LEQUAL;
        /// </summary>
        void DrawSkyBox();

        Skybox(const string& name, Material* material);

    private:
        /// <summary>
        /// 天空盒绘制所需网格
        /// </summary>
        static Mesh* skyboxMesh;

        Material* _material;
    };
}