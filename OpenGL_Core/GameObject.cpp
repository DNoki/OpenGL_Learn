#include "GameObject.h"

#include "ScriptBehaviour.h"

void OpenGL_Learn::GameObject::OnAddComponent(Component* component)
{
    auto script = dynamic_cast<ScriptBehaviour*>(component);
    if (script)
        script->Awake();
}