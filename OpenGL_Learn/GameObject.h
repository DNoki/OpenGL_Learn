#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "ResourceObject.h"
#include "Transform.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Component;
    class Transform;

    // 游戏对象
    class GameObject final : public ResourceObject
    {
    public:
        // 是否激活
        bool Enabled;

        // 是否是根对象
        inline bool IsRootObject() { return _transform->GetParent() == nullptr; }
        inline unsigned int GetChildsCount() { return _transform->GetChilds().size(); }
        // 是否处于激活状态
        // @isInHierarchy 在场景中
        bool GetActive(bool isInHierarchy = true)
        {
            if (!isInHierarchy) return Enabled;
            if (Enabled && !IsRootObject())
                return _transform->GetParent()->GetGameObject().GetActive();
            return Enabled;
        }
        void SetActive(bool value, bool isInHierarchy = true)
        {
            if (!isInHierarchy)
            {
                Enabled = value;
                return;
            }
            if (!IsRootObject())
                _transform->GetParent()->GetGameObject().SetActive(value, isInHierarchy);
            Enabled = value;
        }

        // 添加组件
        template <class T = Component>
        T& AddComponent()
        {
            auto component = make_unique<T>(*this);

            auto result = component.get();
            _components.push_back(move(component));
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

        // 获取变换
        inline Transform& GetTransform() const { return *(_transform); }
        // 返回所有组件
        inline List<unique_ptr<Component>>& GetAllComponents() { return _components; }


        GameObject() :ResourceObject("New GameObject"), Enabled(true),
            _transform(make_unique<Transform>(Transform(*this))), _components() {}
        GameObject(const string& name, Transform* parent = nullptr, bool worldPositionStays = true) :ResourceObject(name), Enabled(true),
            _transform(make_unique<Transform>(Transform(*this))), _components()
        {
            _transform->SetParent(parent, worldPositionStays);
        }

    private:
        unique_ptr<Transform> _transform;
        List<unique_ptr<Component>> _components;
    };



}

