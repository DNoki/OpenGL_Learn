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
        NEVER = GL_NEVER, // 从不通过
        LESS = GL_LESS, // 小于时通过
        EQUAL = GL_EQUAL, // 相等时通过
        LEQUAL = GL_LEQUAL, // 小于等于时通过
        GREATER = GL_GREATER, // 大于时通过
        NOTEQUAL = GL_NOTEQUAL, // 不想等时通过
        GEQUAL = GL_GEQUAL, // 大于等于时通过
        ALWAYS = GL_ALWAYS, // 永远通过
    };

    enum class StencilOpType
    {
        KEEP = GL_KEEP, // 保持当前储存的模板值
        ZERO = GL_ZERO, // 将模板值设置为0
        REPLACE = GL_REPLACE, // 将模板值设置为glStencilFunc函数设置的ref值
        INCR = GL_INCR, // 如果模板值小于最大值则将模板值加1
        INCR_WRAP = GL_INCR_WRAP, // 与GL_INCR一样，但如果模板值超过了最大值则归零
        DECR = GL_DECR, // 如果模板值大于最小值则将模板值减1
        DECR_WRAP = GL_DECR_WRAP, // 与GL_DECR一样，但如果模板值小于0则将其设置为最大值
        INVERT = GL_INVERT, // 按位翻转当前的模板缓冲值
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


    // 着色器状态
    struct ShaderState
    {
    public:
        // 设置状态
        // @要设置的名称
        // @设置开启或关闭
        static void SetState(GLenum name, bool enable);

        // 否是开启深度测试
        bool DepthTest;
        bool DepthMask;// 是否写入深度缓冲
        // 深度测试模式
        // GL_ALWAYS    永远通过深度测试
        // GL_NEVER     永远不通过深度测试
        // GL_LESS      在片段深度值 <  缓冲的深度值时通过测试
        // GL_EQUAL     在片段深度值 =  缓冲区的深度值时通过测试
        // GL_LEQUAL	在片段深度值 <= 缓冲区的深度值时通过测试
        // GL_GREATER	在片段深度值 >  缓冲区的深度值时通过测试
        // GL_NOTEQUAL	在片段深度值 != 缓冲区的深度值时通过测试
        // GL_GEQUAL	在片段深度值 >= 缓冲区的深度值时通过测试
        TestModeType DepthTestMode;

        // 是否开启模板测试
        bool Stencil;
        GLuint StencilMask; // 写入模板缓冲区的遮罩 通常是0xFF写入，0x00不写入
        TestModeType StencilTestMode; // 模板测试模式
        GLint StencilRef; // 模板写入与比较值
        GLuint StencilRefMask; // 模板比较值和存储值遮罩
        StencilOpType StencilFail; // 模板测试失败时动作
        StencilOpType StencilZfail; // 模板测试通过但深度测试失败时动作
        StencilOpType StencilZpass; // 模板测试和深度测试都通过时动作

        // 否是开启混合
        bool Blend; // 最终颜色=(源颜色x源因子)+(目标颜色x目标因子) https://www.khronos.org/registry/OpenGL-Refpages/es3/
        BlendFactorType BlendSfactor; // 源颜色混合因子 // 源颜色是当前片段计算的颜色
        BlendFactorType BlendDfactor; // 目标颜色混合因子 // 目标颜色是在缓冲区的颜色
        BlendFactorType BlendSfactorAlpha; // 源Alpha混合因子
        BlendFactorType BlendDfactorAlpha; // 目标Alpha混合因子

        // 否是开启面剔除
        bool CullFace;
        // 面剔除模式
        // GL_BACK：只剔除背面。
        // GL_FRONT：只剔除正面。
        // GL_FRONT_AND_BACK：剔除背面和正面。
        CullFaceModeType CullFaceMode;

        // 正向面
        // GL_CCW 逆时针
        // GL_CW  顺时针
        GLenum FrontFace;

        ShaderState();
    };

    // 着色器
    class Shader final : public ResourceObject
    {
    public:
        // 使用指定渲染状态
        static void UseState(const ShaderState& state);
        // 查询显卡支持可传递多少顶点属性，一般为16个
        static int CheckMaxVertexAttribsCount();

        // 着色器ID
        GLuint ID;
        // 着色器状态
        ShaderState State;
        // 渲染顺序 （越小的值将越先被渲染）
        unsigned int RenderSequence;

        //@brief 绑定2D贴图
        //@param texture 要绑定的贴图
        //@param name 在Shader中声明的采样器名称
        //@param unit 绑定的纹理单元（不同贴图在同一着色器中需要指定不同纹理单元）(纹理单元有上限)
        void BindTexture(Texture& texture, const string& name, unsigned int unit);
        inline void BindTexture(Texture& texture, const string& name) { BindTexture(texture, name, *GetIntPtr(name)); }
        // 使用着色器
        void UseShader(bool willRender = false);

        inline int PropertyToID(const string& name)
        {
            auto result = glGetUniformLocation(this->ID, name.c_str());
            return result;
        }
        List<Tuple2<UniformType, string>> GetAllUniforms();
        // 设置整形
        inline bool SetInt(const string& name, const int& value) { return SetInt(PropertyToID(name), value); }
        bool SetInt(const int& location, const int& value);
        // 设置Float
        inline bool SetFloat(const string& name, const float& value) { return SetFloat(PropertyToID(name), value); }
        bool SetFloat(const int& location, const float& value);
        // 设置Vector4
        inline bool SetVector4(const string& name, const Vector4& value) { return SetVector4(PropertyToID(name), value); }
        bool SetVector4(const int& location, const Vector4& value);
        // 设置Matrix4x4
        inline bool SetMatrix4x4(const string& name, const Matrix4x4& value) { return SetMatrix4x4(PropertyToID(name), value); }
        bool SetMatrix4x4(const int& location, const Matrix4x4& value);

        // 获取整形
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
        // 获取Float
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
        // 获取Vector4
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

        //@brief 绑定Uniform块(Uniform缓冲对象)
        //@param name Shader中的Uniform块名称
        //@param index 绑定的ID （不同Uniform缓冲对象在同一着色器中需要指定不同索引）
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
        //    cout << "LOG:: 着色器:: " << "已释放ID：" << id << endl << endl;
        //}

        Shader(const Shader& shader);
        Shader(const string& name, const string& vert, const string& frag, const string* geom = nullptr);
        // Shader构造函数
        // @Shader名称
        // @顶点着色器文件路径
        // @片段着色器文件路径
        // @图元着色器文件路径
        Shader(const string& name, const char* vPath, const char* fPath, const char* gPath = nullptr);
        Shader(const string& name, const char* path);
        ~Shader();
    private:
        // 着色器拷贝计数器
        static map<int, unsigned int> Counter;
        // 真实渲染状态
        static unique_ptr<ShaderState> realShaderState;

        // key=ID、 datakey=位置、dataValue=值
        static map<int, map<int, int>> StaticIntUniforms;
        static map<int, map<int, float>> StaticFloatUniforms;
        static map<int, map<int, Vector4>> StaticVector4Uniforms;
        static map<int, map<int, Matrix4x4>> StaticMatrix4x4Uniforms;


        // 绑定的贴图 key=纹理单元、value=贴图
        // shader中的sampler采样器实质上是一个int，存储了一个纹理单元
        // 渲染时激活该shader对应的纹理单元并绑定相应的贴图 （上一个渲染激活的纹理单元若没有被修改，仍然可以在shader中读取到）
        map<unsigned int, Texture*> _textures;

        // key=位置、value=值
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