//#pragma once
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//
//struct Vector2;
//struct Vector3;
//struct Vector4;
//struct Quaternion;
//
//struct Vector2
//{
//public:
//	static const Vector2 right;
//	static const Vector2 left;
//	static const Vector2 down;
//	static const Vector2 up;
//	static const Vector2 one;
//	static const Vector2 zero;
//
//	// 返回from和to之间的无符号角度（以度为单位）。
//	static float Angle(Vector2& from, Vector2& to);
//	// 返回向量的副本，其大小被限制为maxLength。
//	static Vector2 ClampMagnitude(Vector2& vector, float maxLength);
//	static float Distance(Vector2& a, Vector2& b);
//	static float Dot(Vector2& lhs, Vector2& rhs);
//	static Vector2 Lerp(Vector2& a, Vector2& b, float t);
//	static Vector2 LerpUnclamped(Vector2& a, Vector2& b, float t);
//	static Vector2 Max(Vector2& lhs, Vector2& rhs);
//	static Vector2 Min(Vector2& lhs, Vector2& rhs);
//	// 将点移向目标。
//	static Vector2 MoveTowards(Vector2& current, Vector2& target, float maxDistanceDelta);
//	// 返回垂直于此2D矢量的2D矢量。对于正Y轴向上的2D坐标系，结果始终沿逆时针方向旋转90度。
//	static Vector2 Perpendicular(Vector2& inDirection);
//	static Vector2 Reflect(Vector2& inDirection, Vector2& inNormal);
//	// 返回from和to之间的有符号角度（以度为单位）。
//	static float SignedAngle(Vector2& from, Vector2& to);
//
//	float x, y;
//	// 返回此向量，其大小为1。
//	Vector2 normalized() const;
//	// 返回此向量的平方长度
//	float sqrMagnitude() const;
//	// 返回此向量的长度
//	float magnitude() const;
//	glm::vec2 toGLM() const;
//
//	// -- Implicit basic constructors --
//	Vector2() = default;
//	Vector2(Vector2 const& v);
//	// -- Explicit basic constructors --
//	//explicit Vector2(float scalar);
//	Vector2(float x, float y);
//	// -- Conversion constructors --
//	template<typename A, typename B>
//	inline Vector2(A _x, B _y) : x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}
//	explicit Vector2(Vector3 v);
//	explicit Vector2(glm::vec2 v);
//
//	// operators
//	Vector2 operator +();
//	Vector2 operator -();
//	Vector2 operator +(const Vector2& v);
//	Vector2 operator -(const Vector2& v);
//	friend Vector2 operator *(const Vector2& v, const float& f);
//	friend Vector2 operator *(const float& f, const Vector2& v);
//	friend Vector2 operator /(const Vector2& v, const float& f);
//	friend Vector2 operator /(const float& f, const Vector2& v);
//	bool operator ==(const Vector2& v);
//	bool operator !=(const Vector2& v);
//	float& operator [](int i);
//};
//
//struct Vector3
//{
//public:
//	static const Vector3 right;
//	static const Vector3 left;
//	static const Vector3 down;
//	static const Vector3 up;
//	static const Vector3 forward;
//	static const Vector3 back;
//	static const Vector3 one;
//	static const Vector3 zero;
//
//	static float Angle(Vector3& from, Vector3& to);
//	static Vector3 Cross(Vector3& lhs, Vector3& rhs);
//	static float Distance(Vector3& a, Vector3& b);
//	static float Dot(Vector3& lhs, Vector3& rhs);
//	static Vector3 Lerp(Vector3& a, Vector3& b, float t);
//	static Vector3 LerpUnclamped(Vector3& a, Vector3& b, float t);
//	static Vector3 Max(Vector3& lhs, Vector3& rhs);
//	static Vector3 Min(Vector3& lhs, Vector3& rhs);
//	static Vector3 MoveTowards(Vector3& current, Vector3& target, float maxDistanceDelta);
//	Vector3 Project(Vector3& vector, Vector3& onNormal);
//	Vector3 ProjectOnPlane(Vector3& vector, Vector3& planeNormal);
//	Vector3 Reflect(Vector3& inDirection, Vector3& inNormal);
//	static float SignedAngle(Vector3& from, Vector3& to, Vector3& axis);
//
//	float x, y, z;
//	// 返回此向量，其大小为1。
//	Vector3 normalized() const;
//	// 返回此向量的平方长度
//	float sqrMagnitude() const;
//	// 返回此向量的长度
//	float magnitude() const;
//	glm::vec3 toGLM() const;
//
//	// -- Implicit basic constructors --
//	Vector3() = default;
//	Vector3(Vector3 const& v);
//	// -- Explicit basic constructors --
//	Vector3(float x, float y, float z);
//	// -- Conversion constructors --
//	template<typename X, typename Y, typename Z>
//	inline Vector3(X _x, Y _y, Z _z) : x(static_cast<float>(_x)), y(static_cast<float>(_y)), z(static_cast<float>(_z)) {}
//	Vector3(Vector2 v);
//	explicit Vector3(glm::vec3 v);
//
//	// operators
//	Vector3 operator +();
//	Vector3 operator -();
//	Vector3 operator +(const Vector3& v);
//	Vector3 operator -(const Vector3& v);
//	friend Vector3 operator *(const Vector3& v, const float& f);
//	friend Vector3 operator *(const float& f, const Vector3& v);
//	friend Vector3 operator /(const Vector3& v, const float& f);
//	friend Vector3 operator /(const float& f, const Vector3& v);
//	bool operator ==(const Vector3& v);
//	bool operator !=(const Vector3& v);
//	float& operator [](int i);
//};
//
//struct Quaternion
//{
//public:
//	static const Quaternion identity;
//
//	static float Angle(Quaternion& a, Quaternion& b);
//	static Quaternion AngleAxis(float& angle, Vector3& axis);
//	static float Dot(Quaternion& a, Quaternion& b);
//	static Quaternion Euler(Vector3& euler);
//	static Quaternion FromToRotation(Vector3& fromDirection, Vector3& toDirection);
//	static Quaternion Inverse(Quaternion& rotation);
//	static Quaternion Lerp(Quaternion& a, Quaternion& b, float t);
//	static Quaternion LerpUnclamped(Quaternion& a, Quaternion& b, float t);
//	static Quaternion LookRotation(Vector3& forward, Vector3 upwards = Vector3(0.0f, 1.0f, 0.0f));
//
//	float x, y, z, w;
//	Vector3 eulerAngles() const;
//	Quaternion normalized() const;
//	glm::quat toGLM() const;
//
//	// constructors
//	Quaternion();
//	Quaternion(const Quaternion& q);
//	Quaternion(float x, float y, float z, float w);
//	template<typename X, typename Y, typename Z, typename W>
//	inline Quaternion(X _x, Y _y, Z _z, W _w) : x(static_cast<float>(_x)), y(static_cast<float>(_y)), z(static_cast<float>(_z)), w(static_cast<float>(_w)) {}
//	explicit Quaternion(glm::quat q);
//
//	// operators
//	friend Vector3 operator *(const Quaternion& q, const Vector3& point);
//	friend Quaternion operator *(const Quaternion& lhs, const Quaternion& rhs);
//	bool operator ==(const Quaternion& q);
//	bool operator !=(const Quaternion& q);
//
//private:
//	Quaternion operator +(const Quaternion& q);
//	Quaternion operator *(const float& f);
//};
//
//
////struct Matrix4x4
////{
////public:
////	float m00, m10, m20, m30;
////	float m01, m11, m21, m31;
////	float m02, m12, m22, m32;
////	float m03, m13, m23, m33;
////};