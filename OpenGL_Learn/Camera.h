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
	// ����ռ�
	//const static glm::mat4 CameraBasis;

	inline Transform& GetTransform() const { return *transform; }
	void Update();

	// ��ȡ�۲����
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void SetProjectionMode(ProjectionMode mode);
	// �����ӽ�
	inline void SetFieldOfView(float angle) { this->_fieldOfView = glm::clamp(angle, 0.001f, 179.0f); }
	// ����������ͼ��С
	inline void SetOrthographicSize(float size) { this->_orthographicSize = size; }
	// ���ò�����
	void SetClipPlane(float _near, float _far);

	Camera(std::string name,std::unique_ptr<Transform> trans, glm::vec3 target);

private:
	std::unique_ptr<Transform> transform;
	glm::vec3 focus;
	ProjectionMode _projectionMode;
	// ͸��ģʽ���ӽ� �Ƕ���
	float _fieldOfView;
	// ����ģʽ������İ�ߴ�
	float _orthographicSize;

	// ������
	float _nearClipPlane;
	// Զ����
	float _farClipPlane;
};