#pragma once

#include <glm/glm.hpp>
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
//#include <glm/gtc/matrix_transform.hpp>

namespace OpenGL_Core
{
    // 检查：是否常量参数 是否引用参数 是否常量函数

    struct Vector2;
    struct Vector3;
    struct Vector4;
    struct Quaternion;
    struct Matrix4x4;


    class Math
    {
    public:
        static const float Deg2Rad;
        static const float Epsilon;
        static const float PI;
        static const float Rad2Deg;

        inline static float Abs(const float& value) { return glm::abs(value); }
        inline static float Mod(const float& x, const float& y) { return glm::mod(x, y); }
        inline static float Sign(const float& value) { return glm::sign(value); }

        inline static float Exp(const float& power) { return glm::exp(power); }
        inline static float Exp2(const float& power) { return glm::exp2(power); }
        inline static float Log(const float& value) { return glm::log(value); }
        inline static float Pow(const float& value, const float& power) { return glm::pow(value, power); }
        inline static float Sqrt(const float& value) { return glm::sqrt(value); }

        inline static bool Approximately(const float& a, const float& b) { return Abs(a - b) <= Epsilon; }
        inline static bool Min(const float& a, const float& b) { return glm::min(a, b); }
        inline static bool Max(const float& a, const float& b) { return glm::max(a, b); }

        inline static bool Lerp(const float& a, const float& b, const float& t) { return glm::mix(a, b, t); }
        inline static bool SmoothStep(const float& a, const float& b, const float& t) { return glm::smoothstep(a, b, t); }

        inline static float Floor(const float& value) { return glm::floor(value); }
        inline static float Ceil(const float& value) { return glm::ceil(value); }
        inline static float Round(const float& value) { return glm::round(value); }
        inline static float Clamp(const float& value, const float& min, const float& max) { return glm::clamp(value, min, max); }
        inline static float Clamp01(const float& value) { return glm::clamp(value, 0.0f, 1.0f); }

        inline static float Asin(const float& value) { return glm::asin(value); }
        inline static float Acos(const float& value) { return glm::acos(value); }
        inline static float Atan(const float& value) { return glm::atan(value); }
        inline static float Atan2(const float& y, const float& x) { return std::atan2(y, x); }
        inline static float Sin(const float& value) { return glm::sin(value); }
        inline static float Cos(const float& value) { return glm::cos(value); }
        inline static float Tan(const float& value) { return glm::tan(value); }
    };


    struct Vector2 : public glm::vec2
    {
        static const Vector2 Zero;
        inline const float* GetPtr() const { return &((*this).x); }

        Vector2() :glm::vec2() {}
        Vector2(const glm::vec2& v) :glm::vec2(v) {}
        Vector2(const float& x, const  float& y) :glm::vec2(x, y) {}

        bool operator ==(const Vector2& v) const { return  Math::Approximately((*this).x, v.x) && Math::Approximately((*this).y, v.y); }

    };
    struct Vector3 : public glm::vec3
    {
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Right;
        static const Vector3 Left;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Forward;
        static const Vector3 Back;

        static float Angle(const Vector3& from, const Vector3& to)
        {
            if (from == Vector3::Zero || to == Vector3::Zero)return 0.0f;
            Math::Acos(Math::Clamp(Dot(from.GetNormalized(), to.GetNormalized()), -1.0f, 1.0f));
        }
        static float Cross(const Vector3& lhs, const Vector3& rhs);
        static float Distance(const Vector3& a, const Vector3& b);
        inline static float Dot(const Vector3& lhs, const Vector3& rhs)
        {
            auto temp = lhs * rhs;
            return temp.x + temp.y + temp.z;
        }
        static float Lerp(const Vector3& a, const Vector3& b, const float& t);

        inline const float* GetPtr() const { return &((*this).x); }
        inline float GetMagnitude() const { return Math::Sqrt(Dot(*this, *this)); }
        inline Vector3 GetNormalized() const { return *this / GetMagnitude(); }
        inline float GetSqrMagnitude() const { return Dot(*this, *this); }

        Vector3() :glm::vec3() {}
        Vector3(const glm::vec3& v) :glm::vec3(v) {}
        Vector3(const float& x, const  float& y) :glm::vec3(x, y, 0.0f) {}
        Vector3(const float& x, const  float& y, const  float& z) :glm::vec3(x, y, z) {}
        //Vector3(const Vector4& v) :vec3(v.x, v.y, v.z) {}

        Vector3 operator -() const { return -static_cast<glm::vec3>(*this); }
        Vector3 operator +(const Vector3& v) const { return static_cast<glm::vec3>(*this) + v; }
        Vector3 operator -(const Vector3& v) const { return static_cast<glm::vec3>(*this) - v; }
        Vector3 operator *(const float& value) const { return static_cast<glm::vec3>(*this) * value; }
        Vector3 operator *(const Vector3& v) const { return static_cast<glm::vec3>(*this) * v; }
        Vector3 operator /(const float& value) const { return static_cast<glm::vec3>(*this) / value; }
        Vector3 operator /(const Vector3& v) const { return static_cast<glm::vec3>(*this) / v; }
        bool operator ==(const Vector3& v) const { return  Math::Approximately((*this).x, v.x) && Math::Approximately((*this).y, v.y) && Math::Approximately((*this).z, v.z); }
        bool operator !=(const Vector3& v) const { return  !((*this) == v); }

        operator Vector2() const { return Vector2(x, y); }
    };

    struct Vector4 : public glm::vec4
    {
        static const Vector4 Zero;
        static const Vector4 One;

        inline const float* GetPtr() const { return &((*this).x); }

        Vector4() :glm::vec4() {}
        Vector4(const glm::vec4& v) :glm::vec4(v) { }
        Vector4(const float& x, const float& y) :glm::vec4(x, y, 0.0f, 0.0f) { }
        Vector4(const float& x, const float& y, const  float& z) :glm::vec4(x, y, z, 0.0f) { }
        Vector4(const float& x, const float& y, const  float& z, const  float& w) :glm::vec4(x, y, z, w) { }
        Vector4(const Vector2& v, const  float& z, const float& w) :glm::vec4(v.x, v.y, z, w) { }
        Vector4(const Vector3& v, const float& w) :glm::vec4(v.x, v.y, v.z, w) { }

        operator Vector3() const { return Vector3(x, y, z); }

        Vector4 operator -() const { return -static_cast<glm::vec4>(*this); }
        Vector4 operator +(const Vector4& v) const { return static_cast<glm::vec4>(*this) + v; }
        Vector4 operator -(const Vector4& v) const { return static_cast<glm::vec4>(*this) - v; }
        Vector4 operator *(const float& value) const { return static_cast<glm::vec4>(*this) * value; }
        Vector4 operator *(const Vector4& v) const { return static_cast<glm::vec4>(*this) * v; }
        Vector4 operator /(const float& value) const { return static_cast<glm::vec4>(*this) / value; }
        Vector4 operator /(const Vector4& v) const { return static_cast<glm::vec4>(*this) / v; }
        bool operator ==(const Vector4& v) const { return  Math::Approximately((*this).x, v.x) && Math::Approximately((*this).y, v.y) && Math::Approximately((*this).z, v.z) && Math::Approximately((*this).w, v.w); }
        bool operator !=(const Vector4& v) const { return  !((*this) == v); }
    };

    struct Quaternion : public glm::quat
    {
        static const Quaternion Identity;

        static Quaternion AngleAxis(const float& angle, const Vector3& axis) { return angleAxis(angle * Math::Deg2Rad, axis); }
        static Quaternion LookRotation(const Vector3& forward, const Vector3& upwards = Vector3::Up) { return quatLookAt(forward, upwards); }

        inline const float* GetPtr() const { return &((*this)[0]); }

        inline Vector3 GetEulerAngles() const { return glm::eulerAngles(*this) * Math::Rad2Deg; }
        Quaternion Inverse() { return glm::inverse(*this); }

        Quaternion() :glm::quat(1.0f, 0.0f, 0.0f, 0.0f) {}
        Quaternion(const glm::quat& q) :glm::quat(q) {}
        Quaternion(const float& x, const  float& y, const  float& z, const  float& w) :glm::quat(w, x, w, z) {}
        Quaternion(const float& x, const  float& y, const  float& z) :glm::quat(glm::vec3(x, y, z)* Math::Deg2Rad) {}
        Quaternion(const Vector3& v) :glm::quat(v* Math::Deg2Rad) {}

        Quaternion operator * (const Quaternion& q) const { return static_cast<glm::quat>(*this) * q; }
        Vector3 operator * (const Vector3& v) const { return static_cast<glm::quat>(*this) * v; }
    };

    struct Matrix4x4 : public glm::mat4
    {
        static const Matrix4x4 Identity;
        static const Matrix4x4 XInverse;
        static const Matrix4x4 YInverse;
        static const Matrix4x4 ZInverse;

        static Matrix4x4 Translate(Vector3 v) { return glm::translate(Matrix4x4::Identity, v); }
        static Matrix4x4 Rotate(Quaternion q) { return glm::mat4_cast(q); }
        static Matrix4x4 Scale(Vector3 s) { return glm::scale(Matrix4x4::Identity, s); }
        static Matrix4x4 LookAt(Vector3 from, Vector3 to, Vector3 up = Vector3::Up)
        {
            return glm::lookAt(from, to, up);
        };
        /// <summary>
        /// 正交投影
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <param name="bottom"></param>
        /// <param name="top"></param>
        /// <param name="zNear"></param>
        /// <param name="zFar"></param>
        /// <returns></returns>
        static Matrix4x4 Ortho(const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar)
        {
            return glm::ortho(left, right, bottom, top, zNear, zFar);
        }
        static Matrix4x4 Frustum(const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar);
        /// <summary>
        /// 透视投影
        /// </summary>
        /// <param name="fovy"></param>
        /// <param name="aspect"></param>
        /// <param name="zNear"></param>
        /// <param name="zFar"></param>
        /// <returns></returns>
        static Matrix4x4 Perspective(const float& fovy, const float& aspect, const float& zNear, const float& zFar)
        {
            return glm::perspective(Math::Deg2Rad * fovy, aspect, zNear, zFar);
        }

        inline const float* GetPtr() const { return &((*this)[0].x); }

        Matrix4x4 Inverse() { return glm::inverse(*this); }
        Matrix4x4 Transpose() { return glm::transpose(*this); }

        Quaternion QuaternionCast() { return glm::quat_cast(*this); }

        Matrix4x4() :glm::mat4(1.0f) {}
        Matrix4x4(const glm::mat4& m) :glm::mat4(m) {}
        Matrix4x4(
            float const& x0, float const& y0, float const& z0, float const& w0,
            float const& x1, float const& y1, float const& z1, float const& w1,
            float const& x2, float const& y2, float const& z2, float const& w2,
            float const& x3, float const& y3, float const& z3, float const& w3)
            :glm::mat4(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3) {}

        Matrix4x4 operator * (const Matrix4x4& m) const { return static_cast<glm::mat4>(*this) * m; }
        Vector4 operator * (const Vector4& v) const { return static_cast<glm::mat4>(*this) * v; }
    };

}