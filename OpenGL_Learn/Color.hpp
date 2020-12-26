#pragma once

#include "Math.h"

namespace OpenGL_Learn
{
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

        // HSV颜色转RGB
        static Color HSVtoRGB(float hue, float saturation, float value)
        {
            Color result = Color();

            hue = Math::Clamp(hue, 0.0f, 1.0f);
            saturation = Math::Clamp(saturation, 0.0f, 1.0f);
            value = Math::Clamp(value, 0.0f, 1.0f);

            float rgb_min, rgb_max;
            rgb_max = value;
            rgb_min = rgb_max * (1.0f - saturation);

            int i = (int)(hue / 0.16666667f);
            auto difs = Math::Mod(hue, 0.16666667f); // factorial part of h

                               // RGB adjustment amount by hue 
            float rgb_Adj = (rgb_max - rgb_min) * difs / 0.16666667f;

            switch (i) {
            case 0:
                result.r = rgb_max;
                result.g = rgb_min + rgb_Adj;
                result.b = rgb_min;
                break;
            case 1:
                result.r = rgb_max - rgb_Adj;
                result.g = rgb_max;
                result.b = rgb_min;
                break;
            case 2:
                result.r = rgb_min;
                result.g = rgb_max;
                result.b = rgb_min + rgb_Adj;
                break;
            case 3:
                result.r = rgb_min;
                result.g = rgb_max - rgb_Adj;
                result.b = rgb_max;
                break;
            case 4:
                result.r = rgb_min + rgb_Adj;
                result.g = rgb_min;
                result.b = rgb_max;
                break;
            default:		// case 5:
                result.r = rgb_max;
                result.g = rgb_min;
                result.b = rgb_max - rgb_Adj;
                break;
            }
            return result;
        }

        float r;
        float g;
        float b;
        float a;

        Color() :Color(0.0f, 0.0f, 0.0f, 1.0f) {}
        //Color(const int& r, const int& g, const int& b) :Color(r / 255.f, g / 255.f, b / 255.f, 1.0f) {}
        Color(const float& r, const float& g, const float& b) :Color(r, g, b, 1.0f) {}
        Color(const float& r, const float& g, const float& b, const float& a) :r(r), g(g), b(b), a(a) {}
        // 隐式构造函数
        Color(const Vector4& v) :r(v.x), g(v.y), b(v.z), a(v.w) {}

        // 自定义隐式类型转换
        operator Vector4() const { return Vector4(r, g, b, a); }

        bool operator ==(const Color& color) const { return Math::Approximately((*this).r, color.r) && Math::Approximately((*this).g, color.g) && Math::Approximately((*this).b, color.b) && Math::Approximately((*this).a, color.a); }
        bool operator !=(const Color& color) const { return !(*this == color); }
    };


    /*
    .hpp的相关的使用规则

    a)不可包含全局对象和全局函数
    b)类之间不可循环调用
    c)不可使用静态成员
    */
}