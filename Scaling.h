#ifndef _SCALING_
#define _SCALING_

#include "Hittable.h"

namespace SNY
{
class Scaling : public Hittable
{
public:
    Scaling() {}
    Scaling(Hittable *pH, const Vector3R &scale) : pHittable(pH)
    {
        matrix = glm::scale(Matrix4R(1.0), scale);
        inverseMatrix = glm::inverse(matrix);

        AABB3D b(pHittable->GetBoundingBox(0, 1));
        box.minPoint = b.minPoint;
        box.maxPoint = scale * (b.maxPoint - b.minPoint) + b.minPoint;
    }
    virtual ~Scaling() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        Vector4R newOrigin = inverseMatrix * Vector4R(r.origin, 1.0);
        Vector4R newDirection = inverseMatrix * Vector4R(r.direction, 0);
        Ray scaledRay(newOrigin.xyz(), newDirection.xyz(), r.time);
        if (pHittable->Hit(scaledRay, minT, maxT, hi))
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
    virtual void DebugOutput() const
    {
        cout << "Scaling:\n"
             << matrix << "\n"
             << inverseMatrix << endl;
        pHittable->DebugOutput();
    }

    shared_ptr<Hittable> pHittable = nullptr;
    Matrix4R matrix = Matrix4R(1.0);
    Matrix4R inverseMatrix = Matrix4R(1.0);
    AABB3D box = AABB3D(Vector3R(0, 0, 0), Vector3R(0, 0, 0));
};
} // namespace SNY

#endif
