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

const size_t maxDepth = 10;
const size_t imageWidth = 800;
const size_t imageHeight = 800;
const size_t tileWidth = 64;
const size_t tileHeight = 64;
const size_t colorComponents = 3;
const size_t samples = 150;
const size_t threads = 16;

void GenerateSimpleTextureScene(shared_ptr<HittableList> pWorld, shared_ptr<HittableList> pLights, Camera &cam)
{
    shared_ptr<const Texture> pBrightTex(new ConstantTexture(Vector3R(0.9, 0.9, 0.9)));
    shared_ptr<const Texture> pDarkTex(new ConstantTexture(Vector3R(0.2, 0.3, 0.1)));
    shared_ptr<const Texture> pBlueTex(new ConstantTexture(Vector3R(0.1, 0.2, 0.5)));
    shared_ptr<const Texture> pYellowTex1(new ConstantTexture(Vector3R(0.8, 0.8, 0)));
    shared_ptr<const Texture> pYellowTex2(new ConstantTexture(Vector3R(0.8, 0.6, 0.2)));
    shared_ptr<const Texture> pCheckerTex(new CheckerTexture(pDarkTex, pBrightTex));
    shared_ptr<const Texture> pImageTex(new ImageTexture(L"earthmap.jpg"));
    shared_ptr<const Texture> pLightTex(new ConstantTexture(Vector3R(1.0, 1.0, 1.0)));

    shared_ptr<const Material> pYellowLambertianMat(new Lambertian(pYellowTex1));
    shared_ptr<const Material> pBlueLambertianMat(new Lambertian(pBlueTex));
    shared_ptr<const Material> pEarthLambertianMat(new Lambertian(pImageTex));
    shared_ptr<const Material> pCheckerLambertianMat(new Lambertian(pCheckerTex));
    shared_ptr<const Material> pMetalMat(new Metal(pYellowTex2, 0.3));
    shared_ptr<const Material> pGlassMat(new Dielectric(1.5));
    shared_ptr<const Material> pDiffuseLightMat(new DiffuseLight(pLightTex));

    shared_ptr<const Hittable> pSphere1(new Sphere(Vector3R(0, -100.5, -1.0), Vector3R(0, -100.5, -1.0), 100.0, 0, 0, pYellowLambertianMat));
    shared_ptr<const Hittable> pSphere2(new Sphere(Vector3R(0, 0, -1.0), Vector3R(0.2, 0, -1.0), 0.5, 0, 0, pBlueLambertianMat));
    shared_ptr<const Hittable> pSphere3(new Sphere(Vector3R(1.0, 0, -1.0), Vector3R(1.2, 0, -1.0), 0.5, 0, 0, pMetalMat));
    shared_ptr<const Hittable> pSphere4(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), 0.5, 0, 0, pGlassMat));
    shared_ptr<const Hittable> pSphere5(new Sphere(Vector3R(-1.0, 0, -1.0), Vector3R(-0.8, 0, -1.0), -0.45, 0, 0, pGlassMat));
    shared_ptr<const Hittable> pLight(new AARect(-1000, 1000, -1000, 1000, 1000, YAxis, false, pDiffuseLightMat));

    pWorld->Add(pSphere1);
    pWorld->Add(pSphere2);
    pWorld->Add(pSphere3);
    pWorld->Add(pSphere4);
    pWorld->Add(pSphere5);
    pWorld->Add(pLight);

    pLights->Add(pLight);

    Vector3R camPos(3.0, 3.0, 2.0);
    Vector3R lookAt(0.0, 0.0, -1.0);
    REAL focalLength = 10.0;
    REAL aperture = 0.0;
    REAL fov = 40.0;
    cam = Camera(camPos, lookAt, Vector3R(0, 1.0, 0), fov, float(imageWidth) / float(imageHeight), aperture, focalLength, 0, 1.0);
}

void GenerateSimpleLightScene(shared_ptr<HittableList> pWorld, shared_ptr<HittableList> pLights, Camera &cam)
{
    shared_ptr<const Texture> pLightTex(new ConstantTexture(Vector3R(4.0, 4.0, 4.0)));
    shared_ptr<const Texture> pPerlinTex1(new PerlinTexture(4));
    shared_ptr<const Texture> pPerlinTex2(new PerlinTexture(2));

    shared_ptr<const Material> pPerlinLambertianMat1(new Lambertian(pPerlinTex1));
    shared_ptr<const Material> pPerlinLambertianMat2(new Lambertian(pPerlinTex2));
    shared_ptr<const Material> pDiffuseLightMat(new DiffuseLight(pLightTex));

    shared_ptr<const Hittable> pSphere1(new Sphere(Vector3R(0, -1000.0, 0), Vector3R(0, -1000.0, 0), 1000.0, 0, 0, pPerlinLambertianMat1));
    shared_ptr<const Hittable> pSphere2(new Sphere(Vector3R(0, 2.0, 0), Vector3R(0, 2.0, 0), 2.0, 0, 0, pPerlinLambertianMat2));
    shared_ptr<const Hittable> pRectLight(new AARect(3.0, 5.0, 1.0, 3.0, -2.0, ZAxis, false, pDiffuseLightMat));
    shared_ptr<const Hittable> pSphereLight(new Sphere(Vector3R(0, 7.0, 0), Vector3R(0, 7.0, 0), 2.0, 0, 0, pDiffuseLightMat));

    pWorld->Add(pSphere1);
    pWorld->Add(pSphere2);
    pWorld->Add(pRectLight);
    pWorld->Add(pSphereLight);

    pLights->Add(pRectLight);
    pLights->Add(pSphereLight);

    Vector3R camPos(20.0, 3.0, 3.0);
    Vector3R lookAt(0.0, 0.0, -1.0);
    REAL focalLength = 10.0;
    REAL aperture = 0.0;
    REAL fov = 40.0;
    cam = Camera(camPos, lookAt, Vector3R(0, 1.0, 0), fov, float(imageWidth) / float(imageHeight), aperture, focalLength, 0, 1.0);
}

void GenerateCornelBoxScene(shared_ptr<HittableList> pWorld, shared_ptr<HittableList> pLights, Camera &cam)
{
    shared_ptr<const Texture> pGreenTex(new ConstantTexture(Vector3R(0.12, 0.45, 0.15)));
    shared_ptr<const Texture> pRedTex(new ConstantTexture(Vector3R(0.65, 0.05, 0.05)));
    shared_ptr<const Texture> pWhiteTex(new ConstantTexture(Vector3R(0.73, 0.73, 0.73)));
    shared_ptr<const Texture> pBrightTex(new ConstantTexture(Vector3R(15.0, 15.0, 15.0)));
    shared_ptr<const Texture> pGrayTex(new ConstantTexture(Vector3R(0.8, 0.85, 0.88)));

    shared_ptr<const Material> pGreenMat(new Lambertian(pGreenTex));
    shared_ptr<const Material> pRedMat(new Lambertian(pRedTex));
    shared_ptr<const Material> pWhiteMat(new Lambertian(pWhiteTex));
    shared_ptr<const Material> pDiffuseLightMat(new DiffuseLight(pBrightTex));
    shared_ptr<const Material> pMirrorMat(new Metal(pGrayTex, 0));
    shared_ptr<const Material> pGlassMat(new Dielectric(1.5));

    shared_ptr<const Hittable> pLeft(new AARect(0, 555, 0, 555, 555, XAxis, true, pGreenMat));
    shared_ptr<const Hittable> pRight(new AARect(0, 555, 0, 555, 0, XAxis, false, pRedMat));
    shared_ptr<const Hittable> pTop(new AARect(0, 555, 0, 555, 555, YAxis, true, pWhiteMat));
    shared_ptr<const Hittable> pBottom(new AARect(0, 555, 0, 555, 0, YAxis, false, pWhiteMat));
    shared_ptr<const Hittable> pBack(new AARect(0, 555, 0, 555, 555, ZAxis, true, pWhiteMat));
    shared_ptr<const Hittable> pLight(new AARect(213, 343, 227, 332, 554, YAxis, false, pDiffuseLightMat));
    shared_ptr<const Hittable> pSphere(new Sphere(Vector3R(190, 90, 190), Vector3R(), 90.0, 0, 0, pGlassMat));

    shared_ptr<const Hittable> pSmallBox(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pWhiteMat));
    shared_ptr<const Hittable> pBigBox(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 330.0, 165.0), pWhiteMat));
    shared_ptr<const Hittable> pFrontBox(shared_ptr<const Hittable>(new Translation(shared_ptr<const Hittable>(new Rotation(pSmallBox, glm::radians(-18.0), YAxis)), Vector3R(130.0, 0, 65.0))));
    shared_ptr<const Hittable> pBackBox(shared_ptr<const Hittable>(new Translation(shared_ptr<const Hittable>(new Rotation(pBigBox, glm::radians(15.0), YAxis)), Vector3R(265.0, 0, 295.0))));

    pWorld->Add(pLeft);
    pWorld->Add(pRight);
    pWorld->Add(pTop);
    pWorld->Add(pBottom);
    pWorld->Add(pBack);
    pWorld->Add(pLight);
    // pWorld->Add(pFrontBox);
    pWorld->Add(pBackBox);
    pWorld->Add(pSphere);

    pLights->Add(pLight);
    pLights->Add(pSphere);

    Vector3R camPos(278.0, 278.0, -800.0);
    Vector3R lookAt(278.0, 278.0, 0);
    REAL focalLength = 10.0;
    REAL aperture = 0.0;
    REAL fov = 40.0;
    cam = Camera(camPos, lookAt, Vector3R(0, 1.0, 0), fov, float(imageWidth) / float(imageHeight), aperture, focalLength, 0, 1.0);
}

void GenerateSmokeCornelBoxScene(shared_ptr<HittableList> pWorld, shared_ptr<HittableList> pLights, Camera &cam)
{
    shared_ptr<const Texture> pBlackTex(new ConstantTexture(Vector3R(0, 0, 0)));
    shared_ptr<const Texture> pWhiteTex(new ConstantTexture(Vector3R(1.0, 1.0, 1.0)));
    shared_ptr<const Texture> pGreenTex(new ConstantTexture(Vector3R(0.12, 0.45, 0.15)));
    shared_ptr<const Texture> pRedTex(new ConstantTexture(Vector3R(0.65, 0.05, 0.05)));
    shared_ptr<const Texture> pGrayTex(new ConstantTexture(Vector3R(0.73, 0.73, 0.73)));
    shared_ptr<const Texture> pLightTex(new ConstantTexture(Vector3R(15.0, 15.0, 15.0)));

    shared_ptr<const Material> pGreenMat(new Lambertian(pGreenTex));
    shared_ptr<const Material> pRedMat(new Lambertian(pRedTex));
    shared_ptr<const Material> pGrayMat(new Lambertian(pGrayTex));
    shared_ptr<const Material> pDiffuseLightMat(new DiffuseLight(pLightTex));

    shared_ptr<const Hittable> pLeft(new AARect(0, 555, 0, 555, 555, XAxis, true, pGreenMat));
    shared_ptr<const Hittable> pRight(new AARect(0, 555, 0, 555, 0, XAxis, false, pRedMat));
    shared_ptr<const Hittable> pTop(new AARect(0, 555, 0, 555, 555, YAxis, true, pGrayMat));
    shared_ptr<const Hittable> pBottom(new AARect(0, 555, 0, 555, 0, YAxis, false, pGrayMat));
    shared_ptr<const Hittable> pBack(new AARect(0, 555, 0, 555, 555, ZAxis, true, pGrayMat));
    shared_ptr<const Hittable> pLight(new AARect(213, 343, 227, 332, 554, YAxis, false, pDiffuseLightMat));

    shared_ptr<const Hittable> pSmallBox(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 165.0, 165.0), pGrayMat));
    shared_ptr<const Hittable> pBigBox(new Box(Vector3R(0, 0, 0), Vector3R(165.0, 330.0, 165.0), pGrayMat));
    shared_ptr<const Hittable> pFrontBox(shared_ptr<const Hittable>(new Translation(shared_ptr<const Hittable>(new Rotation(pSmallBox, glm::radians(-18.0), YAxis)), Vector3R(130.0, 0, 65.0))));
    shared_ptr<const Hittable> pBackBox(shared_ptr<const Hittable>(new Translation(shared_ptr<const Hittable>(new Rotation(pBigBox, glm::radians(15.0), YAxis)), Vector3R(265.0, 0, 295.0))));
    shared_ptr<const Hittable> pWhiteBox(shared_ptr<const Hittable>(new ConstantMedium(pFrontBox, pWhiteTex, 0.01)));
    shared_ptr<const Hittable> pBlackBox(shared_ptr<const Hittable>(new ConstantMedium(pBackBox, pBlackTex, 0.01)));

    pWorld->Add(pLeft);
    pWorld->Add(pRight);
    pWorld->Add(pTop);
    pWorld->Add(pBottom);
    pWorld->Add(pBack);
    pWorld->Add(pLight);
    pWorld->Add(pWhiteBox);
    pWorld->Add(pBlackBox);

    pLights->Add(pLight);

    Vector3R camPos(278.0, 278.0, -800.0);
    Vector3R lookAt(278.0, 278.0, 0);
    REAL focalLength = 10.0;
    REAL aperture = 0.0;
    REAL fov = 40.0;
    cam = Camera(camPos, lookAt, Vector3R(0, 1.0, 0), fov, float(imageWidth) / float(imageHeight), aperture, focalLength, 0, 1.0);
}

Vector3R ComputeColor(const Ray &ray, shared_ptr<const Hittable> pWorld, shared_ptr<const Hittable> pLights, size_t depth)
{
    HitInfo hi;
    if (pWorld->Hit(ray, 0.001, FLT_MAX, hi))
    {
        Vector3R emitted = hi.pMaterial->Emit(hi);
        if (depth < maxDepth)
        {
            ScatterInfo si;
            Vector3R attenuation = hi.pMaterial->Scatter(ray, hi, si);
            if (attenuation != Vector3R(0, 0, 0))
            {
                if (si.isSpecular || si.isRefraction || si.isIsotropic)
                {
                    return attenuation * ComputeColor(si.ray, pWorld, pLights, depth + 1);
                }
                else
                {
                    shared_ptr<const PDF> pLightPDF(new HittablePDF(pLights, hi.position));
                    shared_ptr<const PDF> pMixedPDF(new MixturePDF(pLightPDF, si.pPDF));
                    Ray scatteredRay(hi.position, pMixedPDF->GenerateDirection(), ray.time);
                    REAL pdf = pMixedPDF->GetPDF(scatteredRay.direction);
                    return emitted + attenuation * ComputeColor(scatteredRay, pWorld, pLights, depth + 1) * hi.pMaterial->GetScatteringPDF(ray, hi, scatteredRay) / pdf;
                }
            }
        }
        return emitted;
    }
    else
    {
        return Vector3R(0, 0, 0);
    }
}

size_t ComputeTile(const Camera &camera, shared_ptr<const Hittable> pWorld, shared_ptr<const Hittable> pLights, vector<UCHAR> &data, size_t i, size_t j, size_t id)
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
                c += DeNaN(ComputeColor(camera.ShootRay(u, v), pWorld, pLights, 0));
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

    Camera camera;
    shared_ptr<HittableList> pWorld(new HittableList);
    shared_ptr<HittableList> pLights(new HittableList);
    // GenerateSimpleTextureScene(pWorld, pLights, camera);
    // GenerateSimpleLightScene(pWorld, pLights, camera);
    GenerateCornelBoxScene(pWorld, pLights, camera);
    // GenerateSmokeCornelBoxScene(pWorld, pLights, camera);
    // StartTiming("BVH building");
    // shared_ptr<const Hittable> pNewWorld(new BVHNode(pWorld->list.data(), pWorld->list.size(), 0, 1.0, true));
    // EndTiming("BVH building");

    StartTiming("Ray tracing");
    size_t id = 0;
    if (threads == 1)
    {
        for (size_t i = 0; i < imageHeight; i += tileHeight)
        {
            for (size_t j = 0; j < imageWidth; j += tileWidth)
            {
                ComputeTile(camera, pWorld, pLights, data, i, j, id);
                // cout << "task " << id << " is running..." << endl;
                ++id;
            }
        }
    }
    else
    {
        WaitableThreadPool threadPool(threads);
        vector<future<size_t>> futures;
        for (size_t i = 0; i < imageHeight; i += tileHeight)
        {
            for (size_t j = 0; j < imageWidth; j += tileWidth)
            {
                futures.push_back(threadPool.Submit(bind(ComputeTile, ref(camera), pWorld, pLights, ref(data), i, j, id)));
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
    }
    stbi_write_bmp("raytracer.bmp", imageWidth, imageHeight, colorComponents, data.data());
    EndTiming("Ray tracing");

    return 0;
}
