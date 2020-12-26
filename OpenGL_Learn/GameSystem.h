#pragma once

#include <string>

struct GLFWwindow;

namespace OpenGL_Learn
{
    using namespace std;

    class GameSystem final
    {
    public:
        static const int ScreenWidth = 1280, ScreenHeight = 720;
        static unsigned int DrawCallCounter;

        //static void Render();

        // 游戏生命周期
        static void LifeCycle();

        inline static GLFWwindow* GetGLFWwindow() { return _window; }
        static void PrintError(const string& text = "");

    private:
        static GLFWwindow* _window;
    };
}