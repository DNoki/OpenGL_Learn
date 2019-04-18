#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Vertex
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	
	Vertex();
	Vertex(
		float Px, float Py, float Pz,
		float Nx, float Ny, float Nz,
		 float Tx, float Ty, float Tz,
		float Cr,float Cg,float Cb,float Ca,
		float u, float v);
};

class Mesh
{
public:
	// 创建预置网格
	static Mesh* CreatePresetMesh(const std::string &name);

	std::string name;

	// 绘制模型 绘制前需启用Shader，并且输入变换、材质
	void DrawMesh();
	// 获取顶点数量
	inline size_t GetVertexCount() { return vertices.size(); }
	// 获取索引数量
	inline size_t GetIndiceCount() { return indices.size(); }

	Mesh(const std::string& name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

private:
	/*  网格数据  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	/*  渲染数据  */
	unsigned int VAO, VBO, EBO;

	// 初始化缓冲
	void setupMesh();
};

