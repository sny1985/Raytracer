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
    BVHNode(shared_ptr<Hittable> *hl, int n, REAL time0, REAL time1)
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
            pLeft = hl[0];
            pRight = nullptr;
        }
        else if (n == 2)
        {
            pLeft = hl[0];
            pRight = hl[1];
        }
        else
        {
            pLeft.reset(new BVHNode(hl, n / 2, time0, time1));
            pRight.reset(new BVHNode(hl + n / 2, n - n / 2, time0, time1));
        }

        box = pLeft->GetBoundingBox(time0, time1);
        if (pRight)
        {
            AABB3D rightBox = pRight->GetBoundingBox(time0, time1);
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
            isLeftHit = pLeft->Hit(r, minT, maxT, leftHi);
            if (pRight)
            {
                isRightHit = pRight->Hit(r, minT, maxT, rightHi);
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
        if (pLeft)
        {
            pLeft->DebugOutput();
        }
        if (pRight)
        {
            pRight->DebugOutput();
        }
    }

    shared_ptr<Hittable> pLeft = nullptr;
    shared_ptr<Hittable> pRight = nullptr;
    AABB3D box;
};
} // namespace SNY

#endif
