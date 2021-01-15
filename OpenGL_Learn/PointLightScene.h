#pragma once

#include "../OpenGL_Core/Scene.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class PointLightScene final : public Scene
    {
    public:
        virtual void ImportResource() override;

        PointLightScene(const string& name) :Scene(name) { }

    private:
    };

}