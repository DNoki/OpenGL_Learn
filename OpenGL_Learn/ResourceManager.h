#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <glad/glad.h>

#include "Camera.h"
#include "SkyBox.h"
#include "GameObject.h"

class ResourceManager
{
public:
	static UniformBufferObject UniformTransform;
	static UniformBufferObject UniformCamera;
	static UniformBufferObject UniformLight;
	static RendererState RealRendererState;

	static std::vector<std::unique_ptr<Shader>> Shaders;
	static std::vector<std::unique_ptr<Mesh>> Meshes;
	static std::vector<std::unique_ptr<Camera>> Cameras;
	static std::vector<std::unique_ptr<SkyBox>> SkyBoxs;
	static std::vector<std::unique_ptr<GameObject>> GameObjects;

	static void Initialize();
};

extern void AddShader(std::unique_ptr<Shader> shader);
extern Shader* FindShader(const std::string& name);
extern void AddMesh(std::unique_ptr<Mesh> mesh);
extern Mesh* FindMesh(const std::string& name, size_t vertexSize = 0);
extern GameObject* FindGameObject(const std::string& name);

// 使用指定渲染状态
extern void UseRendererState(const RendererState& state);
