#ifndef _AABB_
#define _AABB_

#include "Vector.h"
#include "Ray.h"

namespace SNY
{
class AABB3D
{
public:
    AABB3D() {}
    AABB3D(const Vector3R &min, const Vector3R &max) : minPoint(min), maxPoint(max) {}
    ~AABB3D() {}
    bool Hit(const Ray &r, REAL minT, REAL maxT) const
    {
        Vector3R t0 = (minPoint - r.origin) / r.direction;
        Vector3R t1 = (maxPoint - r.origin) / r.direction;

        for (int i = 0; i < 3; ++i)
        {
            if (r.direction[i] < 0)
            {
                std::swap(t0[i], t1[i]);
            }
            minT = t0[i] > minT ? t0[i] : minT;
            maxT = t1[i] < maxT ? t1[i] : maxT;
            if (maxT <= minT)
            {
                return false;
            }
        }

        return true;
    }
    Vector3R GetDimension() const
    {
        return maxPoint - minPoint;
    }
    int GetLongestAxis() const
    {
        Vector3R dim(maxPoint - minPoint);
        REAL maxDim = dim.x;
        int maxAxis = XAxis;
        if (dim.y > maxDim)
        {
            maxDim = dim.y;
            maxAxis = YAxis;
        }
        if (dim.z > maxDim)
        {
            maxDim = dim.z;
            maxAxis = ZAxis;
        }
        return maxAxis;
    }
    REAL GetArea() const
    {
        Vector3R dim(maxPoint - minPoint);

        return 2.0 * (dim.x * dim.y + dim.x * dim.z + dim.y * dim.z);
    }

    Vector3R minPoint = Vector3R(0, 0, 0);
    Vector3R maxPoint = Vector3R(0, 0, 0);
};

inline AABB3D CombineBoundingBox(const AABB3D &box0, const AABB3D &box1)
{
    Vector3R minPoint(min(box0.minPoint.x, box1.minPoint.x),
                      min(box0.minPoint.y, box1.minPoint.y),
                      min(box0.minPoint.z, box1.minPoint.z));
    Vector3R maxPoint(max(box0.maxPoint.x, box1.maxPoint.x),
                      max(box0.maxPoint.y, box1.maxPoint.y),
                      max(box0.maxPoint.z, box1.maxPoint.z));
    return AABB3D(minPoint, maxPoint);
}

inline ostream &operator<<(ostream &os, const AABB3D &box)
{
    os << "AABB3D: min point " << box.minPoint << ", max point " << box.maxPoint;
    return os;
}
} // namespace SNY

#endif
