#ifndef _HITTABLELIST_
#define _HITTABLELIST_

#include "Hittable.h"

namespace SNY
{
class HittableList : public Hittable
{
public:
    HittableList() {}
    virtual ~HittableList()
    {
    }
    void Add(shared_ptr<const Hittable> pH)
    {
        assert(pH);
        list.push_back(pH);
    }
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        bool isHit = false;
        HitInfo tempHi;
        REAL closest = maxT;
        for (size_t i = 0; i < list.size(); ++i)
        {
            if (list[i]->Hit(r, minT, closest, tempHi))
            {
                isHit = true;
                closest = tempHi.t;
                hi = tempHi;
            }
        }
        return isHit;
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        AABB3D entireBox;
        for (int i = 0; i < list.size(); ++i)
        {
            AABB3D box = list[i]->GetBoundingBox(time0, time1);
            entireBox = CombineBoundingBox(entireBox, box);
        }
        return entireBox;
    }
    REAL ComputePDF(const Vector3R &origin, const Vector3R &direction) const
    {
        if (list.empty())
        {
            return 0;
        }
        REAL sum = 0;
        for (size_t i = 0; i < list.size(); ++i)
        {
            sum += list[i]->ComputePDF(origin, direction);
        }
        return sum / list.size();
    }
    Vector3R GenerateRandomDirection(const Vector3R &origin) const
    {
        if (list.empty())
        {
            return Vector3R(0, 0, 0);
        }
        return list[static_cast<int>(RandomReal() * list.size())]->GenerateRandomDirection(origin);
    }
    virtual void DebugOutput() const
    {
        for (int i = 0; i < list.size(); ++i)
        {
            list[i]->DebugOutput();
        }
    }

    vector<shared_ptr<const Hittable>> list;
};
} // namespace SNY

#endif
