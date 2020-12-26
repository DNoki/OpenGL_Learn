#pragma once

#include <memory>

#include "Scene.h"

namespace OpenGL_Learn
{
    using namespace std;


    // ����������
    class SceneManager final
    {
    public:
        // ��ȡ�����
        inline static Scene& GetActiveScene() { return *_activeScene; }

        static void Initialize();
        static void LoadNextScene(unsigned int index);
        static void Destruct();

    private:
        static Scene* _activeScene;
        static List<unique_ptr<Scene>> _scenes;
    };

}

