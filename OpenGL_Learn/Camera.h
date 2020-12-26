#pragma once

#include <string>
#include <memory>

#include "Math.h"
#include "Color.hpp"
#include "Behaviour.h"

namespace OpenGL_Learn
{
    using namespace std;

    class Mesh;
    class Shader;
    class Material;
    struct RenderItem;
    class RenderTexture;
    //class MultisampleRenderTexture;
    class Skybox;

    // ͶӰģʽ
    enum class ProjectionMode
    {
        // ͸��
        PERSPECTIVE = 0,
        // ����
        ORTHOGRAPHIC = 1,
    };
    // �������ѡ��
    enum class CameraClearFlags
    {
        // ����պ����
        SKYBOX,
        // ����ɫ���
        SOLID_COLOR,
        // ��������
        DEPTH,
        // �����
        NOTHING,
    };

    struct RenderState
    {
    public:
        // ��䱳��ɫ
        Color BackgroundColor;
        // ������
        float ClearDepth;
        // ���ģ��
        int ClearStencil;

        RenderState() :BackgroundColor(Color(0.937f, 0.521f, 0.608f)), ClearDepth(1.0f), ClearStencil(0) {}

    };

    class Camera final : public Behaviour
    {
    public:
        // ��Ⱦ��ͼ��ʹ�õ�ƽ��
        static Mesh* DefaultTargetTextureMesh;
        static Shader* DefaultTargetTextureShader;
        static Material* DefaultTargetTextureMaterial;
        static RenderTexture* DefaultTargetTexture;
        static RenderTexture* DefaultTargetMultisampleTexture;
        static Camera* Main;

        static void UseRenderState(const RenderState& state);
        static void RenderToWindow();
        static void DefaultClear();
        static void DebugRenderTexture(RenderTexture& tex, const float& startX, const float& startY, const float& widthScale, const float& heightScale, Shader* shader = nullptr);

        RenderState State;
        // ���������������
        CameraClearFlags ClearFlags;
        // ͶӰģʽ
        ProjectionMode ProjectionMode;
        // ͸��ģʽ���ӽ� �Ƕ���
        float FieldOfView;
        // ����ģʽ������İ�ߴ�
        float OrthographicSize;
        // Ŀ����Ⱦ����
        RenderTexture* TargetTexture;
        // ������ڻ���˳���е�λ�á�ֵ�ϴ���������������ֵ��С�������������
        int Depth;
        // �Ƿ�������������
        bool IsMSAA;


        // ִ����Ⱦ
        void ExcuteRender(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        // ��ָ��ģʽ���������������
        void Clear();
        // ����������ɺ���Ⱦ��պ�
        void DrawSkybox();
        // ����ȾĿ��
        void BindTarget() const;
        // ��Ⱦ��Ŀ����ͼ
        void RenderToTargetTexture(RenderTexture* targetTex = nullptr);

        // ��ȡ�۲����
        Matrix4x4 GetViewMatrix() const;
        // ��ȡͶӰ����
        Matrix4x4 GetProjectionMatrix() const;

        // �����ӽ�
        inline void SetFieldOfView(float angle) { this->FieldOfView = Math::Clamp(angle, 0.001f, 179.0f); }
        // ����������ͼ��С
        inline void SetOrthographicSize(float size) { this->OrthographicSize = size; }
        // ���ò�����
        inline void SetClipPlane(float nearPlane, float farPlane)
        {
            this->_nearClipPlane = glm::max(nearPlane, 0.001f);
            this->_farClipPlane = glm::max(farPlane, this->_nearClipPlane + 0.001f);
        }
        // ������պ���
        inline void SetSkybox(Skybox* skybox) { this->_skybox = skybox; }
        // x��������Ӱ���Ŀ�ȣ�y��������Ӱ���ĸ߶ȣ�zΪ��߱ȣ�w��������Ӱ��ʱΪ1.0��͸��ʱΪ0.0��
        Vector4 GetOrthoParams();

        Camera(GameObject& obj);

    private:
        static unique_ptr<RenderState> realRenderState;

        // ������
        float _nearClipPlane;
        // Զ����
        float _farClipPlane;

        // ��պ���
        Skybox* _skybox;
    };

}
