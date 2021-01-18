#include "pch.h"

#include "InputSystem.h"

namespace OpenGL_Core
{
    map<int, Key> InputSystem::_keys;
    Vector2 InputSystem::_mousePos;
    Vector2 InputSystem::_mouseOffset;

    Key::Key() :IsPress(false), IsPressed(false), IsRelease(false) {}

    bool InputSystem::GetKey(int key)
    {
        return _keys[key].IsPressed;
    }

    bool InputSystem::GetKeyDown(int key)
    {
        return _keys[key].IsPress;
    }

    bool InputSystem::GetKeyUp(int key)
    {
        return _keys[key].IsRelease;
    }

    void InputSystem::ResetOnFrame()
    {
        for (auto& value : _keys)
        {
            value.second.IsPress = false;
            if (value.second.IsRelease)
            {
                value.second.IsPressed = false;
                value.second.IsRelease = false;
            }
        }
        _mouseOffset = Vector2::Zero;
    }

    void InputSystem::Initialize()
    {
        InputSystem::_keys.insert({ GLFW_KEY_SPACE, Key() });

        InputSystem::_keys.insert({ GLFW_KEY_0, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_1, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_2, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_3, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_4, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_5, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_6, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_7, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_8, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_9, Key() });

        InputSystem::_keys.insert({ GLFW_KEY_A, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_B, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_C, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_D, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_E, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_F, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_G, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_H, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_I, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_J, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_K, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_L, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_M, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_N, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_O, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_P, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_Q, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_R, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_S, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_T, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_U, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_V, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_W, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_X, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_Y, Key() });
        InputSystem::_keys.insert({ GLFW_KEY_Z, Key() });

        InputSystem::_keys.insert({ GLFW_KEY_ESCAPE, Key() });
    }

    void InputSystem::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        // 当有按键指令触发时调用，判断已绑定按键是否包含触发按键，若包含则修改该按键相应状态
        if (InputSystem::_keys.count(key))
        {
            if (action == GLFW_PRESS)
            {
                InputSystem::_keys[key].IsPress = true;
                InputSystem::_keys[key].IsPressed = true;
            }
            else if (action == GLFW_RELEASE)
            {
                InputSystem::_keys[key].IsPressed = false;
                InputSystem::_keys[key].IsRelease = true;
            }
        }
    }

    void InputSystem::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto x = (float)xpos, y = (float)ypos;
        InputSystem::_mouseOffset = Vector2(x - InputSystem::_mousePos.x, y - InputSystem::_mousePos.y);
        InputSystem::_mousePos = Vector2(x, y);
    }
}