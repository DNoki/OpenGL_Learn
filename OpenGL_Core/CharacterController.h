#pragma once

#include "Rigidbody.h"

namespace OpenGL_Core
{
    class CharacterController : public Rigidbody
    {
    public:

        CharacterController(GameObject& obj);
        ~CharacterController();

    private:

    };
}

