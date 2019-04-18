#include "SkyBox.h"

#include "Material.h"

using namespace std;

static const float SkyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,	-1.0f, -1.0f, -1.0f,	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,	 1.0f,  1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,	-1.0f, -1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,	 1.0f, -1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,	 1.0f,  1.0f, -1.0f,	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,	 1.0f, -1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,	 1.0f,  1.0f, -1.0f,	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0f,	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0f,	 1.0f, -1.0f,  1.0f
};

void SkyBox::DrawSkyBox()
{
	material->UseMaterial();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

SkyBox::SkyBox(const std::string& shader)
{
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	this->VAO = vao;
	this->VBO = vbo;
	this->material = make_unique<Material>(shader);
}

SkyBox::SkyBox(const std::string & shader, TextureCube * texture, const std::string & samplerName, GLuint unit) :SkyBox(shader)
{
	this->material->BindTexture(texture, samplerName, unit);
}
