#ifndef _BOX_
#define _BOX_

#include "AARect.h"

namespace SNY
{
class Box : public Hittable
{
public:
    Box() {}
    Box(const Vector3R &minPt, const Vector3R &maxPt, shared_ptr<Material> pMat) : minPoint(minPt), maxPoint(maxPt)
    {
        list.Add(new AARect(minPt.x, maxPt.x, minPt.y, maxPt.y, maxPt.z, ZAxis, false, pMat));
        list.Add(new AARect(minPt.x, maxPt.x, minPt.y, maxPt.y, minPt.z, ZAxis, true, pMat));
        list.Add(new AARect(minPt.x, maxPt.x, minPt.z, maxPt.z, maxPt.y, YAxis, false, pMat));
        list.Add(new AARect(minPt.x, maxPt.x, minPt.z, maxPt.z, minPt.y, YAxis, true, pMat));
        list.Add(new AARect(minPt.y, maxPt.y, minPt.z, maxPt.z, maxPt.x, XAxis, false, pMat));
        list.Add(new AARect(minPt.y, maxPt.y, minPt.z, maxPt.z, minPt.x, XAxis, true, pMat));
    }
    virtual ~Box() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        return list.Hit(r, minT, maxT, hi);
    }
    virtual AABB3D GetBoundingBox(REAL time0, REAL time1) const
    {
        return AABB3D(minPoint, maxPoint);
    }
    virtual void DebugOutput() const
    {
        cout << "Box: " << minPoint << ", " << maxPoint << endl;
        list.DebugOutput();
    }

    Vector3R minPoint = Vector3R(0, 0, 0);
    Vector3R maxPoint = Vector3R(0, 0, 0);
    HittableList list;
};
} // namespace SNY

#endif
