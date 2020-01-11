#ifndef _RAY_
#define _RAY_

#include "Vector.h"

namespace SNY
{
class Ray
{
public:
    Ray() : origin(Vector3R(0, 0, 0)), direction(Vector3R(0, 0, 0)), time(0) {}
    Ray(const Vector3R &o, const Vector3R &d, REAL t) : origin(o), direction(d), time(t) {}
    Vector3R GetPointWithParameter(REAL t) const { return origin + t * direction; }

    Vector3R origin;
    Vector3R direction;
    REAL time;
};
} // namespace SNY

#endif
