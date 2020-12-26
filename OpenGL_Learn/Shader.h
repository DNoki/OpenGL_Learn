#pragma once

#include <string>
#include <memory>
#include <map>
#include <glad/glad.h>

#include "Math.h"
#include "ResourceObject.h"
#include "Texture.h"

namespace OpenGL_Learn
{
    using namespace std;

    enum class UniformType;
    class Texture;

    enum class TestModeType
    {
        NEVER = GL_NEVER, // �Ӳ�ͨ��
        LESS = GL_LESS, // С��ʱͨ��
        EQUAL = GL_EQUAL, // ���ʱͨ��
        LEQUAL = GL_LEQUAL, // С�ڵ���ʱͨ��
        GREATER = GL_GREATER, // ����ʱͨ��
        NOTEQUAL = GL_NOTEQUAL, // �����ʱͨ��
        GEQUAL = GL_GEQUAL, // ���ڵ���ʱͨ��
        ALWAYS = GL_ALWAYS, // ��Զͨ��
    };

    enum class StencilOpType
    {
        KEEP = GL_KEEP, // ���ֵ�ǰ�����ģ��ֵ
        ZERO = GL_ZERO, // ��ģ��ֵ����Ϊ0
        REPLACE = GL_REPLACE, // ��ģ��ֵ����ΪglStencilFunc�������õ�refֵ
        INCR = GL_INCR, // ���ģ��ֵС�����ֵ��ģ��ֵ��1
        INCR_WRAP = GL_INCR_WRAP, // ��GL_INCRһ���������ģ��ֵ���������ֵ�����
        DECR = GL_DECR, // ���ģ��ֵ������Сֵ��ģ��ֵ��1
        DECR_WRAP = GL_DECR_WRAP, // ��GL_DECRһ���������ģ��ֵС��0��������Ϊ���ֵ
        INVERT = GL_INVERT, // ��λ��ת��ǰ��ģ�建��ֵ
    };

    enum class BlendFactorType
    {
        ZERO = GL_ZERO,
        ONE = GL_ONE,
        SRC_COLOR = GL_SRC_COLOR,
        ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        DST_COLOR = GL_DST_COLOR,
        ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        SRC_ALPHA = GL_SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        DST_ALPHA = GL_DST_ALPHA,
        ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR = GL_CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
    };

    enum class CullFaceModeType
    {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_AND_BACK = GL_FRONT_AND_BACK,
    };


    // ��ɫ��״̬
    struct ShaderState
    {
    public:
        // ����״̬
        // @Ҫ���õ�����
        // @���ÿ�����ر�
        static void SetState(GLenum name, bool enable);

        // ���ǿ�����Ȳ���
        bool DepthTest;
        bool DepthMask;// �Ƿ�д����Ȼ���
        // ��Ȳ���ģʽ
        // GL_ALWAYS    ��Զͨ����Ȳ���
        // GL_NEVER     ��Զ��ͨ����Ȳ���
        // GL_LESS      ��Ƭ�����ֵ <  ��������ֵʱͨ������
        // GL_EQUAL     ��Ƭ�����ֵ =  �����������ֵʱͨ������
        // GL_LEQUAL	��Ƭ�����ֵ <= �����������ֵʱͨ������
        // GL_GREATER	��Ƭ�����ֵ >  �����������ֵʱͨ������
        // GL_NOTEQUAL	��Ƭ�����ֵ != �����������ֵʱͨ������
        // GL_GEQUAL	��Ƭ�����ֵ >= �����������ֵʱͨ������
        TestModeType DepthTestMode;

        // �Ƿ���ģ�����
        bool Stencil;
        GLuint StencilMask; // д��ģ�建���������� ͨ����0xFFд�룬0x00��д��
        TestModeType StencilTestMode; // ģ�����ģʽ
        GLint StencilRef; // ģ��д����Ƚ�ֵ
        GLuint StencilRefMask; // ģ��Ƚ�ֵ�ʹ洢ֵ����
        StencilOpType StencilFail; // ģ�����ʧ��ʱ����
        StencilOpType StencilZfail; // ģ�����ͨ������Ȳ���ʧ��ʱ����
        StencilOpType StencilZpass; // ģ����Ժ���Ȳ��Զ�ͨ��ʱ����

        // ���ǿ������
        bool Blend; // ������ɫ=(Դ��ɫxԴ����)+(Ŀ����ɫxĿ������) https://www.khronos.org/registry/OpenGL-Refpages/es3/
        BlendFactorType BlendSfactor; // Դ��ɫ������� // Դ��ɫ�ǵ�ǰƬ�μ������ɫ
        BlendFactorType BlendDfactor; // Ŀ����ɫ������� // Ŀ����ɫ���ڻ���������ɫ
        BlendFactorType BlendSfactorAlpha; // ԴAlpha�������
        BlendFactorType BlendDfactorAlpha; // Ŀ��Alpha�������

        // ���ǿ������޳�
        bool CullFace;
        // ���޳�ģʽ
        // GL_BACK��ֻ�޳����档
        // GL_FRONT��ֻ�޳����档
        // GL_FRONT_AND_BACK���޳���������档
        CullFaceModeType CullFaceMode;

        // ������
        // GL_CCW ��ʱ��
        // GL_CW  ˳ʱ��
        GLenum FrontFace;

        ShaderState() :
            DepthTest(true), DepthMask(true), DepthTestMode(TestModeType::LESS),
            Stencil(false), StencilMask(0xFF), StencilTestMode(TestModeType::ALWAYS), StencilRef(0), StencilRefMask(0xFF), StencilFail(StencilOpType::KEEP), StencilZfail(StencilOpType::KEEP), StencilZpass(StencilOpType::REPLACE),
            Blend(false), BlendSfactor(BlendFactorType::SRC_ALPHA), BlendDfactor(BlendFactorType::ONE_MINUS_SRC_ALPHA), BlendSfactorAlpha(BlendFactorType::ONE), BlendDfactorAlpha(BlendFactorType::ZERO),
            CullFace(true), CullFaceMode(CullFaceModeType::BACK),
            FrontFace(GL_CCW) {}
    };

    // ��ɫ��
    class Shader final : public ResourceObject
    {
    public:
        // ʹ��ָ����Ⱦ״̬
        static void UseState(const ShaderState& state);
        // ��ѯ�Կ�֧�ֿɴ��ݶ��ٶ������ԣ�һ��Ϊ16��
        static int CheckMaxVertexAttribsCount();

        // ��ɫ��ID
        GLuint ID;
        // ��ɫ��״̬
        ShaderState State;
        // ��Ⱦ˳�� ��ԽС��ֵ��Խ�ȱ���Ⱦ��
        unsigned int RenderSequence;

        //@brief ��2D��ͼ
        //@param texture Ҫ�󶨵���ͼ
        //@param name ��Shader�������Ĳ���������
        //@param unit �󶨵�����Ԫ����ͬ��ͼ��ͬһ��ɫ������Ҫָ����ͬ����Ԫ��(����Ԫ������)
        void BindTexture(Texture& texture, const string& name, unsigned int unit);
        inline void BindTexture(Texture& texture, const string& name) { BindTexture(texture, name, *GetIntPtr(name)); }
        // ʹ����ɫ��
        void UseShader(bool willRender = false);

        inline int PropertyToID(const string& name)
        {
            auto result = glGetUniformLocation(this->ID, name.c_str());
            return result;
        }
        List<Tuple2<UniformType, string>> GetAllUniforms();
        // ��������
        inline bool SetInt(const string& name, const int& value) { return SetInt(PropertyToID(name), value); }
        bool SetInt(const int& location, const int& value);
        // ����Float
        inline bool SetFloat(const string& name, const float& value) { return SetFloat(PropertyToID(name), value); }
        bool SetFloat(const int& location, const float& value);
        // ����Vector4
        inline bool SetVector4(const string& name, const Vector4& value) { return SetVector4(PropertyToID(name), value); }
        bool SetVector4(const int& location, const Vector4& value);
        // ����Matrix4x4
        inline bool SetMatrix4x4(const string& name, const Matrix4x4& value) { return SetMatrix4x4(PropertyToID(name), value); }
        bool SetMatrix4x4(const int& location, const Matrix4x4& value);

        // ��ȡ����
        inline int GetInt(const string& name) { return GetInt(PropertyToID(name)); }
        inline int GetInt(const int& location)
        {
            int result;
            glGetUniformiv(this->ID, location, &result);
            return result;
        }
        inline int* GetIntPtr(const string& name) { return GetIntPtr(PropertyToID(name)); }
        inline int* GetIntPtr(const int& location)
        {
            if (_intUniforms.count(location) == 0)
                _intUniforms.insert(make_pair(location, GetInt(location)));
            return &_intUniforms[location];
        }
        // ��ȡFloat
        inline float GetFloat(const string& name) { return GetFloat(PropertyToID(name)); }
        inline float GetFloat(const int& location)
        {
            float result;
            glGetUniformfv(this->ID, location, &result);
            return result;
        }
        inline float* GetFloatPtr(const string& name) { return GetFloatPtr(PropertyToID(name)); }
        inline float* GetFloatPtr(const int& location)
        {
            if (_floatUniforms.count(location) == 0)
                _floatUniforms.insert(make_pair(location, GetFloat(location)));
            return &_floatUniforms[location];
        }
        // ��ȡVector4
        inline Vector4 GetVector4(const string& name) { return GetVector4(PropertyToID(name)); }
        inline Vector4 GetVector4(const int& location)
        {
            Vector4 result;
            glGetUniformfv(this->ID, location, const_cast<float*>(result.GetPtr()));
            return result;
        }
        inline Vector4* GetVector4Ptr(const string& name) { return GetVector4Ptr(PropertyToID(name)); }
        inline Vector4* GetVector4Ptr(const int& location)
        {
            if (_vector4Uniforms.count(location) == 0)
                _vector4Uniforms.insert(make_pair(location, GetVector4(location)));
            return &_vector4Uniforms[location];
        }

        inline map<int, int>& GetIntUniforms() { return this->_intUniforms; }
        inline map<int, float>& GetFloatUniforms() { return this->_floatUniforms; }
        inline map<int, Vector4>& GetVector4Uniforms() { return this->_vector4Uniforms; }
        inline map<int, Matrix4x4>& GetMatrix4x4Uniforms() { return this->_matrix4x4Uniforms; }

        //@brief ��Uniform��(Uniform�������)
        //@param name Shader�е�Uniform������
        //@param index �󶨵�ID ����ͬUniform���������ͬһ��ɫ������Ҫָ����ͬ������
        inline void SetUniformBlockBinding(const string& name, int uniformBlockBinding)
        {
            auto index = glGetUniformBlockIndex(this->ID, name.c_str());
            if (index != GL_INVALID_INDEX)
                glUniformBlockBinding(this->ID, index, uniformBlockBinding);
        }

        //void Release()
        //{
        //    auto id = this->ID;
        //    glDeleteProgram(this->ID);
        //    cout << "LOG:: ��ɫ��:: " << "���ͷ�ID��" << id << endl << endl;
        //}

        Shader(const Shader& shader);
        Shader(const string& name, const string& vert, const string& frag, const string* geom = nullptr);
        // Shader���캯��
        // @Shader����
        // @������ɫ���ļ�·��
        // @Ƭ����ɫ���ļ�·��
        // @ͼԪ��ɫ���ļ�·��
        Shader(const string& name, const char* vPath, const char* fPath, const char* gPath = nullptr);
        Shader(const string& name, const char* path);
        ~Shader();
    private:
        // ��ɫ������������
        static map<int, unsigned int> Counter;
        // ��ʵ��Ⱦ״̬
        static unique_ptr<ShaderState> realShaderState;

        // key=ID�� datakey=λ�á�dataValue=ֵ
        static map<int, map<int, int>> StaticIntUniforms;
        static map<int, map<int, float>> StaticFloatUniforms;
        static map<int, map<int, Vector4>> StaticVector4Uniforms;
        static map<int, map<int, Matrix4x4>> StaticMatrix4x4Uniforms;


        // �󶨵���ͼ key=����Ԫ��value=��ͼ
        // shader�е�sampler������ʵ������һ��int���洢��һ������Ԫ
        // ��Ⱦʱ�����shader��Ӧ������Ԫ������Ӧ����ͼ ����һ����Ⱦ���������Ԫ��û�б��޸ģ���Ȼ������shader�ж�ȡ����
        map<unsigned int, Texture*> _textures;

        // key=λ�á�value=ֵ
        map<int, int> _intUniforms;
        map<int, float> _floatUniforms;
        map<int, Vector4> _vector4Uniforms;
        map<int, Matrix4x4> _matrix4x4Uniforms;

        void CheckShaderCompile(GLuint shaderID, const string& type);
        void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometry = nullptr);

        Shader(const string& name);
    };


    enum class UniformType
    {
        FLOAT = GL_FLOAT,	 // 	float
        //FLOAT_VEC2 = GL_FLOAT_VEC2,	//	vec2
        //FLOAT_VEC3 = GL_FLOAT_VEC3,	//	vec3
        FLOAT_VEC4 = GL_FLOAT_VEC4,	//	vec4
        //DOUBLE = GL_DOUBLE,	//	double
        //DOUBLE_VEC2 = GL_DOUBLE_VEC2,	//	dvec2
        //DOUBLE_VEC3 = GL_DOUBLE_VEC3,	//	dvec3
        //DOUBLE_VEC4 = GL_DOUBLE_VEC4,	//	dvec4
        INT = GL_INT,	//	int
        //INT_VEC2 = GL_INT_VEC2,	//	ivec2
        //INT_VEC3 = GL_INT_VEC3,	//	ivec3
        //INT_VEC4 = GL_INT_VEC4,	//	ivec4
        //UNSIGNED_INT = GL_UNSIGNED_INT,	//	unsigned int
        //UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2,	//	uvec2
        //UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3,	//	uvec3
        //UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4,	//	uvec4
        //BOOL = GL_BOOL,	//	bool
        //BOOL_VEC2 = GL_BOOL_VEC2,	//	bvec2
        //BOOL_VEC3 = GL_BOOL_VEC3,	//	bvec3
        //BOOL_VEC4 = GL_BOOL_VEC4,	//	bvec4
        //FLOAT_MAT2 = GL_FLOAT_MAT2,	//	mat2
        //FLOAT_MAT3 = GL_FLOAT_MAT3,	//	mat3
        FLOAT_MAT4 = GL_FLOAT_MAT4,	//	mat4
        //FLOAT_MAT2x3 = GL_FLOAT_MAT2x3,	//	mat2x3
        //FLOAT_MAT2x4 = GL_FLOAT_MAT2x4,	//	mat2x4
        //FLOAT_MAT3x2 = GL_FLOAT_MAT3x2,	//	mat3x2
        //FLOAT_MAT3x4 = GL_FLOAT_MAT3x4,	//	mat3x4
        //FLOAT_MAT4x2 = GL_FLOAT_MAT4x2,	//	mat4x2
        //FLOAT_MAT4x3 = GL_FLOAT_MAT4x3,	//	mat4x3
        //DOUBLE_MAT2 = GL_DOUBLE_MAT2,	//	dmat2
        //DOUBLE_MAT3 = GL_DOUBLE_MAT3,	//	dmat3
        //DOUBLE_MAT4 = GL_DOUBLE_MAT4,	//	dmat4
        //DOUBLE_MAT2x3 = GL_DOUBLE_MAT2x3,	//	dmat2x3
        //DOUBLE_MAT2x4 = GL_DOUBLE_MAT2x4,	//	dmat2x4
        //DOUBLE_MAT3x2 = GL_DOUBLE_MAT3x2,	//	dmat3x2
        //DOUBLE_MAT3x4 = GL_DOUBLE_MAT3x4,	//	dmat3x4
        //DOUBLE_MAT4x2 = GL_DOUBLE_MAT4x2,	//	dmat4x2
        //DOUBLE_MAT4x3 = GL_DOUBLE_MAT4x3,	//	dmat4x3
        //SAMPLER_1D = GL_SAMPLER_1D,	//	sampler1D
        SAMPLER_2D = GL_SAMPLER_2D,	//	sampler2D
        //SAMPLER_3D = GL_SAMPLER_3D,	//	sampler3D
        SAMPLER_CUBE = GL_SAMPLER_CUBE,	//	samplerCube
        //SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,	//	sampler1DShadow
        //SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,	//	sampler2DShadow
        //SAMPLER_1D_ARRAY = GL_SAMPLER_1D_ARRAY,	//	sampler1DArray
        //SAMPLER_2D_ARRAY = GL_SAMPLER_2D_ARRAY,	//	sampler2DArray
        //SAMPLER_1D_ARRAY_SHADOW = GL_SAMPLER_1D_ARRAY_SHADOW,	//	sampler1DArrayShadow
        //SAMPLER_2D_ARRAY_SHADOW = GL_SAMPLER_2D_ARRAY_SHADOW,	//	sampler2DArrayShadow
        //SAMPLER_2D_MULTISAMPLE = GL_SAMPLER_2D_MULTISAMPLE,	//	sampler2DMS
        //SAMPLER_2D_MULTISAMPLE_ARRAY = GL_SAMPLER_2D_MULTISAMPLE_ARRAY,	//	sampler2DMSArray
        //SAMPLER_CUBE_SHADOW = GL_SAMPLER_CUBE_SHADOW,	//	samplerCubeShadow
        //SAMPLER_BUFFER = GL_SAMPLER_BUFFER,	//	samplerBuffer
        //SAMPLER_2D_RECT = GL_SAMPLER_2D_RECT,	//	sampler2DRect
        //SAMPLER_2D_RECT_SHADOW = GL_SAMPLER_2D_RECT_SHADOW,	//	sampler2DRectShadow
        //INT_SAMPLER_1D = GL_INT_SAMPLER_1D,	//	isampler1D
        //INT_SAMPLER_2D = GL_INT_SAMPLER_2D,	//	isampler2D
        //INT_SAMPLER_3D = GL_INT_SAMPLER_3D,	//	isampler3D
        //INT_SAMPLER_CUBE = GL_INT_SAMPLER_CUBE,	//	isamplerCube
        //INT_SAMPLER_1D_ARRAY = GL_INT_SAMPLER_1D_ARRAY,	//	isampler1DArray
        //INT_SAMPLER_2D_ARRAY = GL_INT_SAMPLER_2D_ARRAY,	//	isampler2DArray
        //INT_SAMPLER_2D_MULTISAMPLE = GL_INT_SAMPLER_2D_MULTISAMPLE,	//	isampler2DMS
        //INT_SAMPLER_2D_MULTISAMPLE_ARRAY = GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,	//	isampler2DMSArray
        //INT_SAMPLER_BUFFER = GL_INT_SAMPLER_BUFFER,	//	isamplerBuffer
        //INT_SAMPLER_2D_RECT = GL_INT_SAMPLER_2D_RECT,	//	isampler2DRect
        //UNSIGNED_INT_SAMPLER_1D = GL_UNSIGNED_INT_SAMPLER_1D,	//	usampler1D
        //UNSIGNED_INT_SAMPLER_2D = GL_UNSIGNED_INT_SAMPLER_2D,	//	usampler2D
        //UNSIGNED_INT_SAMPLER_3D = GL_UNSIGNED_INT_SAMPLER_3D,	//	usampler3D
        //UNSIGNED_INT_SAMPLER_CUBE = GL_UNSIGNED_INT_SAMPLER_CUBE,	//	usamplerCube
        //UNSIGNED_INT_SAMPLER_1D_ARRAY = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,	//	usampler2DArray
        //UNSIGNED_INT_SAMPLER_2D_ARRAY = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,	//	usampler2DArray
        //UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,	//	usampler2DMS
        //UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,	//	usampler2DMSArray
        //UNSIGNED_INT_SAMPLER_BUFFER = GL_UNSIGNED_INT_SAMPLER_BUFFER,	//	usamplerBuffer
        //UNSIGNED_INT_SAMPLER_2D_RECT = GL_UNSIGNED_INT_SAMPLER_2D_RECT,	//	usampler2DRect
        //IMAGE_1D = GL_IMAGE_1D,	//	image1D
        //IMAGE_2D = GL_IMAGE_2D,	//	image2D
        //IMAGE_3D = GL_IMAGE_3D,	//	image3D
        //IMAGE_2D_RECT = GL_IMAGE_2D_RECT,	//	image2DRect
        //IMAGE_CUBE = GL_IMAGE_CUBE,	//	imageCube
        //IMAGE_BUFFER = GL_IMAGE_BUFFER,	//	imageBuffer
        //IMAGE_1D_ARRAY = GL_IMAGE_1D_ARRAY,	//	image1DArray
        //IMAGE_2D_ARRAY = GL_IMAGE_2D_ARRAY,	//	image2DArray
        //IMAGE_2D_MULTISAMPLE = GL_IMAGE_2D_MULTISAMPLE,	//	image2DMS
        //IMAGE_2D_MULTISAMPLE_ARRAY = GL_IMAGE_2D_MULTISAMPLE_ARRAY,	//	image2DMSArray
        //INT_IMAGE_1D = GL_INT_IMAGE_1D,	//	iimage1D
        //INT_IMAGE_2D = GL_INT_IMAGE_2D,	//	iimage2D
        //INT_IMAGE_3D = GL_INT_IMAGE_3D,	//	iimage3D
        //INT_IMAGE_2D_RECT = GL_INT_IMAGE_2D_RECT,	//	iimage2DRect
        //INT_IMAGE_CUBE = GL_INT_IMAGE_CUBE,	//	iimageCube
        //INT_IMAGE_BUFFER = GL_INT_IMAGE_BUFFER,	//	iimageBuffer
        //INT_IMAGE_1D_ARRAY = GL_INT_IMAGE_1D_ARRAY,	//	iimage1DArray
        //INT_IMAGE_2D_ARRAY = GL_INT_IMAGE_2D_ARRAY,	//	iimage2DArray
        //INT_IMAGE_2D_MULTISAMPLE = GL_INT_IMAGE_2D_MULTISAMPLE,	//	iimage2DMS
        //INT_IMAGE_2D_MULTISAMPLE_ARRAY = GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,	//	iimage2DMSArray
        //UNSIGNED_INT_IMAGE_1D = GL_UNSIGNED_INT_IMAGE_1D,	//	uimage1D
        //UNSIGNED_INT_IMAGE_2D = GL_UNSIGNED_INT_IMAGE_2D,	//	uimage2D
        //UNSIGNED_INT_IMAGE_3D = GL_UNSIGNED_INT_IMAGE_3D,	//	uimage3D
        //UNSIGNED_INT_IMAGE_2D_RECT = GL_UNSIGNED_INT_IMAGE_2D_RECT,	//	uimage2DRect
        //UNSIGNED_INT_IMAGE_CUBE = GL_UNSIGNED_INT_IMAGE_CUBE,	//	uimageCube
        //UNSIGNED_INT_IMAGE_BUFFER = GL_UNSIGNED_INT_IMAGE_BUFFER,	//	uimageBuffer
        //UNSIGNED_INT_IMAGE_1D_ARRAY = GL_UNSIGNED_INT_IMAGE_1D_ARRAY,	//	uimage1DArray
        //UNSIGNED_INT_IMAGE_2D_ARRAY = GL_UNSIGNED_INT_IMAGE_2D_ARRAY,	//	uimage2DArray
        //UNSIGNED_INT_IMAGE_2D_MULTISAMPLE = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,	//	uimage2DMS
        //UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,	//	uimage2DMSArray
        //UNSIGNED_INT_ATOMIC_COUNTER = GL_UNSIGNED_INT_ATOMIC_COUNTER,	//	atomic_uint
    };

}