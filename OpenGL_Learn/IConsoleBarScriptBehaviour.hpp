#pragma once

#include "ConsoleBar.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class IConsoleBarScriptBehaviour
    {
    public:
        virtual void OnConsoleBar(int index, stringstream& fmt) = 0;
    };
}