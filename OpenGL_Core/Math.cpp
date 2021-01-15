#include "Math.h"

namespace OpenGL_Core
{
	const float Math::Deg2Rad = 0.01745329251994329576923690768489f;
	const float Math::Epsilon = glm::epsilon<float>();
	const float Math::PI = 3.14159265358979323846264338327950288f;
	const float Math::Rad2Deg = 57.295779513082320876798154814105f;

	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);
	const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	const Quaternion Quaternion::Identity = Quaternion();
	const Matrix4x4 Matrix4x4::Identity = Matrix4x4();
	const Matrix4x4 Matrix4x4::XInverse = Matrix4x4(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const Matrix4x4 Matrix4x4::YInverse = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const Matrix4x4 Matrix4x4::ZInverse = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}