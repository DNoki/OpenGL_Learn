#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>

class Shader;
class Material;
class TextureCube;

class SkyBox
{
public:	
	inline Material& GetMaterial() { return *material; }
	void DrawSkyBox();

	SkyBox(const std::string& shader);
	SkyBox(const std::string& shader, TextureCube* texture, const std::string& samplerName, GLuint unit);

private:
	GLuint VAO, VBO;
	std::unique_ptr<Material> material;
};

//unsigned int skyboxVAO, skyboxVBO;
//glGenVertexArrays(1, &skyboxVAO);
//glGenBuffers(1, &skyboxVBO);
//glBindVertexArray(skyboxVAO);
//glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//glEnableVertexAttribArray(0);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

//auto skyBoxMaterial = Material("SkyBox");
//skyBoxMaterial.BindTexture(&skyCubeTexture, "skybox", 0);