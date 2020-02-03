#ifndef _MATERIAL_
#define _MATERIAL_

#include "Utility.h"
#include "Texture.h"
#include "Ray.h"
#include "Hittable.h"

namespace SNY
{
inline Vector3R GenerateRandomPointOnUnitSphere()
{
    Vector3R p;
    do
    {
        p = 2.0f * Vector3R(RandomReal(), RandomReal(), RandomReal()) - Vector3R(1.0, 1.0, 1.0);
    } while (glm::dot(p, p) >= 1.0);
    return p;
}

inline Vector3R Reflect(const Vector3R &v, const Vector3R &n)
{
    /*
        Bram de Greve. Reflections and Refractions in Ray Tracing. 2006
    */
    return v - 2.0f * glm::dot(v, n) * n;
}

inline bool Refract(const Vector3R &v, const Vector3R &n, float eta, Vector3R &refracted)
{
    /*
        Bram de Greve. Reflections and Refractions in Ray Tracing. 2006
    */
    Vector3R unitV = glm::normalize(v);
    REAL dt = glm::dot(unitV, n);
    REAL discriminant = 1.0 - eta * eta * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = eta * (unitV - n * dt) - n * glm::sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

inline REAL Schlick(REAL cosine, REAL refractiveIndex)
{
    REAL r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}

inline bool Refract2(const Vector3R &v, const Vector3R &n, REAL eta1, REAL eta2, Vector3R &refracted)
{
    /*
        Bram de Greve. Reflections and Refractions in Ray Tracing. 2006
    */
    REAL eta = eta1 / eta2;
    REAL cosI = -glm::dot(v, n);
    REAL sinT2 = eta * eta * (1.0 - cosI * cosI);
    if (sinT2 > 1.0)
    {
        return false;
    }
    REAL cosT = glm::sqrt(1.0 - sinT2);
    refracted = eta * v + (eta * cosI - cosT) * n;
    return true;
}

inline REAL Schlick2(REAL cosI, REAL eta1, REAL eta2)
{
    /*
        Bram de Greve. Reflections and Refractions in Ray Tracing. 2006
    */
    REAL r0 = (eta1 - eta2) / (eta1 + eta2);
    r0 = r0 * r0;
    if (eta1 > eta2)
    {
        REAL eta = eta1 / eta2;
        REAL sinT2 = eta * eta * (1.0 - cosI * cosI);
        if (sinT2 > 1.0) // TIR
        {
            return 1.0;
        }
        cosI = glm::sqrt(1.0 - sinT2);
    }
    REAL x = 1.0 - cosI;
    return r0 + (1 - r0) * x * x * x * x * x;
}

class Material
{
public:
    Material() {}
    virtual ~Material() {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const = 0;
    virtual Vector3R Emit(const HitInfo &hi) const
    {
        return Vector3R(0, 0, 0);
    }
};

class Lambertian : public Material
{
public:
    Lambertian() {}
    Lambertian(Texture *pTex) : pTexture(pTex)
    {
        assert(pTexture);
    }
    virtual ~Lambertian() {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const
    {
        out = Ray(hi.position, hi.normal + GenerateRandomPointOnUnitSphere(), in.time);
        return pTexture->ComputeColor(hi.uv, hi.position);
    }

    shared_ptr<Texture> pTexture = nullptr;
};

class Metal : public Material
{
public:
    Metal() {}
    Metal(Texture *pTex, REAL f) : pTexture(pTex), fuzz(f)
    {
        assert(pTexture);
        fuzz = fuzz < 1.0 ? fuzz : 1.0;
    }
    virtual ~Metal() {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const
    {
        out = Ray(hi.position, Reflect(in.direction, hi.normal) + fuzz * GenerateRandomPointOnUnitSphere(), in.time);
        return pTexture->ComputeColor(hi.uv, hi.position);
    }

    shared_ptr<Texture> pTexture = nullptr;
    REAL fuzz = 0;
};

class Dielectric : public Material
{
public:
    Dielectric() {}
    Dielectric(REAL ri) : refractiveIndex(ri) {}
    virtual ~Dielectric() {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const
    {
        Vector3R outwardNormal;
        REAL eta;
        REAL cosine;
        if (glm::dot(in.direction, hi.normal) > 0)
        {
            outwardNormal = -hi.normal;
            eta = refractiveIndex;
            cosine = dot(in.direction, hi.normal) / glm::length(in.direction);
            cosine = glm::sqrt(1 - refractiveIndex * refractiveIndex * (1 - cosine * cosine));
        }
        else
        {
            outwardNormal = hi.normal;
            eta = 1.0 / refractiveIndex;
            cosine = -glm::dot(in.direction, hi.normal) / glm::length(in.direction);
        }

        Vector3R reflected = Reflect(in.direction, hi.normal);
        Vector3R refracted;
        REAL reflectProb;
        if (Refract(in.direction, outwardNormal, eta, refracted))
        {
            reflectProb = Schlick(cosine, refractiveIndex);
        }
        else
        {
            reflectProb = 1.0;
        }

        // It randomly chooses between reflection or refraction and only generates one scattered ray per interaction.
        if (RandomReal() < reflectProb)
        {
            out = Ray(hi.position, reflected, in.time);
        }
        else
        {
            out = Ray(hi.position, refracted, in.time);
        }

        return Vector3R(1.0, 1.0, 1.0);
    }

    // virtual Vector3R Scatter2(const Ray &in, const HitInfo &hi, Ray &out) const
    // {
    // }

    REAL refractiveIndex = 1.0;
};

class Isotropic : public Material
{
public:
    Isotropic(Texture *pTex) : pTexture(pTex) {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const
    {
        out = Ray(hi.position, GenerateRandomPointOnUnitSphere(), in.time);
        return pTexture->ComputeColor(hi.uv, hi.position);
    }

    shared_ptr<Texture> pTexture = nullptr;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight() {}
    DiffuseLight(Texture *pTex) : pTexture(pTex) {}
    virtual ~DiffuseLight() {}
    virtual Vector3R Scatter(const Ray &in, const HitInfo &hi, Ray &out) const
    {
        return Vector3R(0, 0, 0);
    }
    virtual Vector3R Emit(const HitInfo &hi) const
    {
        return pTexture->ComputeColor(hi.uv, hi.position);
    }

    shared_ptr<Texture> pTexture = nullptr;
};
} // namespace SNY

#endif
