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
        // ��ʼ�� GLFW ���� ------------------------------------------------------
        glfwInit();//��ʼ��  ��glfwTerminate��Ӧ
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����GLFW
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);// ����˫����
        // ���ز��������(Multisample Anti-aliasing, MSAA)
        //glfwWindowHint(GLFW_SAMPLES, 4);

        _window = glfwCreateWindow(GameSystem::ScreenWidth, GameSystem::ScreenHeight, "OpenGL", nullptr, nullptr);
        if (_window == nullptr)
        {
            cout << "ERROR:: " << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);// ָ�����ڵ������ĳ�Ϊ��ǰ�̡߳�
        glfwSwapInterval(0);// ����������֮ǰҪ�ȴ�����С��Ļ�������� ��ֱͬ��

        // ��ʼ�� GLAD ��OpenGL������װ�⣩------------------------------------------------------
        // GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ��Ҫ��ʼ��GLAD��
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "ERROR:: " << "Failed to initialize GLAD" << endl;
            return;
        }
        // ���ز��������(Multisample Anti-aliasing, MSAA)
        //glEnable(GL_MULTISAMPLE);


        // �޸��ӿ� ------------------------------------------------------
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        glViewport(0, 0, width, height);


        // ��ʼ������ϵͳ ------------------------------------------------------
        InputSystem::Initialize();
        glfwSetKeyCallback(_window, InputSystem::KeyCallback);// ע�ᰴ���ص�
        glfwSetCursorPosCallback(_window, InputSystem::MousePosCallback); // ���λ�ûص�����


        UniformManager::Initialize();
        SceneManager::Initialize();

        // ��Ϸѭ��
        while (!glfwWindowShouldClose(_window))// glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ϸѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
        {

            auto timeData = Vector4(Time::RunTime(), sin(Time::RunTime()), cos(Time::RunTime()), Time::DeltaTime());
            UniformManager::Time->SetSubData(0, sizeof(Vector4), timeData.GetPtr());

            // ����¼� �������¼���
            InputSystem::ResetOnFrame();
            glfwPollEvents();// glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ���Ȼ����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�������һ������Ϸѭ���Ŀ�ʼ�����¼���������

            SceneManager::GetActiveScene().ExcuteUpdate();

            // ��������
            glfwSwapBuffers(_window);// glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�

            if (InputSystem::GetKeyDown(KeyCode::ESCAPE))
                glfwSetWindowShouldClose(_window, GL_TRUE);

            PrintError("��ѭ��ĩβ��");

            // ����ʱ��
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
        case GL_INVALID_ENUM:                  error = "ö�ٲ������Ϸ�"; break;
        case GL_INVALID_VALUE:                 error = "ֵ�������Ϸ�"; break;
        case GL_INVALID_OPERATION:             error = "һ��ָ���״̬��ָ��Ĳ������Ϸ�"; break;
        case GL_STACK_OVERFLOW:                error = "ѹջ�������ջ����(Overflow)"; break;
        case GL_STACK_UNDERFLOW:               error = "��ջ����ʱջ����͵�,��ջ����(Underflow)"; break;
        case GL_OUT_OF_MEMORY:                 error = "�ڴ���ò����޷����ã��㹻�ģ��ڴ�"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "��ȡ��д��һ����������֡����"; break;
        }
        cout << "ERROR:: " << text << error << endl;
    }
}


