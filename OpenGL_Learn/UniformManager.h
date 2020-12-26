#pragma once

#include <memory>

#include "UniformBufferObject.h"

namespace OpenGL_Learn
{
	using namespace std;

	class Shader;

	// Uniform缓冲对象管理器
	class UniformManager
	{
	public:
		// 变换 
		// mat4 Matrix_P; 
		// mat4 Matrix_V;
		// mat4 Matrix_M;
		static unique_ptr<UniformBufferObject> Transform;
		// 相机数据
		// vec4 WorldSpaceCameraPos; // 世界空间相机位置
		// vec4 OrthoParams; // x=正交摄影机的宽度，y=正交摄影机的高度，z=宽高比，w=正交摄影机时为1.0，透视时为0.0。
		// vec4 ScreenParams; // x=相机目标纹理的宽度，y=相机目标纹理的高度，z=1.0/width，w=1.0/height。
		static unique_ptr<UniformBufferObject> CameraData;
		// 光照数据
		// vec4 LightColor; // 4N	0 // 光照颜色
		// vec4 WorldSpaceLightPos; // 4N	4N // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
		// mat4 LightMatrix; // 16N	8N // 世界到光的矩阵。用于采样Cookie和衰减纹理。
		// float ShadowStrength; // N	24N // 阴影强度。
		// vec4[4] PointLight4Pos; // 16N	28N // 点光源位置
		// vec4[4] PointLight4Color; // 16N	44N // 点光源颜色
		// vec4[POINT_LIGHTS_COUNT] PointLight4Info; // 16N 60N// 点光源衰减 C=1.0, L=x, Q=y 衰减强度 = 1.0 / (C + L * d + Q * d^2) ，点光源近裁面z，点光源远裁面w
		static unique_ptr<UniformBufferObject> LightingData;
		// 时间
		//vec4 Time; // (RunTime, SinTime, CosTime, DeltaTime);
		static unique_ptr<UniformBufferObject> Time;

		static void UniformBlockBinding(Shader* shader);

		// 初始化 Uniform缓冲对象管理器
		static void Initialize();

	private:

	};
}