#pragma once

#include "List.hpp"
#include "Component.h"

//#include "SceneManager.h"
//#include "GameSystem.h"
#include "GameObject.h"

#include "Time.h"


namespace OpenGL_Core
{
    using namespace std;

    class GameObject;

    /// <summary>
    /// 行为组件是可以启用或禁用的组件
    /// </summary>
    class Behaviour : public Component
    {
    public:
        bool GetEnable();
        void SetEnable(bool enable);
        /// <summary>
        /// 该行为是否已激活并已启用？
        /// </summary>
        /// <returns></returns>
        bool GetIsActiveAndEnabled();

        virtual void OnEnable() {}
        virtual void OnDisable() {}

        /// <summary>
        /// 返回找到的第一个组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template <typename T = Component>
        T * GetComponent() const
        {
            return GetGameObject().GetComponent<T>();
        }
        /// <summary>
        /// 返回找到的组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template <typename T = Component>
        List<T*> GetComponents() const
        {
            return GetGameObject().GetComponents<T>();
        }

        Behaviour(GameObject& obj) :Component(obj), _enable(true) {}

    private:
        /// <summary>
        /// 是否激活
        /// </summary>
        bool _enable;
    };
}

