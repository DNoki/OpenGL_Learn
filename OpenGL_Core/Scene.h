#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "List.hpp"
#include "GameObject.h"
#include "ResourceObject.h"
#include "Physics.h"

namespace OpenGL_Core
{
    using namespace std;

    class ScriptBehaviour;

    class Scene
    {
    public:
        /// <summary>
        /// 场景名称
        /// </summary>
        string Name;

        /// <summary>
        /// 此场景的根变换数量。
        /// </summary>
        /// <returns></returns>
        inline unsigned int GetRootCount() const { return (unsigned int)this->_hierarchy.size(); }
        inline List<unique_ptr<GameObject>>& GetAllGameObjects() { return this->_hierarchy; }
        List<GameObject*> GetAllRootGameObjects(bool isUnActiveInclude = false);
        inline map<string, List<unique_ptr<ResourceObject>>>& GetAllResourceObjects() { return this->_resourceObjects; }
        inline List<unique_ptr<ResourceObject>>& GetDestroyedObjects() { return _destroyedObjects; }

        inline Physics& GetPhysicsEngine() { return *_physics; }

        /// <summary>
        /// 添加资源
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="obj"></param>
        /// <returns></returns>
        template <typename T = ResourceObject>
        T* AddResourceObject(unique_ptr<T>&& obj)
        {
            return AddResourceObject(obj);
        }
        /// <summary>
        /// 添加资源
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="obj"></param>
        /// <returns></returns>
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
        /// <summary>
        /// 寻找指定名称资源
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <returns></returns>
        template <typename T = ResourceObject>
        T* FindResourceObject(const string& name)
        {
            auto key = typeid(T).name();
            if (_resourceObjects.count(key) == 0)
            {
                cout << "WARNING:: " << "资源总表中不存在 " << key << " 类型。" << endl;
                return nullptr;
            }

            for (auto& item : _resourceObjects[key])
            {
                if (item->Name == name)
                    return dynamic_cast<T*>(item.get());
            }
            return nullptr;
        }

        /// <summary>
        /// 添加游戏对象
        /// </summary>
        /// <param name="gameObj"></param>
        /// <returns></returns>
        GameObject& AddGameObject(unique_ptr<GameObject>&& gameObj);
        /// <summary>
        /// 添加游戏对象
        /// </summary>
        /// <param name="gameObj"></param>
        /// <returns></returns>
        GameObject& AddGameObject(unique_ptr<GameObject>& gameObj);
        /// <summary>
        /// 查找并返回场景内找到的第一个已激活对象(仅检索根对象)
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        GameObject* FindGameObject(const string& name) const;

        /// <summary>
        /// 查找并返回场景内找到的第一个已激活组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="isUnActiveInclude"></param>
        /// <returns></returns>
        template <typename T = Component>
        T* FindComponent(bool isUnActiveInclude = false) const
        {
            // 遍历所有对象，查找并返回第一个找到的对象
            for (auto& obj : _hierarchy)
            {
                if (!isUnActiveInclude && !obj->GetActive()) continue;

                auto component = obj->GetComponent<T>();
                if (component)
                    return component;
            }
            return nullptr;
        }
        /// <summary>
        /// 获取场景内所有指定组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="isUnActiveInclude">是否包括未激活对象</param>
        /// <returns></returns>
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


        /// <summary>
        /// 初始化场景
        /// </summary>
        inline void Initialize()
        {
            ImportResource();
            BindMainObject();
        }
        /// <summary>
        /// 由系统调用更新场景
        /// </summary>
        void ExcuteUpdate();

        Scene(const string& name);

    protected:

        /// <summary>
        /// 初始层资源加载
        /// </summary>
        virtual void ImportResource() = 0;

    private:
        /// <summary>
        /// 游戏对象列表
        /// </summary>
        List<unique_ptr<GameObject>> _hierarchy;
        /// <summary>
        /// 资源总表
        /// </summary>
        map<string, List<unique_ptr<ResourceObject>>> _resourceObjects;
        List<unique_ptr<ResourceObject>> _destroyedObjects;
        unique_ptr<Physics> _physics;

        /// <summary>
        /// 设定全局主要对象
        /// </summary>
        void BindMainObject();
    };
}