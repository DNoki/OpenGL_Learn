#pragma once

#include <string>

#include "List.h"
#include "Tuple.h"

namespace OpenGL_Learn
{
    using namespace std;

    class GameObject;
    class Transform;

    // 组件
    class Component
    {
    public:
        // 获取游戏对象
        inline GameObject& GetGameObject() const { return this->_gameObject; }
        // 获取变换
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