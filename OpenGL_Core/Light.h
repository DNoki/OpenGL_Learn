#pragma once

#include "Behaviour.h"

#include "Color.h"

#include "Shader.h"
#include "Material.h"
#include "RenderTexture.h"

#include "Camera.h"

namespace OpenGL_Core
{
    enum class LightShadows
    {
        None,
        Hard,
        Soft,
    };


    /// <summary>
    /// 灯光
    /// </summary>
    class Light : public Behaviour
    {
    public:
        /// <summary>
        /// 如何投射阴影
        /// </summary>
        LightShadows Shadows;
        /// <summary>
        /// 光照强度
        /// </summary>
        float Intensity;
        /// <summary>
        /// 光照颜色
        /// </summary>
        Color LightColor;

        /// <summary>
        /// 阴影强度
        /// </summary>
        float ShadowStrength;
        float Bias;
        float NormalBias;

        unsigned int ShadowResolution;
        float ShadowDistance;
        /// <summary>
        /// 近裁面
        /// </summary>
        float ShadowNearPlaneOffset;

        RenderState State;

        //virtual void RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays) = 0;

        Light(GameObject& obj);
    };

    class DirectionalLight : public Light
    {
    public:
        static Material* DirectionalLightShadowmapMaterial;

        RenderTexture* ShadowMap;

        /// <summary>
        /// 获取光照空间矩阵
        /// </summary>
        /// <param name="camera"></param>
        /// <returns></returns>
        Matrix4x4 GetLightSpaceMatrix(Camera* camera) const;
        void GenerateShadowMap(UINT width = 2048, UINT height = 2048);
        void RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);

        DirectionalLight(GameObject& obj);
    };

    class PointLight : public Light
    {
    public:
        static Material* PointLightShadowmapMaterial;

        /// <summary>
        /// 衰减 C=1.0, L=x, Q=y   衰减强度 = 1.0 / (C + L * d + Q * d^2)
        /// </summary>
        Vector2 Attenuation;

        RenderTexture* CubeShadowMap;

        /// <summary>
        /// 获取光照空间矩阵
        /// </summary>
        /// <returns></returns>
        unique_ptr<List<Matrix4x4>> GetLightSpaceMatrix() const;
        void RenderShadowMap(UINT index, List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        inline Vector4 GetPointLightInfo() { return Vector4(Attenuation, ShadowStrength, ShadowDistance); }

        PointLight(GameObject& obj);

    private:
    };
}