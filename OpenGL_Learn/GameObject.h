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

    // ��Ϸ����
    class GameObject final : public ResourceObject
    {
    public:
        // �Ƿ񼤻�
        bool Enabled;

        // �Ƿ��Ǹ�����
        inline bool IsRootObject() { return _transform->GetParent() == nullptr; }
        inline unsigned int GetChildsCount() { return _transform->GetChilds().size(); }
        // �Ƿ��ڼ���״̬
        // @isInHierarchy �ڳ�����
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

        // ������
        template <class T = Component>
        T& AddComponent()
        {
            auto component = make_unique<T>(*this);

            auto result = component.get();
            _components.push_back(move(component));
            return *result;
        }
        // �����ҵ��ĵ�һ�����
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
        // ��ȡ����ָ�����
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

        // ��ȡ�任
        inline Transform& GetTransform() const { return *(_transform); }
        // �����������
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

