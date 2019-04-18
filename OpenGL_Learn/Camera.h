#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Transform;
class SkyBox;

enum ProjectionMode
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};

class Camera
{
public:
	bool Enabled;
	std::string Name;
	// 相机空间
	//const static glm::mat4 CameraBasis;

	inline Transform& GetTransform() const { return *transform; }
	void Update();

	// 获取观察矩阵
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void SetProjectionMode(ProjectionMode mode);
	// 设置视角
	inline void SetFieldOfView(float angle) { this->_fieldOfView = glm::clamp(angle, 0.001f, 179.0f); }
	// 设置正交视图大小
	inline void SetOrthographicSize(float size) { this->_orthographicSize = size; }
	// 设置裁切面
	void SetClipPlane(float _near, float _far);

	Camera(std::string name,std::unique_ptr<Transform> trans, glm::vec3 target);

private:
	std::unique_ptr<Transform> transform;
	glm::vec3 focus;
	ProjectionMode _projectionMode;
	// 透视模式的视角 角度制
	float _fieldOfView;
	// 正交模式下相机的半尺寸
	float _orthographicSize;

	// 近裁面
	float _nearClipPlane;
	// 远裁面
	float _farClipPlane;
};