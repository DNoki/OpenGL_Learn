#pragma once

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <glad/glad.h>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Material.h"

class Transform;

typedef std::tuple<Mesh*, Material> Tuple_Mesh_Material;

// ģ�������һ������Ͳ���һһ��Ӧ�İ� ��һϵ������Ͳ��ʵķ�װ
// �����������ݱ����ڳ����У����ظ�ʹ�ã� ��������ÿ��ģ�ͱ���
class MeshRenderer
{
public:
	static std::unique_ptr<MeshRenderer> AssimpLoad(const std::string & path, const std::string& shaderName);
	static std::unique_ptr<MeshRenderer> Create(const std::string & meshName, const std::string& shaderName);

	bool Enabled;

	void Draw();
	void AddBaseModel(Mesh& mesh, Material& material);
	inline std::vector<Tuple_Mesh_Material>& GetDatas() { return datas; }
	inline size_t GetMeshsCount() const { return datas.size(); }
	inline Material& GetMainMaterial() { return std::get<1>(this->datas[0]); }
	inline Material& GetMaterial(int i) { return std::get<1>(this->datas[i]); }

	MeshRenderer();
	MeshRenderer(Mesh& mesh, Material&& material);

private:
	std::vector<Tuple_Mesh_Material> datas;

	void processNode(aiNode *node, aiScene *scene, const Shader& shader);
	Tuple_Mesh_Material processMesh(aiMesh * mesh, const aiScene * scene, const Shader & shader);
};