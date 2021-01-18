#include "Behaviour.h"

namespace OpenGL_Core
{
    bool Behaviour::GetEnable()
    {
        return _enable;
    }
    void Behaviour::SetEnable(bool enable)
    {
        if (_enable != enable)
        {
            _enable = enable;
            if (_enable)
                OnEnable();
            else OnDisable();
        }
    }
    bool Behaviour::GetIsActiveAndEnabled()
    {
        return _enable && GetGameObject().GetActive();
    }
}