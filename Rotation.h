#ifndef _ROTATION_
#define _ROTATION_

#include "Hittable.h"

namespace SNY
{
class Rotation : public Hittable
{
public:
    Rotation() {}
    Rotation(shared_ptr<const Hittable> pH, REAL angle, int axis) : pHittable(pH)
    {
        Vector3R axisVector;
        switch (axis)
        {
        case XAxis:
            axisVector = Vector3R(1.0, 0, 0);
            break;
        case YAxis:
            axisVector = Vector3R(0, 1.0, 0);
            break;
        case ZAxis:
            axisVector = Vector3R(0, 0, 1.0);
            break;
        default:
            cerr << "Wrong axis" << endl;
            break;
        }
        matrix = glm::rotate(Matrix4R(1.0), angle, axisVector);
        inverseMatrix = glm::inverse(matrix);

        Vector3R min(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3R max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        AABB3D b(pHittable->GetBoundingBox(0, 1));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    REAL x = i * b.maxPoint.x + (1.0 - i) * b.minPoint.x;
                    REAL y = j * b.maxPoint.y + (1.0 - j) * b.minPoint.y;
                    REAL z = k * b.maxPoint.z + (1.0 - k) * b.minPoint.z;
                    Vector4R newPoint = matrix * Vector4R(x, y, z, 1.0);
                    Vector3R tester(newPoint.xyz());
                    for (int c = 0; c < 3; ++c)
                    {
                        if (tester[c] > max[c])
                        {
                            max[c] = tester[c];
                        }
                        if (tester[c] < min[c])
                        {
                            min[c] = tester[c];
                        }
                    }
                }
            }
        }
        box.minPoint = min;
        box.maxPoint = max;
    }
    virtual ~Rotation() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        Vector4R newOrigin = inverseMatrix * Vector4R(r.origin, 1.0);
        Vector4R newDirection = inverseMatrix * Vector4R(r.direction, 0);
        Ray rotatedRay(newOrigin.xyz(), newDirection.xyz(), r.time);
        if (pHittable->Hit(rotatedRay, minT, maxT, hi))
        {
            Vector4R newPosition = matrix * Vector4R(hi.position, 1.0);
            Vector4R newNormal = matrix * Vector4R(hi.normal, 1.0);
            hi.position = newPosition.xyz();
            hi.normal = newNormal.xyz();
            return true;
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
    virtual REAL ComputePDF(const Vector3R& origin, const Vector3R& direction) const
    {
        return pHittable->ComputePDF(origin, direction);
    }
    virtual Vector3R GenerateRandomDirection(const Vector3R& origin) const
    {
        return pHittable->GenerateRandomDirection(origin);
    }
    virtual void DebugOutput() const
    {
        cout << "Rotation:\n"
             << matrix << "\n"
             << inverseMatrix << endl;
        pHittable->DebugOutput();
    }

    shared_ptr<const Hittable> pHittable = nullptr;
    Matrix4R matrix = Matrix4R(1.0);
    Matrix4R inverseMatrix = Matrix4R(1.0);
    AABB3D box = AABB3D(Vector3R(0, 0, 0), Vector3R(0, 0, 0));
};
} // namespace SNY

#endif
