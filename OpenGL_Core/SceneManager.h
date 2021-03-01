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
        inline static Scene* GetActiveScene() { return _scenes[_activeScene].get(); }

        /// <summary>
        /// 设置第一个要加载的场景
        /// </summary>
        /// <param name="sceneIndex"></param>
        static void SetStartupScene(int sceneIndex);
        static void Initialize();
        static void LoadNextScene(unsigned int index);
        static void CheckLoadNextScene();
        static void AddScene(unique_ptr<Scene> scene);
        static void Destruct();

    private:
        static int _activeScene;
        static int _nextScene;
        static List<unique_ptr<Scene>> _scenes;
    };

}

