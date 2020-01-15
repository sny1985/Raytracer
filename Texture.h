#ifndef _TEXTURE_
#define _TEXTURE_

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Vector.h"

namespace SNY
{
class Texture
{
public:
    Texture() {}
    virtual ~Texture() {}
    virtual Vector3R ComputeColor(const Vector2R &uv, const Vector3R &pos) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() {}
    ConstantTexture(const Vector3R &c) : color(c) {}
    virtual ~ConstantTexture() {}
    virtual Vector3R ComputeColor(const Vector2R &uv, const Vector3R &pos) const
    {
        return color;
    }

    Vector3R color = Vector3R(0, 0, 0);
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() {}
    CheckerTexture(Texture *t1, Texture *t2) : pOdd(t1), pEven(t2)
    {
        assert(pOdd);
        assert(pEven);
    }
    virtual ~CheckerTexture() {}
    virtual Vector3R ComputeColor(const Vector2R &uv, const Vector3R &pos) const
    {
        REAL sines = sin(10 * pos.x) * sin(10 * pos.y) * sin(10 * pos.z);
        if (sines < 0)
        {
            return pOdd->ComputeColor(uv, pos);
        }
        else
        {
            return pEven->ComputeColor(uv, pos);
        }
    }

    shared_ptr<Texture> pOdd = nullptr;
    shared_ptr<Texture> pEven = nullptr;
};

class PerlinTexture : public Texture
{
public:
public:
    PerlinTexture() {}
    PerlinTexture(REAL sc) : scale(sc) {}
    virtual ~PerlinTexture() {}
    virtual Vector3R ComputeColor(const Vector2R &uv, const Vector3R &pos) const
    {
        return Vector3R(1.0, 1.0, 1.0) * stb_perlin_noise3(scale * pos.x, scale * pos.y, scale * pos.z, 0, 0, 0);
    }

    REAL scale = 1.0;
};

class ImageTexture : public Texture
{
public:
    ImageTexture() {}
    ImageTexture(const wstring &filePath)
    {
        int w, h, nc;
        pData = stbi_load(s_StrWStrConverter.to_bytes(filePath).c_str(), &w, &h, &nc, 3);
        if (pData)
        {
            width = w;
            height = h;
        }
        else
        {
            cerr << "Failed to open image" << endl;
            stbi_image_free(pData);
            pData = nullptr;
        }
    }
    virtual ~ImageTexture()
    {
        if (pData)
        {
            stbi_image_free(pData);
        }
    }
    virtual Vector3R ComputeColor(const Vector2R &uv, const Vector3R &pos) const
    {
        if (!pData)
        {
            return Vector3R(0, 0, 0);
        }

        size_t i = uv.x * width;
        size_t j = (1.0 - uv.y) * height - 0.001;
        if (i < 0)
        {
            i = 0;
        }
        if (j < 0)
        {
            j = 0;
        }
        if (i > width - 1)
        {
            i = width - 1;
        }
        if (j > height - 1)
        {
            j = height - 1;
        }
        size_t index = 3 * (width * j + i);
        return Vector3R(pData[index] / 255.0, pData[index + 1] / 255.0, pData[index + 2] / 255.0);
    }

    UCHAR *pData = nullptr;
    size_t width = 0;
    size_t height = 0;
};
} // namespace SNY

#endif
