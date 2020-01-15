#ifndef _HITTABLE_
#define _HITTABLE_

#include "Ray.h"
#include "AABB.h"

namespace SNY
{
class Material;

struct HitInfo
{
    HitInfo() {}
    HitInfo(REAL _t, const Vector2R &_uv, const Vector3R &p, const Vector3R &n, shared_ptr<Material> pMat) : t(_t), uv(_uv), position(p), normal(n), pMaterial(pMat)
    {
        assert(pMaterial);
    }
    ~HitInfo() {}

    REAL t = 0;
    Vector2R uv = Vector2R(0, 0);
    Vector3R position = Vector3R(0, 0, 0);
    Vector3R normal = Vector3R(0, 0, 0);
    shared_ptr<Material> pMaterial = nullptr;
};

class Hittable
{
public:
    Hittable() {}
    virtual ~Hittable() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const = 0;
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const = 0;

    virtual void DebugOutput() const = 0;
};
} // namespace SNY

#endif
