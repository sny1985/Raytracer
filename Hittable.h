#ifndef _HITTABLE_
#define _HITTABLE_

#include "Ray.h"

namespace SNY
{
class Material;

struct HitInfo
{
    HitInfo() : t(0), position(Vector3R(0, 0, 0)), normal(Vector3R(0, 0, 0)), pMaterial(nullptr) {}
    HitInfo(REAL _t, const Vector3R &p, const Vector3R &n, Material *pMat) : t(_t), position(p), normal(n), pMaterial(pMat) {}
    ~HitInfo() {}

    REAL t;
    Vector3R position;
    Vector3R normal;
    Material *pMaterial;
};

class Hittable
{
public:
    virtual ~Hittable() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const = 0;
};
} // namespace SNY

#endif
