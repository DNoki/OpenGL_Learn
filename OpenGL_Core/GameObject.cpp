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
        if (!isInHierarchy) return Enabled;
        if (Enabled && !IsRootObject())
            return _transform->GetParent()->GetGameObject().GetActive();
        return Enabled;
    }
    void GameObject::SetActive(bool value, bool isInHierarchy)
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

    GameObject* GameObject::Find(const string& name)
    {
        return SceneManager::GetActiveScene().FindGameObject(name);
    }

    void GameObject::OnAddComponent(Component* component)
    {
        auto script = dynamic_cast<ScriptBehaviour*>(component);
        if (script)
            script->Awake();
    }

    GameObject::GameObject() :ResourceObject("New GameObject"), Enabled(true),
        _transform(make_unique<Transform>(Transform(*this))), _components() {}

    GameObject::GameObject(const string& name, Transform* parent, bool worldPositionStays) : ResourceObject(name), Enabled(true),
        _transform(make_unique<Transform>(Transform(*this))), _components()
    {
        _transform->SetParent(parent, worldPositionStays);
    }
}