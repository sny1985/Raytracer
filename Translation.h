#ifndef _TRANSLATION_
#define _TRANSLATION_

#include "Hittable.h"
#include "Matrix.h"

namespace SNY
{
class Translation : public Hittable
{
public:
    Translation() {}
    Translation(Hittable *pH, const Vector3R &offset) : pHittable(pH)
    {
        matrix = glm::translate(Matrix4R(1.0), offset);
        inverseMatrix = glm::inverse(matrix);

        AABB3D b(pHittable->GetBoundingBox(0, 1));
        box.minPoint = b.minPoint + offset;
        box.maxPoint = b.maxPoint + offset;
    }
    virtual ~Translation() {}
    virtual bool Hit(const Ray &r, REAL minT, REAL maxT, HitInfo &hi) const
    {
        Vector4R newOrigin = inverseMatrix * Vector4R(r.origin, 1.0);
        Vector4R newDirection = inverseMatrix * Vector4R(r.direction, 0);
        Ray translatedRay(newOrigin.xyz(), newDirection.xyz(), r.time);
        if (pHittable->Hit(translatedRay, minT, maxT, hi))
        {
            Vector4R newPosition = matrix * Vector4R(hi.position, 1.0);
            hi.position = newPosition.xyz();
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
        cout << "Translation:\n"
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
