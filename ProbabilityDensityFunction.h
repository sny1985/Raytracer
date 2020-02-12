#ifndef _PROBABILITYDENSITYFUNCTION_
#define _PROBABILITYDENSITYFUNCTION_

#include "Vector.h"
#include "ONB.h"

namespace SNY
{
inline Vector3R GenerateRandomCosineDirection()
{
    REAL r1 = RandomReal();
    REAL r2 = RandomReal();
    REAL phi = glm::two_pi<REAL>() * r1;
    REAL sqrtr2 = glm::sqrt(r2);
    return Vector3R(glm::cos(phi) * sqrtr2,
                    glm::sin(phi) * sqrtr2,
                    glm::sqrt(1.0 - r2));
}

inline Vector3R GenerateRandomToSphere(REAL radius, REAL squaredDistance)
{
    REAL r1 = RandomReal();
    REAL r2 = RandomReal();
    REAL phi = glm::two_pi<REAL>() * r1;
    REAL z = 1.0 + r2 * (sqrt(1.0 - radius * radius / squaredDistance) - 1.0);
    REAL sqrt1_zz = glm::sqrt(1.0 - z * z);
    return Vector3R(cos(phi) * sqrt1_zz,
                    sin(phi) * sqrt1_zz,
                    z);
}

class PDF
{
public:
    virtual ~PDF(){};
    virtual REAL GetPDF(const Vector3R &direction) const = 0;
    virtual Vector3R GenerateDirection() const = 0;
    virtual void DebugOutput() const = 0;
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vector3R &w) : uvw(w) {}
    virtual ~CosinePDF() {}
    virtual REAL GetPDF(const Vector3R &direction) const
    {
        REAL cosine = glm::dot(glm::normalize(direction), uvw.W);
        if (cosine > 0)
        {
            return cosine * glm::one_over_pi<REAL>();
        }
        else
        {
            return 0;
        }
    }
    virtual Vector3R GenerateDirection() const
    {
        return uvw.TransformFromLocal(GenerateRandomCosineDirection());
    }
    virtual void DebugOutput() const
    {
        cout << "U: " << uvw.U << ", V: " << uvw.V << ", W: " << uvw.W << endl;
    }

    ONB uvw;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(shared_ptr<const Hittable> pH, const Vector3R &ori) : pHittable(pH), origin(ori) {}
    virtual ~HittablePDF() {}
    virtual REAL GetPDF(const Vector3R &direction) const
    {
        return pHittable->ComputePDF(origin, direction);
    }
    virtual Vector3R GenerateDirection() const
    {
        return pHittable->GenerateRandomDirection(origin);
    }
    virtual void DebugOutput() const
    {
        cout << "origin: " << origin << endl;
    }

    Vector3R origin = Vector3R(0, 0, 0);
    shared_ptr<const Hittable> pHittable = nullptr;
};

class MixturePDF : public PDF
{
public:
    MixturePDF(shared_ptr<const PDF> p1, shared_ptr<const PDF> p2) : pPDF1(p1), pPDF2(p2) {}
    virtual ~MixturePDF() {}
    virtual REAL GetPDF(const Vector3R &direction) const
    {
        return 0.5 * (pPDF1->GetPDF(direction) + pPDF2->GetPDF(direction));
    }
    virtual Vector3R GenerateDirection() const
    {
        if (RandomReal() < 0.5)
        {
            return pPDF1->GenerateDirection();
        }
        else
        {
            return pPDF2->GenerateDirection();
        }
    }
    virtual void DebugOutput() const
    {
        cout << pPDF1 << ", " << pPDF2 << endl;
    }

    shared_ptr<const PDF> pPDF1;
    shared_ptr<const PDF> pPDF2;
};
} // namespace SNY

#endif
