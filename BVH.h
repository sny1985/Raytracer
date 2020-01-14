#ifndef _BVH_
#define _BVH_

#include "Hittable.h"
#include "AABB.h"

namespace SNY
{
inline int CompareX(const void *p1, const void *p2)
{
    Hittable *pH1 = *(Hittable **)p1;
    Hittable *pH2 = *(Hittable **)p2;
    AABB3D box1(pH1->GetBoundingBox(0, 0));
    AABB3D box2(pH2->GetBoundingBox(0, 0));
    return box1.minPoint.x < box2.minPoint.x ? -1 : 1;
}
inline int CompareY(const void *p1, const void *p2)
{
    Hittable *pH1 = *(Hittable **)p1;
    Hittable *pH2 = *(Hittable **)p2;
    AABB3D box1(pH1->GetBoundingBox(0, 0));
    AABB3D box2(pH2->GetBoundingBox(0, 0));
    return box1.minPoint.y < box2.minPoint.y ? -1 : 1;
}
inline int CompareZ(const void *p1, const void *p2)
{
    Hittable *pH1 = *(Hittable **)p1;
    Hittable *pH2 = *(Hittable **)p2;
    AABB3D box1(pH1->GetBoundingBox(0, 0));
    AABB3D box2(pH2->GetBoundingBox(0, 0));
    return box1.minPoint.z < box2.minPoint.z ? -1 : 1;
}

class BVHNode : public Hittable
{
public:
    BVHNode() {}
    BVHNode(Hittable **hl, int n, REAL time0, REAL time1)
    {
        int axis = int(3 * RandomReal());

        switch (axis)
        {
        case 0:
            qsort(hl, n, sizeof(Hittable *), CompareX);
            break;
        case 1:
            qsort(hl, n, sizeof(Hittable *), CompareY);
            break;
        case 2:
            qsort(hl, n, sizeof(Hittable *), CompareZ);
            break;
        default:
            break;
        }

        if (n == 1)
        {
            left = hl[0];
            right = nullptr;
        }
        else if (n == 2)
        {
            left = hl[0];
            right = hl[1];
        }
        else
        {
            left = new BVHNode(hl, n / 2, time0, time1);
            right = new BVHNode(hl + n / 2, n - n / 2, time0, time1);
        }

        box = left->GetBoundingBox(time0, time1);
        if (right)
        {
            AABB3D rightBox = right->GetBoundingBox(time0, time1);
            box = CombineBoundingBox(box, rightBox);
        }
    }
    ~BVHNode() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        if (box.Hit(r, minT, maxT))
        {
            HitInfo leftHi, rightHi;
            bool isLeftHit = false;
            bool isRightHit = false;
            isLeftHit = left->Hit(r, minT, maxT, leftHi);
            if (right)
            {
                isRightHit = right->Hit(r, minT, maxT, rightHi);
            }
            if (isLeftHit && isRightHit)
            {
                hi = leftHi.t < rightHi.t ? hi = leftHi : hi = rightHi;
                return true;
            }
            else if (isLeftHit)
            {
                hi = leftHi;
                return true;
            }
            else if (isRightHit)
            {
                hi = rightHi;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        return box;
    }
    virtual void DebugOutput() const
    {
        if (left)
        {
            left->DebugOutput();
        }
        if (right)
        {
            right->DebugOutput();
        }
    }

    Hittable *left;
    Hittable *right;
    AABB3D box;
};
} // namespace SNY

#endif
