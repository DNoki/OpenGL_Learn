#include "Mesh.h"

#include <iostream>
#include <glad/glad.h>

using namespace std;
using namespace glm;

extern void AddMesh(std::unique_ptr<Mesh> mesh);
extern Mesh* FindMesh(const std::string& name, size_t vertexSize = 0);

float sqrLength(vec3 v)
{
	return pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
}
void CalculateTangent(Vertex& v1, Vertex& v2, Vertex& v3)
{
	vec3 edge1 = v2.Position - v1.Position;
	vec3 edge2 = v3.Position - v1.Position;
	vec2 deltaUV1 = v2.TexCoord - v1.TexCoord;
	vec2 deltaUV2 = v3.TexCoord - v1.TexCoord;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	//if (sqrLength(v1.Tangent) < 0.1f) v1.Tangent = normalize(tangent);
	//else v1.Tangent = mix(normalize(tangent), v1.Tangent, 0.5f);
	//if (sqrLength(v2.Tangent) < 0.1f) v2.Tangent = v1.Tangent;
	//else v2.Tangent = mix(v1.Tangent, v2.Tangent, 0.5f);
	//if (sqrLength(v3.Tangent) < 0.1f) v3.Tangent = v1.Tangent;
	//else v3.Tangent = mix(v1.Tangent, v3.Tangent, 0.5f);
	v1.Tangent = normalize(tangent);
	v2.Tangent = v1.Tangent;
	v3.Tangent = v1.Tangent;

	//auto formula = [](Vertex& _v1, Vertex& _v2, Vertex& _v3)
	//{
	//	vec3 edge1 = _v2.Position - _v1.Position;
	//	vec3 edge2 = _v3.Position - _v1.Position;
	//	vec2 deltaUV1 = _v2.TexCoord - _v1.TexCoord;
	//	vec2 deltaUV2 = _v3.TexCoord - _v1.TexCoord;

	//	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	//	vec3 tangent;
	//	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	//	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	//	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	//	_v1.Tangent = normalize(tangent);
	//	//if (sqrLength(_v1.Tangent) < 0.1f)
	//	//{
	//	//	_v1.Tangent = normalize(tangent);
	//	//}
	//	//else
	//	//{
	//	//	_v1.Tangent = normalize(mix(_v1.Tangent, normalize(tangent), 0.5f));
	//	//}
	//	//auto ttt = _v1.Tangent;
	//	//cout << ttt.x << " " << ttt.y << " " << ttt.z << endl;
	//};
	//formula(v1, v2, v3);
	//formula(v2, v1, v3);
	//formula(v3, v1, v2);
}

Vertex::Vertex() {}

Vertex::Vertex(float Px, float Py, float Pz,
	float Nx, float Ny, float Nz,
	float Tx, float Ty, float Tz,
	float Cr, float Cg, float Cb, float Ca,
	float u, float v)
{
	this->Position.x = Px;
	this->Position.y = Py;
	this->Position.z = Pz;

	this->Normal.x = Nx;
	this->Normal.y = Ny;
	this->Normal.z = Nz;

	this->Tangent.x = Tx;
	this->Tangent.y = Ty;
	this->Tangent.z = Tz;

	this->Color.r = Cr;
	this->Color.g = Cg;
	this->Color.b = Cb;
	this->Color.a = Ca;

	this->TexCoord.x = u;
	this->TexCoord.y = v;
}


Mesh* Mesh::CreatePresetMesh(const string &name)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	if (name == "Triangle")// 边长为1的三角形
	{
		auto findMesh = FindMesh(name, 3);
		if (findMesh) { return findMesh; }
		vertices.push_back(Vertex(
			-0.5f, -0.288675f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f));
		vertices.push_back(Vertex(
			0.0f, 0.57735f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.5f, 1.0f));
		vertices.push_back(Vertex(
			0.5f, -0.288675f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f));
		indices.push_back(0); indices.push_back(2); indices.push_back(1);
	}
	else if (name == "Square")// 边长为1的正方形
	{
		auto findMesh = FindMesh(name, 4);
		if (findMesh) { return findMesh; }
		vertices.push_back(Vertex(
			-0.5f, -0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f));
		vertices.push_back(Vertex(
			-0.5f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f));
		vertices.push_back(Vertex(
			0.5f, -0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f));
		vertices.push_back(Vertex(
			0.5f, 0.5f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f));
		indices.push_back(0); indices.push_back(2); indices.push_back(1);
		indices.push_back(2); indices.push_back(3); indices.push_back(1);
	}
	else if (name == "Box")
	{

	}
	else
	{
		cout << "指定名称的网格不存在。" << endl;
		return nullptr;
	}
	auto m = make_unique<Mesh>(name, vertices, indices);
	auto result = m.get();
	AddMesh(move(m));
	return result;
}

void Mesh::DrawMesh()
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(const std::string& name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
	:name(name), vertices(vertices), indices(indices)
{
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);// 顶点数组对象
	glGenBuffers(1, &VBO);// 顶点缓冲对象
	glGenBuffers(1, &EBO);// 索引缓冲对象

	glBindVertexArray(VAO);// 绑定VAO

	// 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	// 复制索引数组到缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// 链接顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);// 解绑VAO
}

