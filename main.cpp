#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Common.h"
#include "Utility.h"
#include "Vector.h"
#include "Camera.h"
#include "HittableList.h"
#include "BVH.h"
#include "Sphere.h"
#include "AARect.h"
#include "Box.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Volume.h"
#include "ThreadPool.h"

using namespace SNY;

const size_t maxDepth = 5;
const size_t imageWidth = 600;
const size_t imageHeight = 400;
const size_t tileWidth = 64;
const size_t tileHeight = 64;
const size_t colorComponents = 3;
const size_t samples = 50;

void GenerateSimpleTextureScene(HittableList &list)
{
    list.Add(new Sphere(Vector3R(0, 0, -1.0), Vector3R(0.2, 0, -1.0), 0.5, 0, 0, new Lambertian(new CheckerTexture(new ConstantTexture(Vector3R(0.2, 0.3, 0.1)), new ConstantTexture(Vector3R(0.9, 0.9, 0.9))))));
    list.Add(new Sphere(Vector3R(0, -100.5, -1.0), Vector3R(0, -100.5, -1.0), 100.0, 0, 0, new Lambertian(new ImageTexture(L"earthmap.jpg"))));
    list.Add(new Sphere(Vector3R(1.0, 0, -1.0), Vector3R(1.2, 0, -1.0), 0.5, 0, 0, new Metal(new ConstantTexture(Vector3R(0.8, 0.6, 0.2)), 0.3)));
    list.Add(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), 0.5, 0, 0, new Dielectric(1.5)));
    list.Add(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), -0.45, 0, 0, new Dielectric(1.5)));
}

void GenerateSimpleLightScene(HittableList &list)
{
    list.Add(new Sphere(Vector3R(0, -1000.0, 0), Vector3R(0, -1000.0, 0), 1000.0, 0, 0, new Lambertian(new PerlinTexture(4))));
    list.Add(new Sphere(Vector3R(0, 2.0, 0), Vector3R(0, 2.0, 0), 2.0, 0, 0, new Lambertian(new PerlinTexture(2.0))));
    // list.Add(new Sphere(Vector3R(0, 7.0, 0), Vector3R(0, 7.0, 0), 2.0, 0, 0, new DiffuseLight(new ConstantTexture(Vector3R(4.0, 4.0, 4.0)))));
    list.Add(new AARect(3.0, 5.0, 1.0, 3.0, -2.0, ZAxis, false, shared_ptr<Material>(new DiffuseLight(new ConstantTexture(Vector3R(4.0, 4.0, 4.0))))));
}

void GenerateCornelBoxScene(HittableList &list)
{
    shared_ptr<Material> pRed(new Lambertian(new ConstantTexture(Vector3R(0.65, 0.05, 0.05))));
    shared_ptr<Material> pWhite(new Lambertian(new ConstantTexture(Vector3R(0.73, 0.73, 0.73))));
    shared_ptr<Material> pGreen(new Lambertian(new ConstantTexture(Vector3R(0.12, 0.45, 0.15))));
    shared_ptr<Material> pLight(new DiffuseLight(new ConstantTexture(Vector3R(15.0, 15.0, 15.0))));
    list.Add(new AARect(0, 555, 0, 555, 555, XAxis, true, pGreen));
    list.Add(new AARect(0, 555, 0, 555, 0, XAxis, false, pRed));
    list.Add(new AARect(213, 343, 227, 332, 554, YAxis, false, pLight));
    list.Add(new AARect(0, 555, 0, 555, 555, YAxis, true, pWhite));
    list.Add(new AARect(0, 555, 0, 555, 0, YAxis, false, pWhite));
    list.Add(new AARect(0, 555, 0, 555, 555, ZAxis, true, pWhite));

    // list.Add(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pWhite));
    // list.Add(new Translation(new Rotation(new Scaling(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pWhite), Vector3R(1.0, 1.0, 1.5)), glm::radians(-18.0), YAxis), Vector3R(130.0, 0, 65.0)));
    list.Add(new Translation(new Rotation(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pWhite), glm::radians(-18.0), YAxis), Vector3R(130.0, 0, 65.0)));
    list.Add(new Translation(new Rotation(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 330.0, 165.0), pWhite), glm::radians(15.0), YAxis), Vector3R(265.0, 0, 295.0)));
}

void GenerateSmokeCornelBoxScene(HittableList &list)
{
    shared_ptr<Material> pRed(new Lambertian(new ConstantTexture(Vector3R(0.65, 0.05, 0.05))));
    shared_ptr<Material> pWhite(new Lambertian(new ConstantTexture(Vector3R(0.73, 0.73, 0.73))));
    shared_ptr<Material> pGreen(new Lambertian(new ConstantTexture(Vector3R(0.12, 0.45, 0.15))));
    shared_ptr<Material> pLight(new DiffuseLight(new ConstantTexture(Vector3R(15.0, 15.0, 15.0))));
    list.Add(new AARect(0, 555, 0, 555, 555, XAxis, true, pGreen));
    list.Add(new AARect(0, 555, 0, 555, 0, XAxis, false, pRed));
    list.Add(new AARect(213, 343, 227, 332, 554, YAxis, false, pLight));
    list.Add(new AARect(0, 555, 0, 555, 555, YAxis, true, pWhite));
    list.Add(new AARect(0, 555, 0, 555, 0, YAxis, false, pWhite));
    list.Add(new AARect(0, 555, 0, 555, 555, ZAxis, true, pWhite));

    list.Add(new ConstantMedium(new Translation(new Rotation(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pWhite), glm::radians(-18.0), YAxis), Vector3R(130.0, 0, 65.0)), new ConstantTexture(Vector3R(1.0, 1.0, 1.0)), 0.01));
    list.Add(new ConstantMedium(new Translation(new Rotation(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 330.0, 165.0), pWhite), glm::radians(15.0), YAxis), Vector3R(265.0, 0, 295.0)), new ConstantTexture(Vector3R(0, 0, 0)), 0.01));
}

Vector3R ComputeColor(const Ray &ray, const Hittable *pWorld, size_t depth)
{
    HitInfo hi;
    if (pWorld->Hit(ray, 0.001, FLT_MAX, hi))
    {
        Vector3R emitted = hi.pMaterial->Emit(hi);
        if (depth < maxDepth)
        {
            Ray scattered;
            Vector3R attenuation = hi.pMaterial->Scatter(ray, hi, scattered);
            if (attenuation != Vector3R(0, 0, 0))
            {
                return emitted + attenuation * ComputeColor(scattered, pWorld, depth + 1);
            }
        }
        return emitted;
    }
    else
    {
        Vector3R unitDirection = glm::normalize(ray.direction);
        REAL k = 0.5 * (unitDirection.y + 1.0);
        return (1.0f - k) * Vector3R(1.0, 1.0, 1.0) + k * Vector3R(0.5, 0.7, 1.0);
        // return Vector3R(0, 0, 0);
    }
}

size_t ComputeTile(const Camera &camera, const HittableList &list, vector<UCHAR> &data, size_t i, size_t j, size_t id)
{
    for (size_t ii = 0; ii < tileHeight; ++ii)
    {
        for (size_t jj = 0; jj < tileWidth; ++jj)
        {
            size_t row = i + ii;
            if (row >= imageHeight)
                continue;
            size_t col = j + jj;
            if (col >= imageWidth)
                continue;

            Vector3R c(0, 0, 0);
            for (size_t k = 0; k < samples; ++k)
            {
                REAL u = (col + RandomReal()) / imageWidth;
                REAL v = (row + RandomReal()) / imageHeight;
                c += ComputeColor(camera.ShootRay(u, v), &list /*pWorld.get()*/, 0);
            }
            c /= REAL(samples);

            size_t index = ((imageHeight - row - 1) * imageWidth + col) * colorComponents;
            data[index] = int(255.99 * sqrt(c.x));
            data[index + 1] = int(255.99 * sqrt(c.y));
            data[index + 2] = int(255.99 * sqrt(c.z));
        }
    }

    return id;
}

int main(int argc, char *argv[])
{
    vector<UCHAR> data(imageWidth * imageHeight * colorComponents);

    // Vector3R camPos(3.0, 3.0, 2.0);
    // Vector3R lookAt(0.0, 0.0, -1.0);
    Vector3R camPos(278.0, 278.0, -800.0);
    Vector3R lookAt(278.0, 278.0, 0);
    REAL focalLength = 10.0;
    REAL aperture = 0.0;
    REAL fov = 40.0;
    Camera camera(camPos, lookAt, Vector3R(0, 1.0, 0), fov, float(imageWidth) / float(imageHeight), aperture, focalLength, 0, 1.0);

    HittableList list;
    // GenerateSimpleTextureScene(list);
    // GenerateSimpleLightScene(list);
    GenerateCornelBoxScene(list);
    // GenerateSmokeCornelBoxScene(list);
    StartTiming("BVH building");
    // shared_ptr<Hittable> pWorld(new BVHNode(list.list.data(), list.list.size(), 0, 1.0, true));
    EndTiming("BVH building");

    StartTiming("Ray tracing");
    WaitableThreadPool threadPool(16);
    vector<future<size_t>> futures;
    size_t id = 0;
    for (size_t i = 0; i < imageHeight; i += tileHeight)
    {
        for (size_t j = 0; j < imageWidth; j += tileWidth)
        {
            futures.push_back(threadPool.Submit(bind(ComputeTile, ref(camera), ref(list), ref(data), i, j, id)));
            // cout << "task " << id << " is running..." << endl;
            ++id;
        }
    }
    // cout << "number of tasks: " << id << endl;
    for (size_t i = 0; i < futures.size(); ++i)
    {
        futures[i].get();
        // cout << "task " << futures[i].get() << " finished" << endl;
    }
    stbi_write_bmp("raytracer.bmp", imageWidth, imageHeight, colorComponents, data.data());
    EndTiming("Ray tracing");

    return 0;
}
