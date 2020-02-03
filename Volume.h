#ifndef _VOLUME_
#define _VOLUME_

#include "Hittable.h"
#include "Material.h"

namespace SNY
{
class ConstantMedium : public Hittable
{
public:
    ConstantMedium() {}
    ConstantMedium(Hittable *pB, Texture *pPF, REAL d) : pBoundary(pB), pPhaseFunction(new Isotropic(pPF)), density(d) {}
    virtual ~ConstantMedium() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        // Print occasional samples when debugging. To enable, set enableDebug true.
        const bool enableDebug = false;
        bool debugging = enableDebug && RandomReal() < 0.00001;

        HitInfo hi1, hi2;
        if (pBoundary->Hit(r, -FLT_MAX, FLT_MAX, hi1))
        {
            if (pBoundary->Hit(r, hi1.t + 0.0001, FLT_MAX, hi2))
            {

                if (debugging)
                    cerr << "\nt0 t1 " << hi1.t << " " << hi2.t << endl;

                if (hi1.t < minT)
                    hi1.t = minT;

                if (hi2.t > maxT)
                    hi2.t = maxT;

                if (hi1.t >= hi2.t)
                    return false;

                if (hi1.t < 0)
                    hi1.t = 0;

                REAL distanceInsideBoundary = (hi2.t - hi1.t) * glm::length(r.direction);
                REAL hitDistance = -(1 / density) * glm::log(RandomReal());

                if (hitDistance < distanceInsideBoundary)
                {
                    hi = HitInfo(hi1.t + hitDistance / glm::length(r.direction), Vector2R(0, 0), r.GetPointWithParameter(hi.t), Vector3R(1.0, 0, 0), pPhaseFunction);

                    if (debugging)
                    {
                        cerr << "Hit Distance: " << hitDistance << ", "
                             << "t: " << hi.t << ", "
                             << "p: " << hi.position << endl;
                    }

                    return true;
                }
            }
        }
        return false;
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        return pBoundary->GetBoundingBox(time0, time1);
    }
    virtual void DebugOutput() const
    {
        cout << "Constant Medium: " << density << endl;
    }

    shared_ptr<Hittable> pBoundary = nullptr;
    shared_ptr<Material> pPhaseFunction = nullptr;
    REAL density = 1.0;
};
} // namespace SNY

#endif