#include "ProjectSetting.h"

#include "../OpenGL_Core/SceneManager.h"

#include "SampleScene.h"
#include "PointLightScene.h"
#include "PhysiceTestScene.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    void ProjectSetting::LoadSetting()
    {
        // 添加场景
        SceneManager::AddScene(make_unique<PhysiceTestScene>("SampleScene"));
    }
}
