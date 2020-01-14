#ifndef _SPHERE_
#define _SPHERE_

#include "Hittable.h"
#include "Material.h"

namespace SNY
{
class Sphere : public Hittable
{
public:
    Sphere(const Vector3R &c0, const Vector3R &c1, REAL r, REAL t0, REAL t1, Material *pMat) : center0(c0), center1(c1), radius(r), time0(t0), time1(t1), pMaterial(pMat) {}
    virtual ~Sphere()
    {
        delete pMaterial;
    }
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        Vector3R center = GetCenter(r.time);
        Vector3R oc(r.origin - center);
        REAL a = glm::dot(r.direction, r.direction);
        REAL b = glm::dot(oc, r.direction);
        REAL c = glm::dot(oc, oc) - radius * radius;
        REAL discriminant = b * b - a * c;
        if (discriminant > 0)
        {
            REAL sqrtd = glm::sqrt(discriminant);

            REAL t = (-b - sqrtd) / a;
            if (minT < t && t < maxT)
            {
                Vector3R position = r.GetPointWithParameter(t);
                hi = HitInfo(t, position, (position - center) / radius, pMaterial);
                return true;
            }
            t = (-b + sqrtd) / a;
            if (minT < t && t < maxT)
            {
                Vector3R position = r.GetPointWithParameter(t);
                hi = HitInfo(t, position, (position - center) / radius, pMaterial);
                return true;
            }
        }
        return false;
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        AABB3D box0(GetCenter(time0) - Vector3R(radius, radius, radius),
                    GetCenter(time0) + Vector3R(radius, radius, radius));
        AABB3D box1(GetCenter(time1) - Vector3R(radius, radius, radius),
                    GetCenter(time1) + Vector3R(radius, radius, radius));
        return CombineBoundingBox(box0, box1);
    }
    virtual void DebugOutput() const
    {
        cout << "Sphere: " << center0 << ", " << radius << endl;
    }
    Vector3R GetCenter(REAL t) const
    {
        return center0 + ((t - time0) / (time1 - time0)) * (center1 - center0);
    }

    Vector3R center0;
    Vector3R center1;
    REAL radius;
    REAL time0;
    REAL time1;
    Material *pMaterial;
};
} // namespace SNY

#endif
