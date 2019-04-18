//#include "Mathf.h"
//
//
//
//using namespace glm;
//
//// 转换到GLM结构
////vec2 toGLM(const Vector2& v)
////{
////	return vec2(v.x, v.y);
////}
////Vector2 convert(const vec2& v)
////{
////	return Vector2(v.x, v.y);
////}
////vec3 toGLM(const Vector3& v)
////{
////	return vec3(v.x, v.y, v.z);
////}
////Vector3 convert(const vec3& v)
////{
////	return Vector3(v.x, v.y, v.z);
////}
////quat toGLM(const Quaternion& q)
////{
////	return quat(q.x, q.y, q.z, q.w);
////}
////Quaternion convert(const quat& q)
////{
////	return Quaternion(q.x, q.y, q.z, q.w);
////}
//
//// ----- Vector2 --------------------------------
//const Vector2 Vector2::right = Vector2(1.0f, 0.0f);
//const Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
//const Vector2 Vector2::down = Vector2(0.0f, -1.0f);
//const Vector2 Vector2::up = Vector2(0.0f, 1.0f);
//const Vector2 Vector2::one = Vector2(1.0f, 1.0f);
//const Vector2 Vector2::zero = Vector2(0.0f, 0.0f);
//
//float Vector2::Angle(Vector2 & from, Vector2 & to)
//{
//	float single;
//	float single1 = sqrt(from.sqrMagnitude()*to.sqrMagnitude());
//	if (single1 >= 1E-15f)
//	{
//		float single2 = clamp(Dot(from, to) / single1, -1.0f, 1.0f);
//		single = degrees(acos(single2));
//	}
//	else
//	{
//		single = 0.0f;
//	}
//	return single;
//}
//Vector2 Vector2::ClampMagnitude(Vector2 & vector, float maxLength)
//{
//	return (vector.sqrMagnitude() <= maxLength * maxLength) ? vector : vector.normalized() * maxLength;
//}
//float Vector2::Distance(Vector2 & a, Vector2 & b)
//{
//	return (a - b).magnitude();
//}
//float Vector2::Dot(Vector2 & lhs, Vector2 & rhs)
//{
//	return lhs.x * rhs.x + lhs.y * rhs.y;
//}
//Vector2 Vector2::Lerp(Vector2 & a, Vector2 & b, float t)
//{
//	t = clamp(t, 0.0f, 1.0f);
//	return LerpUnclamped(a, b, t);
//}
//Vector2 Vector2::LerpUnclamped(Vector2 & a, Vector2 & b, float t)
//{
//	return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);;
//}
//Vector2 Vector2::Max(Vector2 & lhs, Vector2 & rhs)
//{
//	return Vector2(max(lhs.x, rhs.x), max(lhs.y, rhs.y));
//}
//Vector2 Vector2::Min(Vector2 & lhs, Vector2 & rhs)
//{
//	return Vector2(min(lhs.x, rhs.x), min(lhs.y, rhs.y));
//}
//Vector2 Vector2::MoveTowards(Vector2 & current, Vector2 & target, float maxDistanceDelta)
//{
//	Vector2 vector2;
//	Vector2 vector21 = target - current;
//	float single = vector21.magnitude();
//	vector2 = (single <= maxDistanceDelta || single == 0.0f ? target : current + ((vector21 / single) * maxDistanceDelta));
//	return vector2;
//}
//Vector2 Vector2::Perpendicular(Vector2 & inDirection)
//{
//	return Vector2(-inDirection.y, inDirection.x);
//}
//Vector2 Vector2::Reflect(Vector2& inDirection, Vector2& inNormal)
//{
//	return (-2.0f * Dot(inNormal, inDirection) * inNormal) + inDirection;
//}
//float Vector2::SignedAngle(Vector2& from, Vector2& to)
//{
//	float single = Angle(from, to);
//	float single1 = sign(from.x * to.y - from.y * to.x);
//	return single * single1;
//}
//
//Vector2 Vector2::normalized() const
//{
//	return *(const_cast<Vector2*>(this)) / this->magnitude();
//}
//float Vector2::sqrMagnitude() const
//{
//	return Dot(*(const_cast<Vector2*>(this)), *(const_cast<Vector2*>(this)));
//}
//float Vector2::magnitude() const
//{
//	return sqrt(Dot(*(const_cast<Vector2*>(this)), *(const_cast<Vector2*>(this))));
//}
//glm::vec2 Vector2::toGLM() const
//{
//	return vec2(x, y);
//}
//
//Vector2::Vector2(Vector2 const & v) :x(v.x), y(v.y) {}
////Vector2::Vector2(float scalar) : x(scalar), y(scalar) {}
//Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}
//Vector2::Vector2(Vector3 v) : x(v.x), y(v.y) {}
//Vector2::Vector2(glm::vec2 v) : x(v.x), y(v.y) {}
//
//Vector2 Vector2::operator+()
//{
//	return *this;
//}
//Vector2 Vector2::operator-()
//{
//	return Vector2(-x, -y);
//}
//Vector2 Vector2::operator+(const Vector2 & v)
//{
//	return Vector2(x + v.x, y + v.y);
//}
//Vector2 Vector2::operator-(const Vector2 & v)
//{
//	return Vector2(x - v.x, y - v.y);
//}
//Vector2 operator*(const Vector2 & v, const float & f)
//{
//	return Vector2(v.x * f, v.y * f);
//}
//Vector2 operator*(const float & f, const Vector2 & v)
//{
//	return Vector2(f * v.x, f * v.y);
//}
//Vector2 operator/(const Vector2& v, const float& f)
//{
//	return Vector2(v.x / f, v.y / f);
//}
//Vector2 operator/(const float & f, const Vector2 & v)
//{
//	return Vector2(f / v.x, f / v.y);
//}
//bool Vector2::operator==(const Vector2 & v)
//{
//	return toGLM() == v.toGLM();
//}
//bool Vector2::operator!=(const Vector2 & v)
//{
//	return toGLM() != v.toGLM();
//}
//float & Vector2::operator[](int i)
//{
//	return toGLM()[i];
//}
//
//
//// ----- Vector3 --------------------------------
//const Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
//const Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
//const Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);
//const Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
//const Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
//const Vector3 Vector3::back = Vector3(0.0f, 0.0f, -1.0f);
//const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
//const Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);
//
//float Vector3::Angle(Vector3& from, Vector3& to)
//{
//	float single;
//	float single1 = sqrt(from.sqrMagnitude() * to.sqrMagnitude());
//	if (single1 >= 1E-15f)
//	{
//		float single2 = clamp(Dot(from, to) / single1, -1.0f, 1.0f);
//		single = degrees(acos(single2));
//	}
//	else
//	{
//		single = 0.0f;
//	}
//	return single;
//}
//Vector3 Vector3::Cross(Vector3& lhs, Vector3& rhs)
//{
//	return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
//}
//float Vector3::Distance(Vector3& a, Vector3& b)
//{
//	return (a - b).magnitude();
//}
//float Vector3::Dot(Vector3& lhs, Vector3& rhs)
//{
//	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
//}
//Vector3 Vector3::Lerp(Vector3& a, Vector3& b, float t)
//{
//	t = clamp(t, 0.0f, 1.0f);
//	return LerpUnclamped(a, b, t);
//}
//Vector3 Vector3::LerpUnclamped(Vector3& a, Vector3& b, float t)
//{
//	return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
//}
//Vector3 Vector3::Max(Vector3& lhs, Vector3& rhs)
//{
//	return  Vector3(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z));
//}
//Vector3 Vector3::Min(Vector3& lhs, Vector3& rhs)
//{
//	return  Vector3(min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z));
//}
//Vector3 Vector3::MoveTowards(Vector3& current, Vector3& target, float maxDistanceDelta)
//{
//	Vector3 vector31 = target - current;
//	float single = vector31.magnitude();
//	return (single <= maxDistanceDelta || single < 1.401298E-45f ? target : current + ((vector31 / single) * maxDistanceDelta));
//}
//Vector3 Vector3::Project(Vector3& vector, Vector3& onNormal)
//{
//	float single = Dot(onNormal, onNormal);
//	return single > 0.0f ? (onNormal * Dot(vector, onNormal)) / single : Vector3::zero;
//}
//Vector3 Vector3::ProjectOnPlane(Vector3& vector, Vector3& planeNormal)
//{
//	return vector - Project(vector, planeNormal);
//}
//Vector3 Vector3::Reflect(Vector3& inDirection, Vector3& inNormal)
//{
//	return (-2.0f * Dot(inNormal, inDirection) * inNormal) + inDirection;
//}
//float Vector3::SignedAngle(Vector3& from, Vector3& to, Vector3& axis)
//{
//	float single = Angle(from, to);
//	auto v = Cross(from, to);
//	float single1 = sign(Dot(axis, v));
//	return single * single1;
//}
//
//Vector3 Vector3::normalized() const
//{
//	return *(const_cast<Vector3*>(this)) / this->magnitude();
//}
//float Vector3::sqrMagnitude() const
//{
//	return Dot(*(const_cast<Vector3*>(this)), *(const_cast<Vector3*>(this)));
//}
//float Vector3::magnitude() const
//{
//	return sqrt(Dot(*(const_cast<Vector3*>(this)), *(const_cast<Vector3*>(this))));
//}
//glm::vec3 Vector3::toGLM() const
//{
//	return vec3(x, y, z);
//}
//
//Vector3::Vector3(Vector3 const & v) :x(v.x), y(v.y), z(v.z) {}
//Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//Vector3::Vector3(Vector2 v) : x(v.x), y(v.y), z(0.0f) {}
//Vector3::Vector3(glm::vec3 v) : x(v.x), y(v.y), z(v.z) {}
//
//Vector3 Vector3::operator+()
//{
//	return *this;
//}
//Vector3 Vector3::operator-()
//{
//	return Vector3(-x, -y, -z);
//}
//Vector3 Vector3::operator+(const Vector3 & v)
//{
//	return Vector3(x + v.x, y + v.y, z + v.z);
//}
//Vector3 Vector3::operator-(const Vector3 & v)
//{
//	return Vector3(x - v.x, y - v.y, z - v.z);
//}
//Vector3 operator*(const Vector3 & v, const float & f)
//{
//	return Vector3(v.x*f, v.y*f, v.z*f);
//}
//Vector3 operator*(const float & f, const Vector3 & v)
//{
//	return Vector3(f*v.x, f*v.y, f*v.z);
//}
//Vector3 operator/(const Vector3& v, const float& f)
//{
//	return Vector3(v.x / f, v.y / f, v.z / f);
//}
//Vector3 operator/(const float & f, const Vector3 & v)
//{
//	return Vector3(f / v.x, f / v.y, f / v.y);
//}
//bool Vector3::operator==(const Vector3 & v)
//{
//	return toGLM() == v.toGLM();
//}
//bool Vector3::operator!=(const Vector3 & v)
//{
//	return toGLM() != v.toGLM();
//}
//float & Vector3::operator[](int i)
//{
//	return toGLM()[i];
//}
//
//
//// ----- Quaternion --------------------------------
//const Quaternion Quaternion::identity = Quaternion();
//
//float Quaternion::Angle(Quaternion & a, Quaternion & b)
//{
//	float single = Dot(a, b);
//	return (!(single > 0.999999f) ? degrees(acos(min(abs(single), 1.0f)) * 2.0f) : 0.0f);
//}
//Quaternion Quaternion::AngleAxis(float & angle, Vector3 & axis)
//{
//	return (Quaternion)(glm::angleAxis(radians(angle), axis.toGLM()));
//}
//float Quaternion::Dot(Quaternion & a, Quaternion & b)
//{
//	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
//}
//Quaternion Quaternion::Euler(Vector3 & euler)
//{
//	return (Quaternion)(quat(euler.toGLM()));
//}
//Quaternion Quaternion::FromToRotation(Vector3 & fromDirection, Vector3 & toDirection)
//{
//	return (Quaternion)(quat(fromDirection.toGLM(), toDirection.toGLM()));
//}
//Quaternion Quaternion::Inverse(Quaternion & rotation)
//{
//	return (Quaternion)(inverse(rotation.toGLM()));
//}
//Quaternion Quaternion::Lerp(Quaternion & a, Quaternion & b, float t)
//{
//	t = clamp(t, 0.0f, 1.0f);
//	return LerpUnclamped(a, b, t);
//}
//Quaternion Quaternion::LerpUnclamped(Quaternion & a, Quaternion & b, float t)
//{
//	return a * (1.0f - t) + (b * t);
//}
//Quaternion Quaternion::LookRotation(Vector3 & forward, Vector3 upwards)
//{
//	return (Quaternion)(quatLookAt(forward.toGLM(), upwards.toGLM()));
//}
//
//Vector3 Quaternion::eulerAngles() const
//{
//	return (Vector3)glm::eulerAngles(toGLM());
//}
//Quaternion Quaternion::normalized() const
//{
//	Quaternion quaternion;
//	auto q = *this;
//	float single = sqrt(Dot(q, q));
//	quaternion = (single > 0.0f ? Quaternion(q.x / single, q.y / single, q.z / single, q.w / single) : Quaternion::identity);
//	return quaternion;
//}
//glm::quat Quaternion::toGLM() const
//{
//	return quat(x, y, z, w);
//}
//
//Quaternion::Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
//Quaternion::Quaternion(const Quaternion & q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
//Quaternion::Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
//Quaternion::Quaternion(glm::quat q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
//
//Vector3 operator*(const Quaternion & q, const Vector3 & point)
//{
//	return (Vector3)(q.toGLM() * point.toGLM());
//}
//Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
//{
//	return Quaternion(
//		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
//		lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
//		lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
//		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
//}
//bool Quaternion::operator==(const Quaternion& q)
//{
//	return toGLM() == q.toGLM();
//}
//bool Quaternion::operator!=(const Quaternion& q)
//{
//	return toGLM() != q.toGLM();;
//}
//Quaternion Quaternion::operator+(const Quaternion & q)
//{
//	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
//}
//Quaternion Quaternion::operator*(const float & f)
//{
//	return Quaternion(x*f, y*f, z*f, w*f);
//}
//
