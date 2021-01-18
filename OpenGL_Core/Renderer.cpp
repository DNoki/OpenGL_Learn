#include "pch.h"

#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

namespace OpenGL_Core
{
    unique_ptr<List<unique_ptr<RenderItem>>> Renderer::GetRenderItems()
    {
        auto result = make_unique<List<unique_ptr<RenderItem>>>();
        for (unsigned int i = 0; i < _material->GetPassCount(); i++)
        {
            auto item = make_unique<RenderItem>(this, _material, i, (*_material)[i]->RenderSequence);
            result->push_back(move(item));
        }
        return result;
    }

    void MeshRenderer::SetData(Mesh& mesh, Material& material)
    {
        _mesh = &mesh;
        _material = &material;
    }
    void MeshRenderer::Draw(unsigned int index)
    {
        if (!_material || !_mesh)
        {
            cout << "WARNING:: " << "网格和材质对象为空，忽略该网格渲染。" << endl;
            throw;
        }
        this->_mesh->DrawMesh(*_material, index);
    }
    void MeshRenderer::Draw(Material* material, unsigned int index)
    {
        if (!material || !_mesh)
        {
            cout << "WARNING:: " << "网格和材质对象为空，忽略该网格渲染。" << endl;
            throw;
        }
        this->_mesh->DrawMesh(*material, index);
    }
}