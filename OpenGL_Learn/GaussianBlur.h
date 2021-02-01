#pragma once

#include "IConsoleBarScriptBehaviour.hpp"

namespace OpenGL_Learn
{
    using namespace OpenGL_Core;
    using namespace std;

    class GaussianBlur : public ScriptBehaviour, public IConsoleBarScriptBehaviour
    {
    public:
        RenderTexture* EffectRenderTexture;
        Material* gaussianBlurMaterial;
        unsigned int DownSample;
        unsigned int Iteration;

        void Awake() override;

        void OnRenderImage(RenderTexture* targetTex) override;

        void OnConsoleBar(int index, stringstream& fmt) override;

        GaussianBlur(GameObject& obj);
    };

}