#pragma once

#include "../OpenGL_Core/OpenGL_CoreCommon.h"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;

    class SampleScene final : public Scene
    {
    public:
        virtual void ImportResource() override;

        SampleScene(const string& name) :Scene(name) { }

    private:
    };

}