#ifndef _MATRIX_
#define _MATRIX_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Common.h"

// Column major matrix, since OpenGL uses column major matrix
// m00 m10 m20 m30
// m01 m11 m21 m31
// m02 m12 m22 m32
// m03 m13 m23 m33
// x   y   z   w

namespace SNY
{
#ifdef HIGH_PRECISION
typedef glm::dmat3 Matrix3R;
typedef glm::dmat4 Matrix4R;
#else
typedef glm::mat3 Matrix3R;
typedef glm::mat4 Matrix4R;
#endif

inline ostream &operator<<(ostream &os, const Matrix3R &m)
{
    os << "Matrix3R: \n"
       << m[0][0] << " " << m[1][0] << " " << m[2][0] << "\n"
       << m[0][1] << " " << m[1][1] << " " << m[2][1] << "\n"
       << m[0][2] << " " << m[1][2] << " " << m[2][2];
    return os;
}

inline ostream &operator<<(ostream &os, const Matrix4R &m)
{
    os << "Matrix4R: \n"
       << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << "\n"
       << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << "\n"
       << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << "\n"
       << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3];
    return os;
}
} // namespace SNY

#endif
