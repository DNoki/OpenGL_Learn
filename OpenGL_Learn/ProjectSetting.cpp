﻿#include "pch.h"

#include "ProjectSetting.h"

#include "SampleScene.h"
#include "PointLightScene.h"
#include "PhysiceTestScene.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    void ProjectSetting::LoadSetting()
    {
        // 添加场景
        SceneManager::AddScene(make_unique<SampleScene>("SampleScene"));
        SceneManager::AddScene(make_unique<PhysiceTestScene>("PhysiceTestScene"));
        SceneManager::AddScene(make_unique<PointLightScene>("PointLightScene"));
        SceneManager::SetStartupScene(0);
    }
}
