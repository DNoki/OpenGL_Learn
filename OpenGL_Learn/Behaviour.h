#pragma once

#include "Component.h"

#include "SceneManager.h"
#include "GameSystem.h"
#include "GameObject.h"

#include "Time.h"


namespace OpenGL_Learn
{
    using namespace std;

    // 行为是可以启用或禁用的组件
    class Behaviour : public Component
    {
    public:
        // 是否激活
        bool Enabled;
        // 该行为是否已激活并已启用？
        inline bool GetIsActiveAndEnabled()
        {
            return Enabled && GetGameObject().GetActive();
        }

        // 返回找到的第一个组件
        template <typename T = Component>
        T * GetComponent() const
        {
            return GetGameObject().GetComponent<T>();
        }
        // 返回找到的组件
        template <typename T = Component>
        List<T*> GetComponents() const
        {
            return GetGameObject().GetComponents<T>();
        }

        Behaviour(GameObject& obj) :Component(obj), Enabled(true) {}
    };
}

