#ifndef _HITTABLE_
#define _HITTABLE_

#include "Ray.h"
#include "AABB.h"

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
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const = 0;

    virtual void DebugOutput() const = 0;
};
} // namespace SNY

#endif
