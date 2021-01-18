#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "ResourceObject.h"
#include "Transform.h"

namespace OpenGL_Core
{
    using namespace std;

    class Component;

    /// <summary>
    /// 游戏对象
    /// </summary>
    class GameObject final : public ResourceObject
    {
    public:
        /// <summary>
        /// 是否激活
        /// </summary>
        bool Enabled;

        /// <summary>
        /// 是否是根对象
        /// </summary>
        /// <returns></returns>
        bool IsRootObject();
        /// <summary>
        /// 获取子对象数量
        /// </summary>
        /// <returns></returns>
        unsigned int GetChildsCount();

        /// <summary>
        /// 是否处于激活状态
        /// </summary>
        /// <param name="isInHierarchy">在场景中</param>
        /// <returns></returns>
        bool GetActive(bool isInHierarchy = true);
        /// <summary>
        /// 设置激活状态
        /// </summary>
        /// <param name="value"></param>
        /// <param name="isInHierarchy"></param>
        void SetActive(bool value, bool isInHierarchy = true);

        /// <summary>
        /// 添加组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template <class T = Component>
        T& AddComponent()
        {
            auto component = make_unique<T>(*this);

            auto result = component.get();
            _components.push_back(move(component));

            OnAddComponent(result);
            return *result;
        }
        // 返回找到的第一个组件
        template <typename T = Component>
        T* GetComponent() const
        {
            for (unsigned int i = 0; i < _components.size(); i++)
            {
                auto tryGet = dynamic_cast<T*>(_components[i].get());
                if (tryGet) return tryGet;
            }
            return nullptr;
        }
        // 获取所有指定组件
        template <typename T = Component>
        List<T*> GetComponents()
        {
            auto result = List<T*>();
            for (unsigned int i = 0; i < _components.size(); i++)
            {
                auto tryGet = dynamic_cast<T*>(_components[i].get());
                if (tryGet) result.push_back(tryGet);
            }
            return result;
        }

        static GameObject* Find(const string& name);

        // 获取变换
        inline Transform& GetTransform() const { return *(_transform); }
        // 返回所有组件
        inline List<unique_ptr<Component>>& GetAllComponents() { return _components; }


        GameObject();
        GameObject(const string& name, Transform* parent = nullptr, bool worldPositionStays = true);

    private:
        void OnAddComponent(Component* component);

        unique_ptr<Transform> _transform;
        List<unique_ptr<Component>> _components;
    };



}

