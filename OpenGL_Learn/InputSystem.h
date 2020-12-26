#pragma once

#include <map>

#include "Math.h"

struct GLFWwindow;

namespace OpenGL_Learn
{
    using namespace std;


    //��
    struct Key
    {
    public:
        // ����һ֡���£�
        bool IsPress;
        // �Ƿ�����״̬
        bool IsPressed;
        // ����һ֡�ͷţ�
        bool IsRelease;

        Key();
    };

    class InputSystem final
    {
    public:
        inline static Vector2 GetMouseOffset() { return _mouseOffset; }

        // �����Ƿ񱻰���
        static bool GetKey(int key);
        // �����Ƿ�����һ֡������
        static bool GetKeyDown(int key);
        // �����Ƿ�����һ֡���ͷ�
        static bool GetKeyUp(int key);

        // ÿһ֡����
        static void ResetOnFrame();
        // ����ϵͳ��ʼ��
        static void Initialize();
        // �����Ļص�����,���а���ָ���ʱ����
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        // ���λ�ûص���������ڴ�����ʱ����
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
