#pragma once

#include <string>
#include <memory>

#include "Math.h"
#include "Color.h"
#include "Behaviour.h"

namespace OpenGL_Core
{
    using namespace std;

    class Mesh;
    class Shader;
    class Material;
    struct RenderItem;
    class RenderTexture;
    //class MultisampleRenderTexture;
    class Skybox;

    /// <summary>
    /// 投影模式
    /// </summary>
    enum class ProjectionMode
    {
        /// <summary>
        /// 透视
        /// </summary>
        PERSPECTIVE = 0,
        /// <summary>
        /// 正交
        /// </summary>
        ORTHOGRAPHIC = 1,
    };
    /// <summary>
    /// 清除背景选项
    /// </summary>
    enum class CameraClearFlags
    {
        /// <summary>
        /// 以天空盒填充
        /// </summary>
        SKYBOX,
        /// <summary>
        /// 以颜色填充
        /// </summary>
        SOLID_COLOR,
        /// <summary>
        /// 仅清除深度
        /// </summary>
        DEPTH,
        /// <summary>
        /// 不清除
        /// </summary>
        NOTHING,
    };

    /// <summary>
    /// 渲染状态
    /// </summary>
    struct RenderState
    {
    public:
        /// <summary>
        /// 填充背景色
        /// </summary>
        Color BackgroundColor;
        /// <summary>
        /// 填充深度
        /// </summary>
        float ClearDepth;
        /// <summary>
        /// 填充模板
        /// </summary>
        int ClearStencil;

        RenderState() :BackgroundColor(Color(0.937f, 0.521f, 0.608f)), ClearDepth(1.0f), ClearStencil(0) {}

    };

    /// <summary>
    /// 相机组件
    /// </summary>
    class Camera final : public Behaviour
    {
    public:
        /// <summary>
        /// 渲染贴图所使用的平面
        /// </summary>
        static Mesh* DefaultTargetTextureMesh;
        static Shader* DefaultTargetTextureShader;
        static Material* DefaultTargetTextureMaterial;
        static RenderTexture* DefaultTargetTexture;
        static RenderTexture* DefaultTargetMultisampleTexture;

        static Camera* GetMain();

        static void UseRenderState(const RenderState& state);
        static void RenderToWindow();
        static void DefaultClear();
        static void DebugRenderTexture(RenderTexture& tex, const float& startX, const float& startY, const float& widthScale, const float& heightScale, Shader* shader = nullptr);

        RenderState State;
        /// <summary>
        /// 相机如何清除背景。
        /// </summary>
        CameraClearFlags ClearFlags;
        /// <summary>
        /// 投影模式
        /// </summary>
        ProjectionMode ProjectionMode;
        /// <summary>
        /// 透视模式的视角 角度制
        /// </summary>
        float FieldOfView;
        /// <summary>
        /// 正交模式下相机的半尺寸
        /// </summary>
        float OrthographicSize;
        /// <summary>
        /// 目标渲染纹理
        /// </summary>
        RenderTexture* TargetTexture;
        /// <summary>
        /// 摄像机在绘制顺序中的位置。值较大的摄像机将绘制在值较小的摄像机顶部。
        /// </summary>
        int Depth;
        /// <summary>
        /// 是否开启多采样抗锯齿
        /// </summary>
        bool IsMSAA;


        /// <summary>
        /// 执行渲染
        /// </summary>
        /// <param name="backgrounds"></param>
        /// <param name="geometrys"></param>
        /// <param name="alphaTests"></param>
        /// <param name="transparents"></param>
        /// <param name="overlays"></param>
        void ExcuteRender(List<unique_ptr<RenderItem>>* backgrounds, List<unique_ptr<RenderItem>>* geometrys, List<unique_ptr<RenderItem>>* alphaTests, List<unique_ptr<RenderItem>>* transparents, List<unique_ptr<RenderItem>>* overlays);
        /// <summary>
        /// 按指定模式清除背景（立即）
        /// </summary>
        void Clear();
        /// <summary>
        /// 场景绘制完成后渲染天空盒
        /// </summary>
        void DrawSkybox();
        /// <summary>
        /// 绑定渲染目标
        /// </summary>
        void BindTarget() const;
        /// <summary>
        /// 渲染到目标贴图
        /// </summary>
        /// <param name="targetTex"></param>
        void RenderToTargetTexture(RenderTexture* targetTex = nullptr);

        /// <summary>
        /// 获取观察矩阵
        /// </summary>
        /// <returns></returns>
        Matrix4x4 GetViewMatrix() const;
        /// <summary>
        /// 获取投影矩阵
        /// </summary>
        /// <returns></returns>
        Matrix4x4 GetProjectionMatrix() const;

        /// <summary>
        /// 设置视角
        /// </summary>
        /// <param name="angle"></param>
        inline void SetFieldOfView(float angle) { this->FieldOfView = Math::Clamp(angle, 0.001f, 179.0f); }
        /// <summary>
        /// 设置正交视图大小
        /// </summary>
        /// <param name="size"></param>
        inline void SetOrthographicSize(float size) { this->OrthographicSize = size; }
        /// <summary>
        /// 设置裁切面
        /// </summary>
        /// <param name="nearPlane"></param>
        /// <param name="farPlane"></param>
        inline void SetClipPlane(float nearPlane, float farPlane)
        {
            this->_nearClipPlane = glm::max(nearPlane, 0.001f);
            this->_farClipPlane = glm::max(farPlane, this->_nearClipPlane + 0.001f);
        }
        /// <summary>
        /// 设置天空盒子
        /// </summary>
        /// <param name="skybox"></param>
        inline void SetSkybox(Skybox* skybox) { this->_skybox = skybox; }
        /// <summary>
        /// x是正交摄影机的宽度，y是正交摄影机的高度，z为宽高比，w当正交摄影机时为1.0，透视时为0.0。
        /// </summary>
        /// <returns></returns>
        Vector4 GetOrthoParams();

        Camera(GameObject& obj);

    private:
        static unique_ptr<RenderState> realRenderState;

        /// <summary>
        /// 近裁面
        /// </summary>
        float _nearClipPlane;
        /// <summary>
        /// 远裁面
        /// </summary>
        float _farClipPlane;

        /// <summary>
        /// 天空盒子
        /// </summary>
        Skybox* _skybox;
    };

}
