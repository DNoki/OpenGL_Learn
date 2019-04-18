#include "Camera.h"

#include <iostream>
#include "Transform.h"
#include "Input.h"

using namespace std;
using namespace glm;

extern int ScreenWidth, ScreenHeight;

bool temp = false;

const glm::mat4 CameraBasis = mat4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

void Camera::Update()
{
	auto moveSpeed = 5.0f;
	auto move = V3_ZERO;
	move += Input::GetKey(KeyCode::W) ? Time::DeltaTime() * moveSpeed * V3_FORWARD : V3_ZERO;
	move += Input::GetKey(KeyCode::S) ? Time::DeltaTime() * -moveSpeed * V3_FORWARD : V3_ZERO;
	move += Input::GetKey(KeyCode::A) ? Time::DeltaTime() * moveSpeed * V3_RIGHT : V3_ZERO;
	move += Input::GetKey(KeyCode::D) ? Time::DeltaTime() * -moveSpeed * V3_RIGHT : V3_ZERO;
	move += Input::GetKey(KeyCode::SPACE) ? Time::DeltaTime() * moveSpeed * V3_UP : V3_ZERO;
	move += Input::GetKey(KeyCode::C) ? Time::DeltaTime() * -moveSpeed * V3_UP : V3_ZERO;

	GetTransform().Translate(move);

	auto rotateSpeed = 90.0f;
	auto rotate = 0.0f;
	rotate += Input::GetKey(KeyCode::Q) ? Time::DeltaTime() * rotateSpeed : 0;
	rotate += Input::GetKey(KeyCode::E) ? Time::DeltaTime() * -rotateSpeed : 0;
	GetTransform().Rotate(V3_UP, rotate, true);

	rotate = 0.0f;
	rotate += Input::GetKey(KeyCode::Y) ? Time::DeltaTime() * -rotateSpeed : 0;
	rotate += Input::GetKey(KeyCode::H) ? Time::DeltaTime() * rotateSpeed : 0;
	GetTransform().Rotate(V3_RIGHT, rotate, false);
}

glm::mat4 Camera::GetViewMatrix() const
{
	// 将世界中的相机的基变换到OpenGl视口基
	/*相机空间与OpenGL约定相匹配：相机的前向是负Z轴。
	在世界坐标系中，使用左手坐标系，
	在视口中，使用右手坐标系，屏幕对着你的方向是+Z
	*/
	//auto pos = (vec3)(CameraBasis *vec4(this->transform->position, 1.0f));
	//auto center = (vec3)(CameraBasis *vec4(this->transform->GetForward(), 1.0f)) + pos;
	//auto up = (vec3)(CameraBasis * vec4(this->transform->GetUp(), 1.0f));
	auto pos = this->transform->position;
	auto center = this->transform->GetForward() + pos;
	auto up = this->transform->GetUp();

	//return getProjectionMatrix() * Camera::cameraBasis * inverse(this->transform->GetTransformMatrix());
	return lookAt(pos, center, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	mat4 projectionMatrix(1.0f);
	auto aspect = (GLfloat)ScreenWidth / ScreenHeight;
	if (_projectionMode == ProjectionMode::PERSPECTIVE)
	{
		projectionMatrix = perspective(radians(_fieldOfView), aspect, _nearClipPlane, _farClipPlane);
	}
	else
	{
		auto width = aspect * _orthographicSize;
		auto height = _orthographicSize;
		projectionMatrix = ortho(-width, width, -height, height, _nearClipPlane, _farClipPlane);
	}
	return projectionMatrix;
}

void Camera::SetProjectionMode(ProjectionMode mode)
{
	_projectionMode = mode;
}

void Camera::SetClipPlane(float _near, float _far)
{
	this->_nearClipPlane = max(_near, 0.001f);
	this->_farClipPlane = max(_far, this->_nearClipPlane + 0.001f);
}

Camera::Camera(string name, std::unique_ptr<Transform> trans, glm::vec3 target) :
	Enabled(true), Name(name), transform(move(trans)), focus(target)
{
	_projectionMode = ProjectionMode::PERSPECTIVE;
	_fieldOfView = 60.0f;
	_orthographicSize = 5;
	_nearClipPlane = 0.1f;
	_farClipPlane = 1000.0f;
}
