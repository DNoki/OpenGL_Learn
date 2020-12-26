#pragma once

#include <memory>

#include "UniformBufferObject.h"

namespace OpenGL_Learn
{
	using namespace std;

	class Shader;

	// Uniform������������
	class UniformManager
	{
	public:
		// �任 
		// mat4 Matrix_P; 
		// mat4 Matrix_V;
		// mat4 Matrix_M;
		static unique_ptr<UniformBufferObject> Transform;
		// �������
		// vec4 WorldSpaceCameraPos; // ����ռ����λ��
		// vec4 OrthoParams; // x=������Ӱ���Ŀ�ȣ�y=������Ӱ���ĸ߶ȣ�z=��߱ȣ�w=������Ӱ��ʱΪ1.0��͸��ʱΪ0.0��
		// vec4 ScreenParams; // x=���Ŀ������Ŀ�ȣ�y=���Ŀ������ĸ߶ȣ�z=1.0/width��w=1.0/height��
		static unique_ptr<UniformBufferObject> CameraData;
		// ��������
		// vec4 LightColor; // 4N	0 // ������ɫ
		// vec4 WorldSpaceLightPos; // 4N	4N // ƽ�й⣺������ռ䷽��, 0���������⣺������ռ�λ��, 1����
		// mat4 LightMatrix; // 16N	8N // ���絽��ľ������ڲ���Cookie��˥������
		// float ShadowStrength; // N	24N // ��Ӱǿ�ȡ�
		// vec4[4] PointLight4Pos; // 16N	28N // ���Դλ��
		// vec4[4] PointLight4Color; // 16N	44N // ���Դ��ɫ
		// vec4[POINT_LIGHTS_COUNT] PointLight4Info; // 16N 60N// ���Դ˥�� C=1.0, L=x, Q=y ˥��ǿ�� = 1.0 / (C + L * d + Q * d^2) �����Դ������z�����ԴԶ����w
		static unique_ptr<UniformBufferObject> LightingData;
		// ʱ��
		//vec4 Time; // (RunTime, SinTime, CosTime, DeltaTime);
		static unique_ptr<UniformBufferObject> Time;

		static void UniformBlockBinding(Shader* shader);

		// ��ʼ�� Uniform������������
		static void Initialize();

	private:

	};
}