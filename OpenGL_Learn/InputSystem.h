#pragma once

#include <map>

#include "Math.h"

struct GLFWwindow;

namespace OpenGL_Learn
{
    using namespace std;


    //键
    struct Key
    {
    public:
        // 在这一帧摁下？
        bool IsPress;
        // 是否摁下状态
        bool IsPressed;
        // 在这一帧释放？
        bool IsRelease;

        Key();
    };

    class InputSystem final
    {
    public:
        inline static Vector2 GetMouseOffset() { return _mouseOffset; }

        // 按键是否被按下
        static bool GetKey(int key);
        // 按键是否在这一帧被按下
        static bool GetKeyDown(int key);
        // 按键是否在这一帧被释放
        static bool GetKeyUp(int key);

        // 每一帧重置
        static void ResetOnFrame();
        // 输入系统初始化
        static void Initialize();
        // 按键的回调函数,当有按键指令触发时调用
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        // 鼠标位置回调，当鼠标在窗口上时触发
        static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);

    private:
        static map<int, Key> _keys;
        static Vector2 _mousePos;
        static Vector2 _mouseOffset;
    };

    enum KeyCode
    {
        SPACE = 32,
        KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        ESCAPE = 256,
        RIGHT = 262, LEFT, DOWN, UP,
    };
}
