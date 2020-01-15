#ifndef _RAY_
#define _RAY_

#include "Vector.h"

namespace SNY
{
class Ray
{
public:
    Ray() {}
    Ray(const Vector3R &o, const Vector3R &d, REAL t) : origin(o), direction(d), time(t) {}
    Vector3R GetPointWithParameter(REAL t) const { return origin + t * direction; }
    ~Ray() {}

    Vector3R origin = Vector3R(0, 0, 0);
    Vector3R direction = Vector3R(0, 0, 0);
    REAL time = 0;
};
} // namespace SNY

#endif
