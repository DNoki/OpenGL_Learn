#include "pch.h"
#include "CharacterController.h"


namespace OpenGL_Core
{
    CharacterController::CharacterController(GameObject& obj) : Rigidbody(obj)
    {
    }
    CharacterController::~CharacterController()
    {
    }
}