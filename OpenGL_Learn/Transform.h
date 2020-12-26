#pragma once

#include "Math.h"

#include "Component.h"

namespace OpenGL_Learn
{
    // �任
    class Transform final : public Component
    {
    public:
        // ��������λ��
        Vector3 LocalPosition;
        // ����������ת
        Vector3 LocalEulerAngles;
        // ������������
        Vector3 LocalScale;

        // ��ȡ��������λ��
        Vector3 GetPosition(bool isWorld = true) const;
        // ����λ��
        void SetPosition(const Vector3& pos, bool isWorld = true);
        // ��ȡ����������ת
        Quaternion GetRotation(bool isWorld = true) const;
        // ������ת
        void SetRotation(const Quaternion& rot, bool isWorld = true);
        // ��ȡ��������ŷ����
        Vector3 GetEulerAngles(bool isWorld = true) const;
        // ����ŷ����
        void SetEulerAngles(const Vector3& e, bool isWorld = true);
        // ��ȡȫ�ֱ���
        Vector3 GetLossyScale() const;

        // ��ȡ�任����
        Matrix4x4 GetTransformMatrix() const;
        // ��ȡƽ�ƾ���
        Matrix4x4 GetPositionMatrix() const;
        // ��ȡ��ת����
        Matrix4x4 GetRotationMatrix() const;
        // �������ñ任����
        void SetTransformMatrix(Matrix4x4 matrix, bool isWorld = false);

        // ��ȡǰ����
        Vector3 GetForward() const;
        // ��ȡ�Ϸ���
        Vector3 GetUp() const;
        // ��ȡ�ҷ���
        Vector3 GetRight() const;

        // ��ȡ���任
        Transform* GetRootTransfom();
        // ���ø�����
        // @������
        // @��������ռ�λ��
        void SetParent(Transform* parent, bool worldPositionStays = true);
        inline Transform* GetParent() { return this->_parent; }
        inline List<Transform*>& GetChilds() { return _childs; }

        // ƽ��
        void Translate(Vector3 value, bool isWorld = false);
        // ��ָ��������ת
        void Rotate(Vector3 axis, float angle, bool isWorld = false);
        void RotateAround(Vector3 point, Vector3 axis, float angle);

        // ȡ��ע�ӷ���
        void LookAt(Vector3 target, Vector3 up = Vector3::Up);

        Transform(GameObject& obj);
        Transform(GameObject& obj, Vector3 p, Quaternion r, Vector3 s);

    private:
        // ������任
        Transform* _parent;
        List<Transform*> _childs;

        void SetLossyScale(const Vector3& s);

        Matrix4x4 GetScaleMatrix() const;
    };

}
