#include "pch.h"

#include "Graphics.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Material.h"

namespace OpenGL_Core
{
    List<unique_ptr<RenderItem>>& Renderer::GetRenderItems()
    {
        return _renderItems;
    }

    void Renderer::GenerateRenderItems()
    {
        _renderItems = List<unique_ptr<RenderItem>>();
        for (unsigned int i = 0; i < _material->GetPassCount(); i++)
        {
            auto item = make_unique<RenderItem>(this, _material, i, (*_material)[i]->RenderSequence);
            _renderItems.push_back(move(item));
        }
    }

    Renderer::Renderer(GameObject& obj) :Behaviour(obj), _material(), _renderItems() {}

    void MeshRenderer::Initialize(Mesh& mesh, Material& material)
    {
        _mesh = &mesh;
        _material = &material;
        GenerateRenderItems();
    }
    void MeshRenderer::Draw(unsigned int index)
    {
        if (!_material || !_mesh)
        {
            cout << "WARNING:: " << "网格和材质对象为空，忽略该网格渲染。" << endl;
            throw;
        }
        Graphics::DrawMesh(*_mesh, *_material, index);
    }
    void MeshRenderer::Draw(Material* material, unsigned int index)
    {
        if (!material || !_mesh)
        {
            cout << "WARNING:: " << "网格和材质对象为空，忽略该网格渲染。" << endl;
            throw;
        }
        Graphics::DrawMesh(*_mesh, *material, index);
    }
}