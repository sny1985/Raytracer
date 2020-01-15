#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Common.h"
#include "Utility.h"
#include "Vector.h"
#include "Camera.h"
#include "HittableList.h"
#include "BVH.h"
#include "Sphere.h"

using namespace SNY;

const int maxDepth = 10;

Vector3R ComputeColor(const Ray &ray, Hittable *world, int depth)
{
    HitInfo hi;
    if (world->Hit(ray, 0.001, FLT_MAX, hi))
    {
        Ray scattered;
        Vector3R attenuation;

        if (depth < maxDepth && hi.pMaterial->Scatter(ray, hi, attenuation, scattered))
        {
            return attenuation * ComputeColor(scattered, world, depth + 1);
        }
        else
        {
            return Vector3R(0, 0, 0);
        }
    }
    else
    {
        Vector3R unitDirection = glm::normalize(ray.direction);
        REAL k = 0.5 * (unitDirection.y + 1.0);
        return (1.0f - k) * Vector3R(1.0, 1.0, 1.0) + k * Vector3R(0.5, 0.7, 1.0);
    }
}

int main(int argc, char *argv[])
{
    size_t nw = 600;
    size_t nh = 400;
    size_t nc = 3;
    size_t ns = 32;

    vector<UCHAR> data(nw * nh * nc);

    Vector3R camPos(3.0, 3.0, 2.0);
    Vector3R lookAt(0, 0, -1.0);
    Camera camera(camPos, lookAt, Vector3R(0, 1.0, 0), 60.0, float(nw) / float(nh), 0, glm::length(camPos - lookAt), 0, 1.0);

    HittableList list;
    list.Add(new Sphere(Vector3R(0, 0, -1.0), Vector3R(0.2, 0, -1.0), 0.5, 0, 0, new Lambertian(new CheckerTexture(new ConstantTexture(Vector3R(0.2, 0.3, 0.1)), new ConstantTexture(Vector3R(0.9, 0.9, 0.9))))));
    list.Add(new Sphere(Vector3R(0, -100.5, -1.0), Vector3R(0, -100.5, -1.0), 100.0, 0, 0, new Lambertian(new ImageTexture(L"earthmap.jpg"))));
    list.Add(new Sphere(Vector3R(1.0, 0, -1.0), Vector3R(1.2, 0, -1.0), 0.5, 0, 0, new Metal(new ConstantTexture(Vector3R(0.8, 0.6, 0.2)), 0.3)));
    list.Add(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), 0.5, 0, 0, new Dielectric(1.5)));
    list.Add(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), -0.45, 0, 0, new Dielectric(1.5)));
    // Hittable *world = new BVHNode(list.list.data(), list.list.size(), 0, 1.0);
    // world->DebugOutput();
    Hittable *world = &list;

    StartTiming("Ray tracing");
    for (size_t i = 0; i < nh; ++i)
    {
        for (size_t j = 0; j < nw; ++j)
        {
            Vector3R c(0, 0, 0);
            for (size_t k = 0; k < ns; ++k)
            {
                REAL u = (j + RandomReal()) / nw;
                REAL v = (i + RandomReal()) / nh;
                c += ComputeColor(camera.ShootRay(u, v), world, 0);
            }
            c /= REAL(ns);

            size_t index = ((nh - i - 1) * nw + j) * nc;
            data[index] = int(255.99 * sqrt(c.x));
            data[index + 1] = int(255.99 * sqrt(c.y));
            data[index + 2] = int(255.99 * sqrt(c.z));
        }
    }
    stbi_write_bmp("raytracer.bmp", nw, nh, nc, data.data());
    EndTiming("Ray tracing");

    return 0;
}
