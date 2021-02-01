#pragma once

#include "Math.h"
#include "List.hpp"

#include "Component.h"

namespace OpenGL_Core
{
    // 变换
    class Transform final : public Component
    {
    public:
        // 本地坐标位置
        Vector3 LocalPosition;
        // 本地坐标旋转
        Vector3 LocalEulerAngles;
        // 本地坐标缩放
        Vector3 LocalScale;

        // 获取世界坐标位置
        Vector3 GetPosition(bool isWorld = true) const;
        // 设置位置
        void SetPosition(const Vector3& pos, bool isWorld = true);
        // 获取世界坐标旋转
        Quaternion GetRotation(bool isWorld = true) const;
        // 设置旋转
        void SetRotation(const Quaternion& rot, bool isWorld = true);
        // 获取世界坐标欧拉角
        Vector3 GetEulerAngles(bool isWorld = true) const;
        // 设置欧拉角
        void SetEulerAngles(const Vector3& e, bool isWorld = true);
        // 获取全局比例
        Vector3 GetLossyScale() const;

        // 获取变换矩阵
        Matrix4x4 GetTransformMatrix() const;
        // 获取平移矩阵
        Matrix4x4 GetPositionMatrix() const;
        // 获取旋转矩阵
        Matrix4x4 GetRotationMatrix() const;
        // 尝试设置变换矩阵
        void SetTransformMatrix(Matrix4x4& matrix, bool isWorld = false);

        // 获取前方向
        Vector3 GetForward() const;
        // 获取上方向
        Vector3 GetUp() const;
        // 获取右方向
        Vector3 GetRight() const;

        // 获取根变换
        Transform* GetRootTransfom();
        // 设置父对象
        // @父对象
        // @保持世界空间位置
        void SetParent(Transform* parent, bool worldPositionStays = true);
        inline Transform* GetParent() { return this->_parent; }
        inline List<Transform*>& GetChilds() { return _childs; }

        // 平移
        void Translate(Vector3 value, bool isWorld = false);
        // 绕指定轴向旋转
        void Rotate(Vector3 axis, float angle, bool isWorld = false);
        void RotateAround(Vector3 point, Vector3 axis, float angle);

        // 取得注视方向
        void LookAt(Vector3 target, Vector3 up = Vector3::Up);

        Transform(GameObject& obj);
        Transform(GameObject& obj, Vector3 p, Quaternion r, Vector3 s);

    private:
        // 父对象变换
        Transform* _parent;
        List<Transform*> _childs;

        void SetLossyScale(const Vector3& s);

        Matrix4x4 GetScaleMatrix() const;
    };

}
