#include "Component.h"

#include "GameObject.h"

namespace OpenGL_Learn
{
    Component::Component(GameObject& obj) :_gameObject(obj), _transform(obj.GetTransform())
    {
    }
    Component::Component(GameObject& obj, Transform& trans) : _gameObject(obj), _transform(trans)
    {
    }

}