#include "Transform.h"

#include <assert.h>
//#include <iostream>
//
//using namespace std;
using namespace glm;

mat4 Transform::GetTransformMatrix() const
{
	return GetPositionMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

glm::mat4 Transform::GetPositionMatrix() const { return translate(MATRIX4_IDENTITY, this->position); }
mat4 Transform::GetRotationMatrix() const { return mat4_cast(this->rotation); }
glm::mat4 Transform::GetScaleMatrix() const { return glm::scale(MATRIX4_IDENTITY, this->scale); }

vec3 Transform::GetForward() const { return this->rotation * V3_FORWARD; }
vec3 Transform::GetUp() const { return this->rotation * V3_UP; }
vec3 Transform::GetRight() const { return this->rotation * V3_RIGHT; }

void Transform::Translate(vec3 value, bool isWorld)
{
	if (!isWorld)
		value = GetRotationMatrix() * vec4(value, 1.0f);
	this->position = value + this->position;

}

void Transform::Rotate(vec3 axis, float angle, bool isWorld)
{
	if (isWorld)
	{
		this->rotation = rotate(QUAT_IDENTITY, radians(angle), axis) * this->rotation;
	}
	else
	{
		this->rotation = (glm::quat)rotate(this->rotation, radians(angle), axis);
	}
}

void Transform::LookAt(vec3 target, vec3 up)
{
	auto forward = normalize(position - target);
	this->rotation = glm::quatLookAt(forward, up);
}

Transform::Transform()
{
	this->position = V3_ZERO;
	this->rotation = QUAT_IDENTITY;
	this->scale = V3_ONE;
}

Transform::Transform(vec3 p, glm::quat r, vec3 s) :position(p), rotation(r), scale(s) {}

