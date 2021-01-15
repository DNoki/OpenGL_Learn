#include "Behaviour.h"

namespace OpenGL_Core
{
    bool Behaviour::GetIsActiveAndEnabled()
    {
        return Enabled && GetGameObject().GetActive();
    }
}