#include "pch.h"

#include "Math.h"

namespace OpenGL_Core
{
    using namespace glm;

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

    float Vector2::Distance(const Vector2& a, const Vector2& b)
    {
        return distance((vec2)a, (vec2)b);
    }
    float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
    {
        auto temp = lhs * rhs;
        return temp.x + temp.y;
    }
    Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, const float& t)
    {
        return mix((vec2)a, (vec2)b, t);
    }

    float Vector3::Angle(const Vector3& from, const Vector3& to)
    {
        if (from == Vector3::Zero || to == Vector3::Zero) return 0.0f;
        return Math::Acos(Math::Clamp(Dot(from.GetNormalized(), to.GetNormalized()), -1.0f, 1.0f));
    }
    Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
    {
        return cross((vec3)lhs, (vec3)rhs);
    }
    float Vector3::Distance(const Vector3& a, const Vector3& b)
    {
        return distance((vec3)a, (vec3)b);
    }
    float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
    {
        auto temp = lhs * rhs;
        return temp.x + temp.y + temp.z;
    }
    Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, const float& t)
    {
        return mix((vec3)a, (vec3)b, t);
    }
}