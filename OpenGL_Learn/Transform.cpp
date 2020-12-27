#include "Transform.h"

#include "GameObject.h"

namespace OpenGL_Learn
{
    inline Vector3 Transform::GetPosition(bool isWorld) const
    {
        if (_parent == nullptr || !isWorld)
            return LocalPosition;
        else
            return _parent->GetTransformMatrix() * Vector4(LocalPosition, 1.0f);
    }
    void Transform::SetPosition(const Vector3& pos, bool isWorld)
    {
        if (_parent == nullptr || !isWorld)
            LocalPosition = pos;
        else
            LocalPosition = _parent->GetTransformMatrix().Inverse() * Vector4(pos, 1.0f);
    }
    inline Quaternion Transform::GetRotation(bool isWorld) const
    {
        return Quaternion(GetEulerAngles(isWorld));
        //if (Parent == nullptr || !isWorld)
        //    return Quaternion(LocalEulerAngles);
        //else
        //    return Parent->GetRotation() * Quaternion(LocalEulerAngles);
    }
    void Transform::SetRotation(const Quaternion& rot, bool isWorld)
    {
        SetEulerAngles(rot.GetEulerAngles(), isWorld);
        //if (Parent == nullptr || !isWorld)
        //    LocalRotation = rot;
        //else
        //    LocalRotation = Parent->GetRotation().Inverse() * rot;
    }
    Vector3 Transform::GetEulerAngles(bool isWorld) const
    {
        if (_parent == nullptr || !isWorld)
            return LocalEulerAngles;
        else
            return (_parent->GetRotation() * Quaternion(LocalEulerAngles)).GetEulerAngles();
        //return GetRotation(isWorld).GetEulerAngles();
    }
    void Transform::SetEulerAngles(const Vector3& e, bool isWorld)
    {
        if (_parent == nullptr || !isWorld)
            LocalEulerAngles = e;
        else
            LocalEulerAngles = (_parent->GetRotation().Inverse() * Quaternion(e)).GetEulerAngles();
        //SetRotation(Quaternion(e), isWorld);
    }
    Vector3 Transform::GetLossyScale() const
    {
        if (_parent == nullptr)
            return LocalScale;
        else
            return _parent->GetLossyScale() * LocalScale;
    }
    void Transform::SetLossyScale(const Vector3& s)
    {
        if (_parent == nullptr)
            LocalScale = s;
        else
            LocalScale = s / _parent->GetLossyScale();
    }

    Matrix4x4 Transform::GetTransformMatrix() const
    {
        return GetPositionMatrix() * GetRotationMatrix() * GetScaleMatrix();
    }
    void Transform::SetTransformMatrix(Matrix4x4 matrix, bool isWorld)
    {
        matrix = matrix.Transpose();

        Vector3 pos, scaleVar;
        Quaternion rotation;

        scaleVar = Vector3(length(matrix[0]), length(matrix[1]), length(matrix[2]));
        pos = Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);

        auto rotateMatrix = matrix;
        for (unsigned int x = 0; x < 3; x++)
        {
            for (unsigned int y = 0; y < 3; y++)
                rotateMatrix[x][y] /= scaleVar[x];
        }
        rotation = rotateMatrix.QuaternionCast();

        if (isWorld)
        {
            SetPosition(pos);
            SetRotation(rotation);
            SetLossyScale(scaleVar);
        }
        else
        {
            LocalPosition = pos;
            LocalEulerAngles = rotation.GetEulerAngles();
            LocalScale = scaleVar;
        }
    }
    Matrix4x4 Transform::GetPositionMatrix() const
    {
        return Matrix4x4::Translate(GetPosition());
    }
    Matrix4x4 Transform::GetRotationMatrix() const
    {
        return Matrix4x4::Rotate(GetRotation());
    }
    Matrix4x4 Transform::GetScaleMatrix() const
    {
        return Matrix4x4::Scale(GetLossyScale());
    }

    Vector3 Transform::GetForward() const { return GetRotation() * Vector3::Forward; }
    Vector3 Transform::GetUp() const { return GetRotation() * Vector3::Up; }
    Vector3 Transform::GetRight() const { return GetRotation() * Vector3::Right; }

    Transform* Transform::GetRootTransfom()
    {
        if (GetGameObject().IsRootObject())
            return this;
        else return _parent->GetRootTransfom();
    }

    void Transform::SetParent(Transform* parent, bool worldPositionStays)
    {
        if (!parent || _parent == parent) return;
        if (_parent) _parent->_childs.Remove(_parent);
        parent->_childs.push_back(this);

        if (!worldPositionStays)
            _parent = parent;
        else
        {
            auto pos = GetPosition();
            auto rotation = GetRotation();
            auto s = GetLossyScale();

            _parent = parent;
            SetPosition(pos);
            SetRotation(rotation);
            SetLossyScale(s);
        }
    }

    void Transform::Translate(Vector3 value, bool isWorld)
    {
        if (!isWorld)
            value = GetRotationMatrix() * Vector4(value, 1.0f);
        LocalPosition = value + LocalPosition;

    }
    void Transform::Rotate(Vector3 axis, float angle, bool isWorld)
    {
        if (axis == Vector3::Zero) return;

        if (isWorld)
            SetRotation(Quaternion::AngleAxis(angle, axis) * GetRotation());
        //SetRotation(rotate(Matrix4x4::Identity, radians(angle), axis) * GetRotation());
        else
            LocalEulerAngles = (Quaternion::AngleAxis(angle, axis) * Quaternion(LocalEulerAngles)).GetEulerAngles();
        //LocalRotation = rotate(LocalRotation, radians(angle), axis);
    }
    void Transform::RotateAround(Vector3 point, Vector3 axis, float angle)
    {
        auto tempPos = GetPosition() - point;
        tempPos = Quaternion::AngleAxis(angle, axis) * tempPos;
        //tempPos = rotate(QUAT_IDENTITY, radians(angle), axis) * tempPos;
        SetPosition(tempPos + point);
        Rotate(axis, angle, true);
    }
    void Transform::LookAt(Vector3 target, Vector3 up)
    {
        auto forward = (target - GetPosition()).GetNormalized();
        SetRotation(Quaternion::LookRotation(forward, up));
    }

    Transform::Transform(GameObject& obj) :Transform(obj, Vector3::Zero, Quaternion::Identity, Vector3::One)
    {
    }

    Transform::Transform(GameObject& obj, Vector3 p, Quaternion r, Vector3 s) : Component(obj, *this),
        LocalPosition(p), LocalEulerAngles(r.GetEulerAngles()), LocalScale(s),
        _parent(nullptr), _childs()
    {
    }
}