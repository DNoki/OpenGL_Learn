#pragma once

#include "RenderTexture.h"

namespace OpenGL_Core
{
    class Mesh;
    class Material;
    struct Matrix4x4;

    class Graphics
    {
    public:
        /// <summary>
        /// 位块传送 拷贝到指定帧缓冲
        /// </summary>
        /// <param name="source">拷贝元</param>
        /// <param name="dest">拷贝目标</param>
        /// <param name="mask">指定要读取的缓冲区 // GL_COLOR_BUFFER_BIT 颜色缓冲区 // GL_DEPTH_BUFFER_BIT 深度缓冲区 // GL_STENCIL_BUFFER_BIT 模板缓冲区</param>
        /// <param name="filter">伸缩变形时的插值方法 GL_NEAREST  GL_LINEAR</param>
        static void Blit(const RenderTexture& source, const RenderTexture& dest, GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, ScaleFilterType filter = ScaleFilterType::NEAREST);

        /// <summary>
        /// 使用指定材质的Shader和模型变换绘制网格
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="material"></param>
        /// <param name="index"></param>
        static void DrawMesh(const Mesh& mesh, const Matrix4x4& matrix, const Material& material, unsigned int index);
        /// <summary>
        /// 使用指定材质的Shader绘制网格
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="material"></param>
        /// <param name="index"></param>
        static void DrawMesh(const Mesh& mesh, const Material& material, unsigned int index);
    };

}
