#pragma once

#include "Behaviour.h"

#include "Color.hpp"

#include "Shader.h"
#include "Material.h"
#include "RenderTexture.h"

#include "Camera.h"

namespace OpenGL_Learn
{
    enum class LightShadows
    {
        None,
        Hard,
        Soft,
    };


    // 灯光
    class Light : public Behaviour
    {
    public:
        // 如何投射阴影
        LightShadows Shadows;
        // 光照强度
        float Intensity;
        // 光照颜色
        Color LightColor;

        // 阴影强度
        float ShadowStrength;
        float Bias;
        float NormalBias;

        unsigned int ShadowResolution;
        float ShadowDistance;
        float ShadowNearPlaneOffset;

        RenderState State;

        //virtual void RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays) = 0;

        Light(GameObject& obj) :Behaviour(obj),
            Shadows(LightShadows::Soft), Intensity(1.0f), LightColor(Color::White()),
            ShadowStrength(0.5f), Bias(0.f), NormalBias(0.f),
            ShadowResolution(1024), ShadowDistance(40), ShadowNearPlaneOffset(0.1f),
            State()
        { }
    };

    class DirectionalLight : public Light
    {
    public:
        static Material* DirectionalLightShadowmapMaterial;

        RenderTexture* ShadowMap;

        // 获取光照空间矩阵
        Matrix4x4 GetLightSpaceMatrix(Camera* camera) const;
        void GenerateShadowMap(UINT width = 2048, UINT height = 2048);
        void RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);

        DirectionalLight(GameObject& obj);
    };

    class PointLight : public Light
    {
    public:
        static Material* PointLightShadowmapMaterial;

        // 衰减 C=1.0, L=x, Q=y   衰减强度 = 1.0 / (C + L * d + Q * d^2)
        Vector2 Attenuation;

        RenderTexture* CubeShadowMap;

        // 获取光照空间矩阵
        unique_ptr<List<Matrix4x4>> GetLightSpaceMatrix() const;
        void RenderShadowMap(UINT index, List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        inline Vector4 GetPointLightInfo() { return Vector4(Attenuation, ShadowStrength, ShadowDistance); }

        PointLight(GameObject& obj);

    private:

        //// 近裁面
        //float _nearClipPlane;
        //// 远裁面
        //float _farClipPlane;
    };
}