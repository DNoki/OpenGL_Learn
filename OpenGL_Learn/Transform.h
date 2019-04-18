#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//#include "Mathf.h"
#define V3_RIGHT glm::vec3(1.0f, 0.0f, 0.0f)
#define V3_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define V3_FORWARD glm::vec3(0.0f, 0.0f, 1.0f)
#define V3_ZERO glm::vec3(0.0f, 0.0f, 0.0f)
#define V3_ONE glm::vec3(1.0f, 1.0f, 1.0f)

#define QUAT_IDENTITY glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
#define MATRIX4_IDENTITY glm::mat4(1.0f)

class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	//inline glm::vec3 GetPosition() const { return this->position; }
	//inline glm::quat GetRotation() const { return this->rotation; }
	inline glm::vec3 GetEulerAngles() const { return degrees(eulerAngles(this->rotation)); }
	//inline glm::vec3 GetScale() const { return this->scale; }
	//inline void SetPosition(glm::vec3 v) { this->position = v; }
	//inline void SetRotation(glm::quat q) { this->rotation = q; }
	inline void SetEulerAngles(glm::vec3 e) { this->rotation = glm::quat(radians(e)); }
	//inline void SetScale(glm::vec3 s) { this->scale = s; }

	glm::mat4 GetTransformMatrix() const;
	glm::mat4 GetPositionMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void Translate(glm::vec3 value, bool isWorld = false);
	void Rotate(glm::vec3 axis, float angle, bool isWorld = false);
	void LookAt(glm::vec3 target, glm::vec3 up = V3_UP);

	Transform();
	Transform(glm::vec3 p, glm::quat r, glm::vec3 s);

private:	
};

/*
.hpp的相关的使用规则

a)不可包含全局对象和全局函数
b)类之间不可循环调用
c)不可使用静态成员
*/