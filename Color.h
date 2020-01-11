#ifndef _COLOR_
#define _COLOR_

#include "Common.h"
#include "Vector.h"

namespace SNY
{
class Color
{
public:
    Color() : Color(255, 255, 255, 255) {}
    Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a = 255) : m_R(r), m_G(g), m_B(b), m_A(a) {}
    Color(uint32_t c) : m_R(c >> 24), m_G(c >> 16 & 0x0ff), m_B(c >> 8 & 0xff), m_A(c & 0xff) {}
    explicit Color(const Vector3R &c)
    {
        Vector3R cc = c / max(max(c.r, c.g), c.b);
        m_R = cc.r * 255;
        m_G = cc.g * 255;
        m_B = cc.b * 255;
        m_A = 255;
    }
    explicit Color(const Vector4R &c)
    {
        Vector4R cc = c / max(max(max(c.r, c.g), c.b), c.a);
        m_R = cc.r * 255;
        m_G = cc.g * 255;
        m_B = cc.b * 255;
        m_A = cc.a * 255;
    }
    bool operator==(const Color &c) const { return m_R == c.m_R && m_G == c.m_G && m_B == c.m_B && m_A == c.m_A; }
    bool operator!=(const Color &c) const { return !(*this == c); }

    Vector3R to_v3r() { return Vector3R(m_R * s_255Inv, m_G * s_255Inv, m_B * s_255Inv); }
    Vector4R to_v4r() { return Vector4R(m_R * s_255Inv, m_G * s_255Inv, m_B * s_255Inv, m_A * s_255Inv); }

    UCHAR m_R;
    UCHAR m_G;
    UCHAR m_B;
    UCHAR m_A;

    static constexpr float s_255Inv = 1.0f / 255.0f;
};
} // namespace SNY

#endif
