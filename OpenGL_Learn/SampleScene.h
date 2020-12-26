#pragma once

#include "Scene.h"

namespace OpenGL_Learn
{
    class SampleScene final : public Scene
    {
    public:
        virtual void ImportResource() override;

        SampleScene(const string& name) :Scene(name) { }

    private:
    };

}