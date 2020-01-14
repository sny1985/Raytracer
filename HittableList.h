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
        for (size_t i = 0; i < list.size(); ++i)
        {
            delete list[i];
        }
    }
    void Add(Hittable *pH)
    {
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
    virtual void DebugOutput() const
    {
        for (int i = 0; i < list.size(); ++i)
        {
            list[i]->DebugOutput();
        }
    }

    vector<Hittable *> list;
};
} // namespace SNY

#endif
