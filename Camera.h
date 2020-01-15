#ifndef _CAMERA_
#define _CAMERA_

#include "Utility.h"
#include "Ray.h"

namespace SNY
{
inline Vector3R GenerateRandomPointOnUnitDisk()
{
    Vector3R p;
    do
    {
        p = 2.0f * Vector3R(RandomReal(), RandomReal(), 0) - Vector3R(1.0, 1.0, 0);
    } while (glm::dot(p, p) >= 1.0);
    return p;
}

class Camera
{
public:
    Camera() {}
    Camera(const Vector3R &position, const Vector3R &lookAt, const Vector3R &up, REAL fov, REAL aspect, REAL aperture, REAL focalLength, REAL t0, REAL t1) : origin(position), time0(t0), time1(t1)
    {
        REAL theta = fov * glm::pi<REAL>() / 180.0;
        REAL halfHeight = glm::tan(0.5 * theta);
        REAL halfWidth = aspect * halfHeight;
        lensRadius = 0.5 * aperture;
        forward = glm::normalize(position - lookAt);
        rightward = glm::normalize(glm::cross(up, forward));
        upward = glm::cross(forward, rightward);
        lowerLeftCorner = origin - halfWidth * focalLength * rightward - halfHeight * focalLength * upward - focalLength * forward;
        horizontal = 2.0f * focalLength * halfWidth * rightward;
        vertical = 2.0f * focalLength * halfHeight * upward;
    }
    ~Camera() {}
    Ray ShootRay(REAL u, REAL v)
    {
        Vector3R rd(lensRadius * GenerateRandomPointOnUnitDisk());
        Vector3R offset(rd.x * rightward + rd.y * upward);
        return Ray(origin + offset, lowerLeftCorner + u * horizontal + v * vertical - origin - offset, time0 + RandomReal() * (time1 - time0));
    }

    Vector3R origin = Vector3R(0, 0, 0);
    Vector3R lowerLeftCorner = Vector3R(0, 0, 0);
    Vector3R horizontal = Vector3R(0, 0, 0);
    Vector3R vertical = Vector3R(0, 0, 0);
    Vector3R forward = Vector3R(0, 0, 0);
    Vector3R rightward = Vector3R(0, 0, 0);
    Vector3R upward = Vector3R(0, 0, 0);
    REAL lensRadius = 0;
    REAL time0 = 0;
    REAL time1 = 0;
};
} // namespace SNY

#endif
