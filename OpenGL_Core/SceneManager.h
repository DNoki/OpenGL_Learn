#pragma once

#include <memory>

#include "Scene.h"

namespace OpenGL_Core
{
    using namespace std;


    /// <summary>
    /// 场景管理器
    /// </summary>
    class SceneManager final
    {
    public:
        /// <summary>
        /// 获取活动场景
        /// </summary>
        /// <returns></returns>
        inline static Scene& GetActiveScene() { return *_activeScene; }

        static void Initialize();
        static void LoadNextScene(unsigned int index);
        static void AddScene(unique_ptr<Scene> scene);
        static void Destruct();

    private:
        static Scene* _activeScene;
        static List<unique_ptr<Scene>> _scenes;
    };

}

