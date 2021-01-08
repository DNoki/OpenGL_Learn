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

    // 投影模式
    enum class ProjectionMode
    {
        // 透视
        PERSPECTIVE = 0,
        // 正交
        ORTHOGRAPHIC = 1,
    };
    // 清除背景选项
    enum class CameraClearFlags
    {
        // 以天空盒填充
        SKYBOX,
        // 以颜色填充
        SOLID_COLOR,
        // 仅清除深度
        DEPTH,
        // 不清除
        NOTHING,
    };

    struct RenderState
    {
    public:
        // 填充背景色
        Color BackgroundColor;
        // 填充深度
        float ClearDepth;
        // 填充模板
        int ClearStencil;

        RenderState() :BackgroundColor(Color(0.937f, 0.521f, 0.608f)), ClearDepth(1.0f), ClearStencil(0) {}

    };

    class Camera final : public Behaviour
    {
    public:
        // 渲染贴图所使用的平面
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
        // 相机如何清除背景。
        CameraClearFlags ClearFlags;
        // 投影模式
        ProjectionMode ProjectionMode;
        // 透视模式的视角 角度制
        float FieldOfView;
        // 正交模式下相机的半尺寸
        float OrthographicSize;
        // 目标渲染纹理
        RenderTexture* TargetTexture;
        // 摄像机在绘制顺序中的位置。值较大的摄像机将绘制在值较小的摄像机顶部。
        int Depth;
        // 是否开启多采样抗锯齿
        bool IsMSAA;


        // 执行渲染
        void ExcuteRender(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        // 按指定模式清除背景（立即）
        void Clear();
        // 场景绘制完成后渲染天空盒
        void DrawSkybox();
        // 绑定渲染目标
        void BindTarget() const;
        // 渲染到目标贴图
        void RenderToTargetTexture(RenderTexture* targetTex = nullptr);

        // 获取观察矩阵
        Matrix4x4 GetViewMatrix() const;
        // 获取投影矩阵
        Matrix4x4 GetProjectionMatrix() const;

        // 设置视角
        inline void SetFieldOfView(float angle) { this->FieldOfView = Math::Clamp(angle, 0.001f, 179.0f); }
        // 设置正交视图大小
        inline void SetOrthographicSize(float size) { this->OrthographicSize = size; }
        // 设置裁切面
        inline void SetClipPlane(float nearPlane, float farPlane)
        {
            this->_nearClipPlane = glm::max(nearPlane, 0.001f);
            this->_farClipPlane = glm::max(farPlane, this->_nearClipPlane + 0.001f);
        }
        // 设置天空盒子
        inline void SetSkybox(Skybox* skybox) { this->_skybox = skybox; }
        // x是正交摄影机的宽度，y是正交摄影机的高度，z为宽高比，w当正交摄影机时为1.0，透视时为0.0。
        Vector4 GetOrthoParams();

        Camera(GameObject& obj);

    private:
        static unique_ptr<RenderState> realRenderState;

        // 近裁面
        float _nearClipPlane;
        // 远裁面
        float _farClipPlane;

        // 天空盒子
        Skybox* _skybox;
    };

}
