#ifndef _ONB_
#define _ONB_

#include "Vector.h"

namespace SNY
{
class ONB
{
public:
    ONB() {}
    ONB(const Vector3R &n) : W(glm::normalize(n))
    {
        Vector3R a;
        if (glm::abs(W.x) > 0.9)
        {
            a = Vector3R(0, 1.0, 0);
        }
        else
        {
            a = Vector3R(1.0, 0, 0);
        }
        V = glm::normalize(glm::cross(W, a));
        U = glm::cross(W, V);
    }
    ~ONB() {}
    Vector3R TransformFromLocal(REAL a, REAL b, REAL c) const
    {
        return a * U + b * V + c * W;
    }
    Vector3R TransformFromLocal(const Vector3R &a) const
    {
        return a.x * U + a.y * V + a.z * W;
    }

    Vector3R U = Vector3R(0, 0, 0);
    Vector3R V = Vector3R(0, 0, 0);
    Vector3R W = Vector3R(0, 0, 0);
};

inline ostream &operator<<(ostream &os, const ONB &uvw)
{
    os << "ONB: " << uvw.U << ", " << uvw.V << ", " << uvw.W;
    return os;
}
} // namespace SNY

#endif
