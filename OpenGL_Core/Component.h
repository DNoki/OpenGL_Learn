#pragma once

#include <string>

namespace OpenGL_Core
{
    using namespace std;

    class GameObject;
    class Transform;

    /// <summary>
    /// 组件
    /// </summary>
    class Component
    {
    public:
        /// <summary>
        /// 获取游戏对象
        /// </summary>
        /// <returns></returns>
        inline GameObject& GetGameObject() const { return this->_gameObject; }
        /// <summary>
        /// 获取变换
        /// </summary>
        /// <returns></returns>
        inline Transform& GetTransform() const { return this->_transform; }

        virtual ~Component() {}

        //bool operator == (const Component& value) { return this == &value; }

    protected:
        Component(GameObject& obj);
        Component(GameObject& obj, Transform& trans);

    private:
        GameObject& _gameObject;
        Transform& _transform;

    };
}