#include "pch.h"

#include "UniformManager.h"
#include "GameSystem.h"
#include "Mesh.h"
#include "Material.h"

#include "Graphics.h"

namespace OpenGL_Core
{
    void Graphics::Blit(const RenderTexture& source, const RenderTexture& dest, GLbitfield mask, ScaleFilterType filter)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetFrameBufferID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.GetFrameBufferID());
        glBlitFramebuffer(0, 0, dest.GetWidth(), dest.GetHeight(), 0, 0, source.GetWidth(), source.GetHeight(), mask, static_cast<int>(filter));
    }

    void Graphics::DrawMesh(const Mesh& mesh, const Matrix4x4& matrix, const Material& material, unsigned int index)
    {
        if (!material.UseShaderPass(index)) return;

        UniformManager::Transform->SetSubData(2 * sizeof(Matrix4x4), sizeof(Matrix4x4), matrix.GetPtr());
        DrawMesh(mesh, material, index);
    }
    void Graphics::DrawMesh(const Mesh& mesh, const Material& material, unsigned int index)
    {
        if (!material.UseShaderPass(index)) return;

        auto renderMode = (GLenum)mesh.GetRenderMode();

        GameSystem::PrintError("绘制模型前：");
        mesh.GetVAO()->BindVertexArray();
        if (mesh.GetMode() == MeshMode::ARRAYS)
            glDrawArrays(renderMode, 0, mesh.GetVertexCount());
        else if (mesh.GetMode() == MeshMode::ELEMENTS)
            glDrawElements(renderMode, mesh.GetIndiceCount(), GL_UNSIGNED_INT, 0);
        //mesh.GetVAO()->UnBindVertexArray();
        GameSystem::PrintError("绘制模型后：");
        GameSystem::DrawCallCounter++;
    }
}