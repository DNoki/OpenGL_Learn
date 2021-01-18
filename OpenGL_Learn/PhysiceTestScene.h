#pragma once

#include "../OpenGL_Core/Scene.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class PhysiceTestScene final : public Scene
    {
    public:
        virtual void ImportResource() override;

        PhysiceTestScene(const string& name) :Scene(name) { }

    private:
    };
}

