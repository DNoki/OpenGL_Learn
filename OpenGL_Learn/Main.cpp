#include <iostream>
#include <memory>

#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Definition.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Input.h"
#include "ResourceManager.h"
#include "SkyBox.h"
#include "Time.h"

#ifdef TRUE

using namespace std;
using namespace glm;


int ScreenWidth = 800, ScreenHeight = 600;
// �����ص�
extern void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// ÿ֡����
void Update()
{
    // ������Ϸ����
    for (auto& obj : ResourceManager::GameObjects)
    {
        if (obj->Enabled)
        {
            obj->Update();
        }
    }
    // �������
    for (auto& camera : ResourceManager::Cameras)
    {
        if (camera->Enabled)
        {
            camera->Update();
        }
    }
}
void LateUpdate()
{
    // ������Ϸ����
    for (auto& obj : ResourceManager::GameObjects)
    {
        if (obj->Enabled)
        {
            obj->LateUpdate();
        }
    }
}
void Rendering()
{
    // ��Ⱦָ��
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f); // ״̬���ú���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //״̬ʹ�ú���

    // ���͹�������
    ResourceManager::UniformLight.SetSubBufferData(0, sizeof(vec4), value_ptr(vec4(10, 10, 10, 1.0f)));
    ResourceManager::UniformLight.SetSubBufferData(sizeof(vec4), sizeof(vec4), value_ptr(vec4(0.0f, -1.0f, 0.0f, 0.0f)));

    // ����ÿ�����������Ⱦ
    for (auto& camera : ResourceManager::Cameras)
    {
        if (!(camera->Enabled)) continue;


        // ����ͶӰ���۲����
        auto v = camera->GetViewMatrix();
        auto p = camera->GetProjectionMatrix();
        ResourceManager::UniformTransform.SetSubBufferData(0, sizeof(v), glm::value_ptr(p));
        ResourceManager::UniformTransform.SetSubBufferData(sizeof(v), sizeof(p), glm::value_ptr(v));
        // �����������
        ResourceManager::UniformCamera.SetSubBufferData(0, sizeof(glm::vec4), glm::value_ptr(vec4(camera->GetTransform().position, 1.0f)));

        for (auto& obj : ResourceManager::GameObjects)
        {
            // ����ģ�;���
            auto m = obj->GetTransform().GetTransformMatrix();
            ResourceManager::UniformTransform.SetSubBufferData(sizeof(v) + sizeof(p), sizeof(m), glm::value_ptr(m));

            if (obj->Enabled && obj->GetRenderer().Enabled)
            {
                obj->GetRenderer().Draw();
            }
        }

        //if (camera->Name == "MainCamera")
        {
            // ��Ⱦ��պ�
        }
    }
    //ʵ�����������Shader��װ��ʵ�ָ��ӹ�ϵ�任
}

void ImportTest() {
    // Shader����
    {
        {
            AddShader(move(make_unique<Shader>("SampleShader", ASSET_PATH("Shader/SampleVertShader.vert"), ASSET_PATH("Shader/SampleFragShader.frag"))));
            auto shader = FindShader("SampleShader");
            //shader->State.CullFace = GL_TRUE;
            shader->State.DepthTest = GL_TRUE;
            shader->State.Blend = GL_TRUE;
            //shader->State.FrontFace = GL_CW;
            shader->SetUniformBlockBinding("TRANSFORM", ResourceManager::UniformTransform.BindedID);
            shader->SetUniformBlockBinding("CAMERA", ResourceManager::UniformCamera.BindedID);
            shader->SetUniformBlockBinding("LIGHT", ResourceManager::UniformLight.BindedID);
        }
        {
            AddShader(move(make_unique<Shader>("DiMian", ASSET_PATH("Shader/SampleVertShader.vert"), ASSET_PATH("Shader/Dimian.frag"))));
            auto shader = FindShader("DiMian");
            shader->State.CullFace = GL_TRUE;
            shader->State.DepthTest = GL_TRUE;
            shader->State.Blend = GL_TRUE;
            //shader->State.FrontFace = GL_CW;
        }
        {
            AddShader(move(make_unique<Shader>("RenderTexture", ASSET_PATH("Shader/RenderTexture.vert"), ASSET_PATH("Shader/RenderTexture.frag"))));
            auto shader = FindShader("RenderTexture");
            //shader->State.CullFace = GL_TRUE;
            //shader->State.DepthTest = GL_TRUE;
            //shader->State.Blend = GL_TRUE;
            //shader->State.FrontFace = GL_CW;
        }
        {
            AddShader(move(make_unique<Shader>("SkyBox", ASSET_PATH("Shader/SkyBox.vert"), ASSET_PATH("Shader/SkyBox.frag"))));
            auto shader = FindShader("SkyBox");
            //shader->State.CullFace = GL_TRUE;
            shader->State.DepthTest = GL_TRUE;
            //shader->State.Blend = GL_TRUE;
            //shader->State.FrontFace = GL_CW;
            shader->State.DepthTestMode = GL_LEQUAL;
        }
        {
            auto s = make_unique<Shader>("DisplayNormal",
                ASSET_PATH("Shader/DisplayNormal.vert"),
                ASSET_PATH("Shader/DisplayNormal.frag"),
                ASSET_PATH("Shader/DisplayNormal.geoe"));
            AddShader(move(s));
            auto shader = FindShader("DisplayNormal");
            //shader->State.CullFace = GL_TRUE;
            shader->State.DepthTest = GL_TRUE;
            //shader->State.Blend = GL_TRUE;
            //shader->State.FrontFace = GL_CW;
        }
    }

    // ������������
    {
        {
            auto meshRenderer = MeshRenderer::AssimpLoad(ASSET_PATH("Sphere.fbx"), "SampleShader");
            //meshRenderer->GetMainMaterial().BindTexture(&(Texture2D::LoadTexture2D(ASSET_PATH("TestImage.png"))), "texture2D", 0);
            auto gameObj = make_unique<GameObject>("Object", meshRenderer);
            gameObj->GetTransform().Translate(V3_UP * 1.0f);
            //gameObj->GetTransform().scale = V3_ONE + V3_UP;
            ResourceManager::GameObjects.push_back(move(gameObj));
        }
        {
            auto meshRenderer = MeshRenderer::AssimpLoad(ASSET_PATH("Sphere.obj"), "DisplayNormal");
            auto gameObj = make_unique<GameObject>("Object", meshRenderer);
            gameObj->GetTransform().Translate(V3_UP * 1.0f);
            //gameObj->GetTransform().scale = V3_ONE + V3_UP;
            ResourceManager::GameObjects.push_back(move(gameObj));
        }
        {
            auto meshRenderer = MeshRenderer::Create("Square", "DiMian");
            //meshRenderer->GetMainMaterial().BindTexture(Texture2D::LoatTexture2D("../Asset/TestImage.png"), "texture2D", 0);
            auto gameObj = make_unique<GameObject>("DiMian", meshRenderer);
            gameObj->GetTransform().Rotate(V3_RIGHT, -90.0f);
            gameObj->GetTransform().scale = V3_ONE * 5.0f;
            ResourceManager::GameObjects.push_back(move(gameObj));
        }
    }

    // �������
    {
        auto camera = make_unique<Camera>("MainCamera", make_unique<Transform>(), V3_ZERO);
        camera->SetProjectionMode(ProjectionMode::PERSPECTIVE);
        camera->GetTransform().position = vec3(0.0f, 0.0f, -5.0f);
        camera->GetTransform().LookAt(V3_ZERO);
        auto t = camera->GetTransform().GetEulerAngles();
        ResourceManager::Cameras.push_back(move(camera));
    }
}

// �������
int main(int argc, char** argv[])
{
    //cout << endl;
    //auto t = translate(MATRIX4_IDENTITY, vec3(0.0f, -3.0f, 5.0f)) * vec4(0.0f, 1.0f, 1.0f, 1.0f);
    ////for (size_t i = 0; i < 4; i++)
    ////{
    ////	for (size_t j = 0; j < 4; j++)
    ////	{
    ////		cout << t[i][j] << "  ";
    ////	}
    ////	cout << endl;
    ////}
    //for (size_t j = 0; j < 4; j++)
    //{
    //	cout << t[j] << "  ";
    //}
    //system("pause");
    //return 0;

    glfwInit();//��ʼ��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����GLFW
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Tertris", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);// ָ�����ڵ������ĳ�Ϊ��ǰ�̡߳�

    // GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ע�ᰴ���ص�
    Input::Initialize();
    glfwSetKeyCallback(window, KeyCallback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // �޸��ӿ�
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);

    // ������ʼ��
    ResourceManager::Initialize();
    ImportTest();

    //��պв���
    vector<std::string> faces
    {
        "../Asset/skybox/right.jpg",
        "../Asset/skybox/left.jpg",
        "../Asset/skybox/top.jpg",
        "../Asset/skybox/bottom.jpg",
        "../Asset/skybox/front.jpg",
        "../Asset/skybox/back.jpg"
    };
    auto skyBox = SkyBox("SkyBox", &(TextureCube::LoadTextureCube("SkyCube", faces)), "skybox", 0);


    // ֡�������
    auto frameBuffer = make_unique<FrameBuffer>(ScreenWidth, ScreenHeight, GL_RGBA);
    auto mesh = Mesh::CreatePresetMesh("Square");



    // ��Ϸѭ��
    while (!glfwWindowShouldClose(window))// glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ϸѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
    {
        // ����ʱ��
        Time::Update(glfwGetTime());

        Update();
        LateUpdate();
        frameBuffer->UseFrameBuffer();
        Rendering();

        //��պв���
        auto& camera = (ResourceManager::Cameras[0]);
        auto v = (mat4)((mat3)(camera->GetViewMatrix()));
        ResourceManager::UniformTransform.SetSubBufferData(sizeof(v), sizeof(v), glm::value_ptr(v));
        skyBox.DrawSkyBox();

        frameBuffer->UseDefaultFrameBuffer();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // ״̬���ú���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //״̬ʹ�ú���
        auto shader = FindShader("RenderTexture");
        UseRendererState(shader->State);
        shader->UseShader();
        const glm::mat4 S = mat4(
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        shader->SetMatrix4("Matrix_M", S);
        glActiveTexture(GL_TEXTURE0);// ��������Ԫ
        glBindTexture(GL_TEXTURE_2D, frameBuffer->GetRenderTextureID());
        //glBindTexture(GL_TEXTURE_2D, texture->ID);
        mesh->DrawMesh();

        // ��������
        glfwSwapBuffers(window);// glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�

        // ����¼�
        Input::Reset();
        glfwPollEvents();// glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ���Ȼ����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�������һ������Ϸѭ���Ŀ�ʼ�����¼���������

        if (Input::GetKeyDown(KeyCode::ESCAPE))
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
    glfwTerminate();

    //system("pause");

    return 0;
}

#else
#endif // DEBUG