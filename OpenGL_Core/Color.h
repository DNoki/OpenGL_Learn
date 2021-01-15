/*
.hpp的相关的使用规则

a)不可包含全局对象和全局函数
b)类之间不可循环调用
c)不可使用静态成员
*/

#pragma once

namespace OpenGL_Core
{
    struct Vector4;

    /// <summary>
    /// 颜色类
    /// </summary>
    struct Color
    {
    public:
        inline static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
        inline static Color White() { return Color(1.0f, 1.0f, 1.0f); }
        inline static Color Gray() { return Color(0.5f, 0.5f, 0.5f); }
        inline static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
        inline static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
        inline static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
        inline static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }
        inline static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }
        inline static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }

        /// <summary>
        /// HSV颜色转RGB
        /// </summary>
        /// <param name="hue"></param>
        /// <param name="saturation"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        static Color HSVtoRGB(float hue, float saturation, float value);

        float r;
        float g;
        float b;
        float a;

        Color() :Color(0.0f, 0.0f, 0.0f, 1.0f) {}
        //Color(const int& r, const int& g, const int& b) :Color(r / 255.f, g / 255.f, b / 255.f, 1.0f) {}
        Color(const float& r, const float& g, const float& b) :Color(r, g, b, 1.0f) {}
        Color(const float& r, const float& g, const float& b, const float& a) :r(r), g(g), b(b), a(a) {}
        Color(const Vector4& v);

        operator Vector4() const;// 自定义隐式类型转换

        bool operator ==(const Color& color) const;
        bool operator !=(const Color& color) const;
    };
}