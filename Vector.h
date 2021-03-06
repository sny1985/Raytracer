#ifndef _VECTORS_
#define _VECTORS_

#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Common.h"

// Column vector
// x
// y
// z
// w

namespace SNY
{
#ifdef HIGH_PRECISION
typedef glm::dvec2 Vector2R;
typedef glm::dvec3 Vector3R;
typedef glm::dvec4 Vector4R;
#else
typedef glm::vec2 Vector2R;
typedef glm::vec3 Vector3R;
typedef glm::vec4 Vector4R;
#endif

typedef glm::ivec2 Vector2i;
typedef glm::ivec3 Vector3i;
typedef glm::ivec4 Vector4i;

inline Vector2R DeNaN(const Vector2R &c)
{
    Vector2R temp = c;
    if (!(temp[0] == temp[0]))
        temp[0] = 0;
    if (!(temp[1] == temp[1]))
        temp[1] = 0;
    return temp;
}

inline Vector3R DeNaN(const Vector3R &c)
{
    Vector3R temp = c;
    if (!(temp[0] == temp[0]))
        temp[0] = 0;
    if (!(temp[1] == temp[1]))
        temp[1] = 0;
    if (!(temp[2] == temp[2]))
        temp[2] = 0;
    return temp;
}

inline Vector4R DeNaN(const Vector4R &c)
{
    Vector4R temp = c;
    if (!(temp[0] == temp[0]))
        temp[0] = 0;
    if (!(temp[1] == temp[1]))
        temp[1] = 0;
    if (!(temp[2] == temp[2]))
        temp[2] = 0;
    if (!(temp[3] == temp[3]))
        temp[3] = 0;
    return temp;
}

inline ostream &operator<<(ostream &os, const Vector2i &v)
{
    os << "Vector2i: (" << v.x << ", " << v.y << ")";
    return os;
}

inline ostream &operator<<(ostream &os, const Vector3i &v)
{
    os << "Vector3i: (" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

inline ostream &operator<<(ostream &os, const Vector4i &v)
{
    os << "Vector4i: (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

inline ostream &operator<<(ostream &os, const Vector2R &v)
{
    os << "Vector2R: (" << v.x << ", " << v.y << ")";
    return os;
}

inline ostream &operator<<(ostream &os, const Vector3R &v)
{
    os << "Vector3R: (" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

inline ostream &operator<<(ostream &os, const Vector4R &v)
{
    os << "Vector4R: (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}
} // namespace SNY

#endif
