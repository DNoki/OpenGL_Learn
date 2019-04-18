#include "Renderer.h"

#include <iostream>

using namespace std;
using namespace glm;

extern void AddShader(unique_ptr<Shader> shader);
extern void AddMesh(std::unique_ptr<Mesh> mesh);
extern Mesh* FindMesh(const std::string& name, size_t vertexSize = 0);
extern Shader* FindShader(const string& name);
extern void CalculateTangent(Vertex& v1, Vertex& v2, Vertex& v3);

unique_ptr<MeshRenderer> MeshRenderer::AssimpLoad(const string & path, const string& shaderName)
{
	Assimp::Importer i;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.c_str(),
		aiPostProcessSteps::aiProcess_Triangulate |
		aiPostProcessSteps::aiProcess_FlipUVs |
		aiPostProcessSteps::aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		throw;
	}
	Shader* shader = FindShader(shaderName);
	if (!shader)
	{
		cout << "指定的Shader不存在。" << endl;
		throw;
	}

	auto renderer = make_unique<MeshRenderer>();
	renderer->processNode(scene->mRootNode, const_cast<aiScene*>(scene), *shader);
	return renderer;
}

std::unique_ptr<MeshRenderer> MeshRenderer::Create(const std::string & meshName, const std::string & shaderName)
{
	Shader* shader = FindShader(shaderName);
	if (!shader)
	{
		cout << "指定的Shader不存在。" << endl;
		throw;
	}
	return make_unique<MeshRenderer>(*Mesh::CreatePresetMesh(meshName), Material(*shader));
}

void MeshRenderer::Draw()
{
	for (auto& data : datas)
	{
		get<1>(data).UseMaterial();
		// 必须在使用Shader后在传送Uniform变量数据
		get<0>(data)->DrawMesh();
	}
}

void MeshRenderer::AddBaseModel(Mesh& mesh, Material& material)
{
	this->datas.push_back(Tuple_Mesh_Material(&mesh, material));
}

MeshRenderer::MeshRenderer() :Enabled(true) {}

MeshRenderer::MeshRenderer(Mesh& mesh, Material&& material) : MeshRenderer()
{
	AddBaseModel(mesh, material);
}

void MeshRenderer::processNode(aiNode * node, aiScene * scene, const Shader & shader)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		this->datas.push_back(processMesh(mesh, scene, shader));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, shader);
	}
}

Tuple_Mesh_Material MeshRenderer::processMesh(aiMesh * mesh, const aiScene * scene, const Shader & shader)
{ 
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	auto findMesh = FindMesh(mesh->mName.C_Str(), mesh->mNumVertices);
	if (findMesh)
	{
		cout << "找到重复网格" << endl;
		return Tuple_Mesh_Material(findMesh, Material(const_cast<Shader&>(shader)));
	}

	// 顶点
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.Position.x = mesh->mVertices[i].x;
		v.Position.y = mesh->mVertices[i].y;
		v.Position.z = mesh->mVertices[i].z;
		v.Normal.x = mesh->mNormals[i].x;
		v.Normal.y = mesh->mNormals[i].y;
		v.Normal.z = mesh->mNormals[i].z;
		if (mesh->HasTangentsAndBitangents())
		{
			v.Tangent.x = mesh->mTangents[i].x;
			v.Tangent.y = mesh->mTangents[i].y;
			v.Tangent.z = mesh->mTangents[i].z;
		}
		//for (size_t j = 0; j < 8; j++){}
		if (mesh->mTextureCoords[0])
		{
			v.TexCoord.x = mesh->mTextureCoords[0][i].x;
			v.TexCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else v.TexCoord = vec2(0.0f, 0.0f);

		vertices.push_back(v);
	}

	// 索引
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			if (face.mIndices[j] == 20)
			{
				cout << face.mIndices[j] << endl;
			}
			indices.push_back(face.mIndices[j]);
		}
		if (!mesh->HasTangentsAndBitangents() && face.mNumIndices == 3)
		{
			CalculateTangent(vertices[face.mIndices[0]], vertices[face.mIndices[1]], vertices[face.mIndices[2]]);
		}

	}
	// 材质
	if (mesh->mMaterialIndex >= 0)
	{
		auto material = scene->mMaterials[mesh->mMaterialIndex];
	}

	auto m = make_unique<Mesh>(mesh->mName.C_Str(), vertices, indices);
	auto rMesh = m.get();
	//Scene::Meshes.push_back(move(m));
	AddMesh(move(m));

	return Tuple_Mesh_Material(rMesh, Material(const_cast<Shader&>(shader)));
}

//#define TEST_MODEL
#ifdef TEST_MODEL

void main()
{

}

#endif // TEST_MODEL
