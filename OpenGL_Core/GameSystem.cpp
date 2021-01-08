#include "GameSystem.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "SceneManager.h"
#include "UniformManager.h"
#include "Time.h"
#include "InputSystem.h"

#include "Camera.h"


namespace OpenGL_Learn
{
    GLFWwindow* GameSystem::_window = nullptr;
    unsigned int GameSystem::DrawCallCounter = 0;

    void GameSystem::LifeCycle()
    {
        // 初始化 GLFW 窗口 ------------------------------------------------------
        glfwInit();//初始化  和glfwTerminate对应
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//配置GLFW
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);// 禁用双缓冲
        // 多重采样抗锯齿(Multisample Anti-aliasing, MSAA)
        //glfwWindowHint(GLFW_SAMPLES, 4);

        _window = glfwCreateWindow(GameSystem::ScreenWidth, GameSystem::ScreenHeight, "OpenGL", nullptr, nullptr);
        if (_window == nullptr)
        {
            cout << "ERROR:: " << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);// 指定窗口的上下文成为当前线程。
        glfwSwapInterval(0);// 交换缓冲区之前要等待的最小屏幕更新数。 垂直同步

        // 初始化 GLAD （OpenGL函数封装库）------------------------------------------------------
        // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前需要初始化GLAD。
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "ERROR:: " << "Failed to initialize GLAD" << endl;
            return;
        }
        // 多重采样抗锯齿(Multisample Anti-aliasing, MSAA)
        //glEnable(GL_MULTISAMPLE);


        // 修改视口 ------------------------------------------------------
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        glViewport(0, 0, width, height);


        // 初始化输入系统 ------------------------------------------------------
        InputSystem::Initialize();
        glfwSetKeyCallback(_window, InputSystem::KeyCallback);// 注册按键回调
        glfwSetCursorPosCallback(_window, InputSystem::MousePosCallback); // 鼠标位置回调函数


        UniformManager::Initialize();
        SceneManager::Initialize();

        // 游戏循环
        while (!glfwWindowShouldClose(_window))// glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后游戏循环便结束了，之后为我们就可以关闭应用程序了。
        {

            auto timeData = Vector4(Time::RunTime(), sin(Time::RunTime()), cos(Time::RunTime()), Time::DeltaTime());
            UniformManager::Time->SetSubData(0, sizeof(Vector4), timeData.GetPtr());

            // 检查事件 （输入事件）
            InputSystem::ResetOnFrame();
            glfwPollEvents();// glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数（可以通过回调方法手动设置）。我们一般在游戏循环的开始调用事件处理函数。

            SceneManager::GetActiveScene().ExcuteUpdate();

            // 交换缓冲
            glfwSwapBuffers(_window);// glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。

            if (InputSystem::GetKeyDown(KeyCode::ESCAPE))
                glfwSetWindowShouldClose(_window, GL_TRUE);

            PrintError("主循环末尾：");

            // 更新时间
            Time::Update(glfwGetTime());
            //cout << Time::RunTime() << " | " << Time::RealtimeSinceStartup() << endl;
        }
        SceneManager::Destruct();
        glfwTerminate();

    }

    void GameSystem::PrintError(const string& text)
    {
        auto errorCode = glGetError();
        if (!errorCode) return;
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "枚举参数不合法"; break;
        case GL_INVALID_VALUE:                 error = "值参数不合法"; break;
        case GL_INVALID_OPERATION:             error = "一个指令的状态对指令的参数不合法"; break;
        case GL_STACK_OVERFLOW:                error = "压栈操作造成栈上溢(Overflow)"; break;
        case GL_STACK_UNDERFLOW:               error = "弹栈操作时栈在最低点,即栈下溢(Underflow)"; break;
        case GL_OUT_OF_MEMORY:                 error = "内存调用操作无法调用（足够的）内存"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "读取或写入一个不完整的帧缓冲"; break;
        }
        cout << "ERROR:: " << text << error << endl;
    }
}


