#include "ResourceManager.h"



using namespace std;

UniformBufferObject ResourceManager::UniformTransform;
UniformBufferObject ResourceManager::UniformCamera;
UniformBufferObject ResourceManager::UniformLight;
RendererState ResourceManager::RealRendererState;

vector<unique_ptr<Shader>> ResourceManager::Shaders;
vector<unique_ptr<Mesh>> ResourceManager::Meshes;
vector<unique_ptr<Camera>> ResourceManager::Cameras;
vector<unique_ptr<SkyBox>> ResourceManager::SkyBoxs;
vector<unique_ptr<GameObject>> ResourceManager::GameObjects;

void ResourceManager::Initialize()
{
	ResourceManager::UniformTransform = UniformBufferObject(3 * sizeof(glm::mat4), 0);
	ResourceManager::UniformCamera = UniformBufferObject(1 * sizeof(glm::vec4), 1);
	ResourceManager::UniformLight = UniformBufferObject(2 * sizeof(glm::vec4), 2);
	ResourceManager::RealRendererState = RendererState();
}

void AddShader(unique_ptr<Shader> shader)
{
	if (FindShader(shader->ShaderName))
	{
		cout << "已存在同名Shader，添加失败。" << endl;
		return;
	}
	ResourceManager::Shaders.push_back(move(shader));
}
Shader * FindShader(const string & name)
{
	for (auto& s : ResourceManager::Shaders)
	{
		if (s->ShaderName == name)
		{
			return s.get();
		}
	}
	return nullptr;
}

void AddMesh(unique_ptr<Mesh> mesh)
{
	if (FindMesh(mesh->name, mesh->GetVertexCount()))
	{
		cout << "已存在同名Mesh，添加失败。" << endl;
		return;
	}
	ResourceManager::Meshes.push_back(move(mesh));
}
Mesh * FindMesh(const string & name, size_t vertexSize)
{
	for (auto& m : ResourceManager::Meshes)
	{
		if (m->name == name)
		{
			if (vertexSize == 0 || m->GetVertexCount() == vertexSize)// 顶点为0时直接返回同名网格
			{
				return m.get();
			}
		}
	}
	return nullptr;
}

GameObject * FindGameObject(const std::string & name)
{
	for (auto& obj : ResourceManager::GameObjects)
	{
		if (obj->Name == name)
		{
			return obj.get();
		}
	}
	return nullptr;
}

// 使用指定渲染状态
void UseRendererState(const RendererState & state)
{
	auto& realState = ResourceManager::RealRendererState;

	// 深度测试
	if (realState.DepthTest != state.DepthTest)
	{
		realState.DepthTest = state.DepthTest;
		realState.DepthTestMode = state.DepthTestMode;
		realState.DepthMask = state.DepthMask;

		RendererState::SetState(GL_DEPTH_TEST, realState.DepthTest);
		glDepthFunc(realState.DepthTestMode);
		glDepthMask(realState.DepthMask);
	}
	else
	{
		if (realState.DepthTest &&
			(realState.DepthTestMode != state.DepthTestMode || realState.DepthMask != state.DepthMask))
		{
			realState.DepthTestMode = state.DepthTestMode;
			realState.DepthMask = state.DepthMask;
			glDepthFunc(realState.DepthTestMode);
			glDepthMask(realState.DepthMask);
		}
	}
	// 混合
	if (realState.Blend != state.Blend)
	{
		realState.Blend = state.Blend;
		realState.Sfactor = state.Sfactor;
		realState.Dfactor = state.Dfactor;
		realState.SfactorAlpha = state.SfactorAlpha;
		realState.DfactorAlpha = state.DfactorAlpha;
		RendererState::SetState(GL_BLEND, realState.Blend);
		glBlendFuncSeparate(realState.Sfactor, realState.Dfactor, realState.SfactorAlpha, realState.DfactorAlpha);
	}
	else
	{
		if (realState.Blend &&
			(realState.Sfactor != state.Sfactor || realState.Dfactor != state.Dfactor ||
				realState.SfactorAlpha != state.SfactorAlpha || realState.DfactorAlpha != state.DfactorAlpha))
		{
			realState.Sfactor = state.Sfactor;
			realState.Dfactor = state.Dfactor;
			realState.SfactorAlpha = state.SfactorAlpha;
			realState.DfactorAlpha = state.DfactorAlpha;
			glBlendFuncSeparate(realState.Sfactor, realState.Dfactor, realState.SfactorAlpha, realState.DfactorAlpha);
		}
	}
	// 面剔除
	if (realState.CullFace != state.CullFace)
	{
		realState.CullFace = state.CullFace;
		realState.CullFaceMode = state.CullFaceMode;
		RendererState::SetState(GL_CULL_FACE, realState.CullFace);
		glCullFace(realState.CullFaceMode);
	}
	else
	{
		if (realState.CullFace && realState.CullFaceMode != state.CullFaceMode)
		{
			realState.CullFaceMode = state.CullFaceMode;
			glCullFace(realState.CullFaceMode);
		}
	}
	// 正向面
	if (realState.FrontFace != state.FrontFace)
	{
		realState.FrontFace = state.FrontFace;
		glFrontFace(realState.FrontFace);
	}
}


