#pragma once

#include "Scene.h"

namespace OpenGL_Learn
{
    class PointLightScene final : public Scene
    {
    public:
        virtual void ImportResource() override;

        PointLightScene(const string& name) :Scene(name) { }

    private:
    };

}