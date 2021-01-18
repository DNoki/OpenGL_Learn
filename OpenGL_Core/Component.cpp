#include "pch.h"

#include "Component.h"

#include "GameObject.h"

namespace OpenGL_Core
{
    Component::Component(GameObject& obj) :_gameObject(obj), _transform(obj.GetTransform())
    {
    }
    Component::Component(GameObject& obj, Transform& trans) : _gameObject(obj), _transform(trans)
    {
    }
}