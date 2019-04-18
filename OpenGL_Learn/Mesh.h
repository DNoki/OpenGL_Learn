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
	// ����Ԥ������
	static Mesh* CreatePresetMesh(const std::string &name);

	std::string name;

	// ����ģ�� ����ǰ������Shader����������任������
	void DrawMesh();
	// ��ȡ��������
	inline size_t GetVertexCount() { return vertices.size(); }
	// ��ȡ��������
	inline size_t GetIndiceCount() { return indices.size(); }

	Mesh(const std::string& name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

private:
	/*  ��������  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	/*  ��Ⱦ����  */
	unsigned int VAO, VBO, EBO;

	// ��ʼ������
	void setupMesh();
};

