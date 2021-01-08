#pragma once

#include <memory>

#include "Scene.h"

namespace OpenGL_Learn
{
    using namespace std;


    // 场景管理器
    class SceneManager final
    {
    public:
        // 获取活动场景
        inline static Scene& GetActiveScene() { return *_activeScene; }

        static void Initialize();
        static void LoadNextScene(unsigned int index);
        static void Destruct();

    private:
        static Scene* _activeScene;
        static List<unique_ptr<Scene>> _scenes;
    };

}

