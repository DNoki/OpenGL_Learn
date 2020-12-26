#pragma once

#include <string>

#include "List.h"
#include "Tuple.h"

namespace OpenGL_Learn
{
    using namespace std;

    class GameObject;
    class Transform;

    // ���
    class Component
    {
    public:
        // ��ȡ��Ϸ����
        inline GameObject& GetGameObject() const { return this->_gameObject; }
        // ��ȡ�任
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