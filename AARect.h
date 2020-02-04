#ifndef _AARECT_
#define _AARECT_

#include "Hittable.h"
#include "Material.h"

namespace SNY
{
class AARect : public Hittable
{
public:
    AARect() {}
    AARect(REAL _p0, REAL _p1, REAL _q0, REAL _q1, REAL _k, int axis, bool isFlipNormal, shared_ptr<Material> pMat) : p0(_p0), p1(_p1), q0(_q0), q1(_q1), k(_k), perpendicularAxis(axis), pMaterial(pMat)
    {
        pAxis = (perpendicularAxis + 1) % 3;
        qAxis = (perpendicularAxis + 2) % 3;
        if (pAxis > qAxis)
        {
            swap(pAxis, qAxis);
        }

        REAL sign = isFlipNormal ? -1.0 : 1.0;

        switch (perpendicularAxis)
        {
        case XAxis:
            normal = Vector3R(1.0 * sign, 0, 0);
            break;
        case YAxis:
            normal = Vector3R(0, 1.0 * sign, 0);
            break;
        case ZAxis:
            normal = Vector3R(0, 0, 1.0 * sign);
            break;
        }
    }
    virtual ~AARect() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        REAL t = (k - r.origin[perpendicularAxis]) / r.direction[perpendicularAxis];

        if (t < minT || t > maxT)
        {
            return false;
        }
        REAL p = r.origin[pAxis] + t * r.direction[pAxis];
        REAL q = r.origin[qAxis] + t * r.direction[qAxis];
        if (p < p0 || p > p1 || q < q0 || q > q1)
        {
            return false;
        }
        Vector2R uv((p - p0) / (p1 - p0), (q - q0) / (q1 - q0));
        hi = HitInfo(t, uv, r.GetPointWithParameter(t), normal, pMaterial);
        return true;
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        switch (perpendicularAxis)
        {
        case XAxis:
            return AABB3D(Vector3R(k - 0.0001, p0, q0), Vector3R(k + 0.0001, p1, q1));
        case YAxis:
            return AABB3D(Vector3R(p0, k - 0.0001, q0), Vector3R(p1, k + 0.0001, q1));
        case ZAxis:
            return AABB3D(Vector3R(p0, q0, k - 0.0001), Vector3R(p1, q1, k + 0.0001));
        default:
            return AABB3D();
        }
    }
    virtual void DebugOutput() const
    {
        string axisStr;
        switch (perpendicularAxis)
        {
        case 0:
            axisStr = "x";
            break;
        case 1:
            axisStr = "y";
            break;
        case 2:
            axisStr = "z";
            break;
        default:
            break;
        }
        cout << "AARect: " << p0 << ", " << p1 << ", " << q0 << ", " << q1 << ", " << axisStr << ", " << k << endl;
    }

    REAL p0 = 0;
    REAL p1 = 0;
    REAL q0 = 0;
    REAL q1 = 0;
    REAL k = 0;
    int perpendicularAxis = -1;
    int pAxis = -1;
    int qAxis = -1;
    Vector3R normal;
    shared_ptr<Material> pMaterial = nullptr;
};
} // namespace SNY

#endif
