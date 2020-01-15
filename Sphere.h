#ifndef _SPHERE_
#define _SPHERE_

#include "Hittable.h"
#include "Material.h"

namespace SNY
{
class Sphere : public Hittable
{
public:
    Sphere() {}
    Sphere(const Vector3R &c0, const Vector3R &c1, REAL r, REAL t0, REAL t1, Material *pMat) : center0(c0), center1(c1), radius(r), time0(t0), time1(t1), pMaterial(pMat) {}
    virtual ~Sphere() {}
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
                Vector3R normal = (position - center) / radius;
                hi = HitInfo(t, GetUV(normal), position, normal, pMaterial);
                return true;
            }
            t = (-b + sqrtd) / a;
            if (minT < t && t < maxT)
            {
                Vector3R position = r.GetPointWithParameter(t);
                Vector3R normal = (position - center) / radius;
                hi = HitInfo(t, GetUV(normal), position, normal, pMaterial);
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
        if (time0 == time1)
        {
            return center0;
        }
        else
        {
            return center0 + ((t - time0) / (time1 - time0)) * (center1 - center0);
        }
    }
    Vector2R GetUV(const Vector3R &n) const
    {
        REAL phi = atan2(n.z, n.x);
        REAL theta = asin(n.y);
        return Vector2R(1.0 - (phi + glm::pi<REAL>()) / glm::two_pi<REAL>(), (theta + glm::half_pi<REAL>()) / glm::pi<REAL>());
    }

    Vector3R center0 = Vector3R(0, 0, 0);
    Vector3R center1 = Vector3R(0, 0, 0);
    REAL radius = 0;
    REAL time0 = 0;
    REAL time1 = 0;
    shared_ptr<Material> pMaterial = nullptr;
};
} // namespace SNY

#endif
