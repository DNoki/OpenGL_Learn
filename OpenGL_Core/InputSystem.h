#pragma once

#include <map>

#include "Math.h"

struct GLFWwindow;

namespace OpenGL_Core
{
    using namespace std;


    /// <summary>
    /// 键
    /// </summary>
    struct Key
    {
    public:
        /// <summary>
        /// 在这一帧摁下？
        /// </summary>
        bool IsPress;
        /// <summary>
        /// 是否摁下状态
        /// </summary>
        bool IsPressed;
        /// <summary>
        /// 在这一帧释放？
        /// </summary>
        bool IsRelease;

        Key();
    };

    class InputSystem final
    {
    public:
        inline static Vector2 GetMouseOffset() { return _mouseOffset; }

        /// <summary>
        /// 按键是否被按下
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        static bool GetKey(int key);
        /// <summary>
        /// 按键是否在这一帧被按下
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        static bool GetKeyDown(int key);
        /// <summary>
        /// 按键是否在这一帧被释放
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        static bool GetKeyUp(int key);

        /// <summary>
        /// 每一帧重置
        /// </summary>
        static void ResetOnFrame();
        /// <summary>
        /// 输入系统初始化
        /// </summary>
        static void Initialize();
        /// <summary>
        /// 按键的回调函数,当有按键指令触发时调用
        /// </summary>
        /// <param name="window"></param>
        /// <param name="key"></param>
        /// <param name="scancode"></param>
        /// <param name="action"></param>
        /// <param name="mode"></param>
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        /// <summary>
        /// 鼠标位置回调，当鼠标在窗口上时触发
        /// </summary>
        /// <param name="window"></param>
        /// <param name="xpos"></param>
        /// <param name="ypos"></param>
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
        F1 = GLFW_KEY_F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    };
}
