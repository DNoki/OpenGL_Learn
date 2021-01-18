#include "pch.h"

#include "Color.h"

#include "Math.h"

namespace OpenGL_Core
{
    Color Color::HSVtoRGB(float hue, float saturation, float value)
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

    Color::Color(const Vector4& v) :r(v.x), g(v.y), b(v.z), a(v.w) {} // 隐式构造函数

    Color::operator Vector4() const
    {
        return Vector4(r, g, b, a);
    }

    bool Color::operator==(const Color& color) const
    {
        return Math::Approximately((*this).r, color.r) && Math::Approximately((*this).g, color.g) && Math::Approximately((*this).b, color.b) && Math::Approximately((*this).a, color.a);
    }
    bool Color::operator!=(const Color& color) const
    {
        return !(*this == color);
    }
}