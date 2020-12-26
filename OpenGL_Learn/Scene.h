#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "List.h"
#include "GameObject.h"
#include "ResourceObject.h"

namespace OpenGL_Learn
{
    using namespace std;

    class ScriptBehaviour;

    class Scene
    {
    public:
        // ��������
        string Name;

        // �˳����ĸ��任������
        inline unsigned int GetRootCount() const { return (unsigned int)this->_hierarchy.size(); }
        inline List<unique_ptr<GameObject>>& GetAllGameObjects() { return this->_hierarchy; }
        List<GameObject*> GetAllRootGameObjects(bool isUnActiveInclude = false)
        {
            auto objs = List<GameObject*>();
            for (auto& obj : _hierarchy)
            {
                if (!isUnActiveInclude && !obj->GetActive()) continue;
                if (obj->IsRootObject())
                    objs.push_back(obj.get());
            }
            return objs;
        }
        inline map<string, List<unique_ptr<ResourceObject>>>& GetAllResourceObjects() { return this->_resourceObjects; }
        inline List<unique_ptr<ResourceObject>>& GetDestroyedObjects() { return _destroyedObjects; }


        // �����Դ
        template <typename T = ResourceObject>
        T* AddResourceObject(unique_ptr<T>&& obj)
        {
            return AddResourceObject(obj);
        }
        // �����Դ
        template <typename T = ResourceObject>
        T* AddResourceObject(unique_ptr<T>& obj)
        {
            auto key = typeid(T).name();
            if (_resourceObjects.count(key) == 0)
                _resourceObjects.insert(pair<string, List<unique_ptr<ResourceObject>>>(key, List<unique_ptr<ResourceObject>>()));
            auto result = obj.get();
            _resourceObjects[key].push_back(move(obj));
            return result;
        }
        // Ѱ��ָ��������Դ
        template <typename T = ResourceObject>
        T* FindResourceObject(const string& name)
        {
            auto key = typeid(T).name();
            if (_resourceObjects.count(key) == 0)
            {
                cout << "WARNING:: " << "��Դ�ܱ��в����� " << key << " ���͡�" << endl;
                return nullptr;
            }

            for (auto& item : _resourceObjects[key])
            {
                if (item->Name == name)
                    return dynamic_cast<T*>(item.get());
            }
            return nullptr;
        }

        // �����Ϸ����
        GameObject& AddGameObject(unique_ptr<GameObject>&& gameObj);
        // �����Ϸ����
        GameObject& AddGameObject(unique_ptr<GameObject>& gameObj);
        // ���Ҳ����س������ҵ��ĵ�һ���Ѽ������(������������)
        GameObject* FindGameObject(const string& name) const;

        // ���Ҳ����س������ҵ��ĵ�һ���Ѽ������
        template <typename T = Component>
        T* FindComponent(bool isUnActiveInclude = false) const
        {
            // �������ж��󣬲��Ҳ����ص�һ���ҵ��Ķ���
            for (auto& obj : _hierarchy)
            {
                if (!isUnActiveInclude && !obj->GetActive()) continue;

                auto component = obj->GetComponent<T>();
                if (component)
                    return component;
            }
            return nullptr;
        }
        // ��ȡ����������ָ�����
        // @isUnActiveInclude �Ƿ����δ�������
        template <typename T = Component>
        List<T*> FindComponents(bool isUnActiveInclude = false)
        {
            auto components = List<T*>();
            for (auto& obj : _hierarchy)
            {
                if (!isUnActiveInclude && !obj->GetActive()) continue;

                for (auto& component : obj->GetAllComponents())
                {
                    auto tryGetBehaviour = dynamic_cast<T*>(component.get());
                    if (tryGetBehaviour) components.push_back(tryGetBehaviour);
                }
            }
            return components;
        }


        // ��ʼ������
        inline void Initialize()
        {
            ImportResource();
            BindMainObject();
        }
        void ExcuteUpdate();

        Scene(const string& name) :Name(name), _hierarchy(), _resourceObjects() {}

    protected:

        // �㵼��
        virtual void ImportResource() = 0;

    private:
        // ��Ϸ�����б�
        List<unique_ptr<GameObject>> _hierarchy;
        // ��Դ�ܱ�
        map<string, List<unique_ptr<ResourceObject>>> _resourceObjects;
        List<unique_ptr<ResourceObject>> _destroyedObjects;

        // �趨ȫ����Ҫ����
        void BindMainObject();
    };
}