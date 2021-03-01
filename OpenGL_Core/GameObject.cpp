#include "pch.h"

#include "GameObject.h"

#include "ScriptBehaviour.h"
#include "SceneManager.h"

namespace OpenGL_Core
{
    bool GameObject::IsRootObject()
    {
        return _transform->GetParent() == nullptr;
    }
    unsigned int GameObject::GetChildsCount()
    {
        return _transform->GetChilds().size();
    }
    bool GameObject::GetActive(bool isInHierarchy)
    {
        if (!isInHierarchy) return _enabled;
        if (_enabled && !IsRootObject())
            return _transform->GetParent()->GetGameObject().GetActive();
        return _enabled;
    }
    void GameObject::SetActive(bool value)
    {
        //if (isInHierarchy)
        //{
        //    if (!IsRootObject())
        //        _transform->GetParent()->GetGameObject().SetActive(value, isInHierarchy);
        //}
        if (_enabled != value)
        {
            _enabled = value;

            for (auto& component : _components)
            {
                auto behaviour = dynamic_cast<Behaviour*>(component.get());
                if (behaviour)
                {
                    if (_enabled)
                        behaviour->OnEnable();
                    else
                        behaviour->OnDisable();
                }
            }
        }
    }

    GameObject* GameObject::Find(const string& name)
    {
        return SceneManager::GetActiveScene()->FindGameObject(name);
    }

    void GameObject::OnAddComponent(Component* component)
    {
        auto script = dynamic_cast<ScriptBehaviour*>(component);
        if (script)
            script->Awake();
    }

    GameObject::GameObject() :ResourceObject("New GameObject"), _enabled(true),
        _transform(make_unique<Transform>(Transform(*this))), _components() {}

    GameObject::GameObject(const string& name, Transform* parent, bool worldPositionStays) : ResourceObject(name), _enabled(true),
        _transform(make_unique<Transform>(Transform(*this))), _components()
    {
        _transform->SetParent(parent, worldPositionStays);
    }
}