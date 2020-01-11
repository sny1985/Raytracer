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

    vector<Hittable *> list;
};
} // namespace SNY

#endif
