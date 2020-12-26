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


    // �ƹ�
    class Light : public Behaviour
    {
    public:
        // ���Ͷ����Ӱ
        LightShadows Shadows;
        // ����ǿ��
        float Intensity;
        // ������ɫ
        Color LightColor;

        // ��Ӱǿ��
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

        // ��ȡ���տռ����
        Matrix4x4 GetLightSpaceMatrix(Camera* camera) const;
        void GenerateShadowMap(UINT width = 2048, UINT height = 2048);
        void RenderShadowMap(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);

        DirectionalLight(GameObject& obj);
    };

    class PointLight : public Light
    {
    public:
        static Material* PointLightShadowmapMaterial;

        // ˥�� C=1.0, L=x, Q=y   ˥��ǿ�� = 1.0 / (C + L * d + Q * d^2)
        Vector2 Attenuation;

        RenderTexture* CubeShadowMap;

        // ��ȡ���տռ����
        unique_ptr<List<Matrix4x4>> GetLightSpaceMatrix() const;
        void RenderShadowMap(UINT index, List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        inline Vector4 GetPointLightInfo() { return Vector4(Attenuation, ShadowStrength, ShadowDistance); }

        PointLight(GameObject& obj);

    private:

        //// ������
        //float _nearClipPlane;
        //// Զ����
        //float _farClipPlane;
    };
}