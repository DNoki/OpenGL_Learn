#include "UniformManager.h"

#include "Math.h"
#include "Shader.h"

namespace OpenGL_Core
{
	unique_ptr<UniformBufferObject> UniformManager::Transform = nullptr;
	unique_ptr<UniformBufferObject> UniformManager::CameraData = nullptr;
	unique_ptr<UniformBufferObject> UniformManager::LightingData = nullptr;
	unique_ptr<UniformBufferObject> UniformManager::Time = nullptr;


	void UniformManager::UniformBlockBinding(Shader* shader)
	{
		if (!shader) return;
		shader->SetUniformBlockBinding(Transform->Name, Transform->BindedIndex);
		shader->SetUniformBlockBinding(CameraData->Name, CameraData->BindedIndex);
		shader->SetUniformBlockBinding(LightingData->Name, LightingData->BindedIndex);
		shader->SetUniformBlockBinding(Time->Name, Time->BindedIndex);
	}

	void UniformManager::Initialize()
	{
		Transform = make_unique<UniformBufferObject>("TRANSFORM", 3 * sizeof(Matrix4x4), 0);
		CameraData = make_unique<UniformBufferObject>("CAMERA_DATA", 3 * sizeof(Vector4), 1);
		LightingData = make_unique<UniformBufferObject>("LIGHTING_DATA", 76 * sizeof(float), 2);
		Time = make_unique<UniformBufferObject>("TIME", 1 * sizeof(Vector4), 3);
	}

}