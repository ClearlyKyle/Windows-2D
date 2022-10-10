#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <windows.h>
#include <stdbool.h>
#include "./maths/maths.h"

typedef struct
{
    void      *Memory;
    BITMAPINFO Info;
    int        Width;
    int        Height;
    size_t     PixelCount;
} bitmap;

inline bool Bitmap_init(bitmap *Bitmap, HWND handle)
{
    memset((void *)Bitmap, 0, sizeof(bitmap));

    // Get client area dimensions
    RECT ClientRect;
    GetClientRect(handle, &ClientRect);
    Bitmap->Width  = ClientRect.right - ClientRect.left;
    Bitmap->Height = ClientRect.bottom - ClientRect.top;

    // Allocate memory for the bitmap
    const int BytesPerPixel = 4;
    Bitmap->PixelCount      = Bitmap->Width * Bitmap->Height * BytesPerPixel;

    Bitmap->Memory = VirtualAlloc(0,
                                  Bitmap->Width * Bitmap->Height * BytesPerPixel,
                                  MEM_RESERVE | MEM_COMMIT,
                                  PAGE_READWRITE);

    // BitmapInfo struct for StretchDIBits
    Bitmap->Info.bmiHeader.biSize  = sizeof(Bitmap->Info.bmiHeader);
    Bitmap->Info.bmiHeader.biWidth = Bitmap->Width;

    // Negative height makes top left as the coordinate system origin for the DrawPixel function, otherwise its bottom left
    Bitmap->Info.bmiHeader.biHeight      = -Bitmap->Height;
    Bitmap->Info.bmiHeader.biPlanes      = 1;
    Bitmap->Info.bmiHeader.biBitCount    = 32;
    Bitmap->Info.bmiHeader.biCompression = BI_RGB;

    return true;
}

inline void Bitmap_Draw_Pixel(const int X, const int Y, const unsigned int colour, bitmap *Bitmap)
{
    const int new_X = ((X % Bitmap->Width) + Bitmap->Width) % Bitmap->Width;
    const int new_Y = ((Y % Bitmap->Height) + Bitmap->Height) % Bitmap->Height;

    unsigned int *pixel = (unsigned int *)Bitmap->Memory;
    pixel += new_Y * Bitmap->Width + new_X;
    *pixel = colour;
}

void Bitmap_Draw_Rectangle(const int          RectangleX,
                           const int          RectangleY,
                           const int          RectangleWidth,
                           const int          RectangleHeight,
                           const unsigned int colour,
                           bitmap            *Bitmap)
{

    unsigned int *Pixel = (unsigned int *)Bitmap->Memory;
    Pixel += RectangleY * Bitmap->Width + RectangleX;

    for (int Y = 0; Y < RectangleHeight; ++Y)
    {
        for (int X = 0; X < RectangleWidth; ++X)
        {
            *Pixel++ = colour;
        }
        Pixel += Bitmap->Width - RectangleWidth;
    }
}

void Bitmap_Draw_Rectangle_Bordered(int RectangleX, int RectangleY,
                                    int RectangleWidth, int RectangleHeight,
                                    unsigned int colour,
                                    int BorderSize, unsigned int Bordercolour,
                                    bitmap *Bitmap)
{

    Bitmap_Draw_Rectangle(RectangleX,
                          RectangleY,
                          RectangleWidth,
                          RectangleHeight,
                          Bordercolour,
                          Bitmap);
    Bitmap_Draw_Rectangle(RectangleX + BorderSize,
                          RectangleY + BorderSize,
                          RectangleWidth - BorderSize * 2,
                          RectangleHeight - BorderSize * 2,
                          colour,
                          Bitmap);
}

void Bitmap_Draw_Circle(const int r, const int x, const int y, unsigned int colour, bitmap *Bitmap)
{
    // const int r2   = (const int)(r * r);
    // const int area = r2 << 2;
    // const int rr   = r << 1;

    // for (int i = 0; i < area; i++)
    //{
    //     int tx = (i % rr) - r;
    //     int ty = (i / rr) - r;

    //    if (tx * tx + ty * ty <= r2)
    //        Bitmap_Draw_Pixel(x + tx, y + ty, colour, Bitmap);
    //}

    const int radius_sqr = r * r;
    for (int xx = -r; xx < r; xx++)
    {
        int hh = (int)sqrt(radius_sqr - xx * xx);
        int rx = x + xx;
        int ph = y + hh;

        for (int yy = y - hh; yy < ph; yy++)
        {
            Bitmap_Draw_Pixel(rx, yy, colour, Bitmap);
        }
    }
}

#define NUM_OF_OUTLINE_SEGMENTS 360
#define DEG2RAD (180 * M_1_PI)
void Bitmap_Draw_Circle_Outline(const float r, const float x, const float y, unsigned int colour, bitmap *Bitmap)
{
    for (int i = 0; i < NUM_OF_OUTLINE_SEGMENTS; i++)
    {
        const double degInRad = i * DEG2RAD;
        Bitmap_Draw_Pixel((int)(x + cos(degInRad) * r),
                          (int)(y + sin(degInRad) * r),
                          colour,
                          Bitmap);
        // glVertex2f(a + cos(degInRad) * r, b + sin(degInRad) * r);
    }
}

inline static void _vec2_swap(vec2 *a, vec2 *b)
{
    vec2 tmp = *a;
    *a       = *b;
    *b       = tmp;
}

static void Draw_Flat_Top_Triangle(const vec2 v0, const vec2 v1, const vec2 v2, unsigned int colour, bitmap *Bitmap)
{
    /*   ______   < ystart
        \     /
      m0 \   / m1
          \ /
                 < yend
    */

    // calculate slopes in screen space
    // using dx/dy becuase of inverted screen space, so m = dx/dy
    const float m0 = (v2.x - v0.x) / (v2.y - v0.y);
    const float m1 = (v2.x - v1.x) / (v2.y - v1.y);

    // calculate start and end scanlines
    // ceil - rounds a number up to the next largest integer
    const int ystart = (int)ceil(v0.y - 0.5f);
    const int yend   = (int)ceil(v2.y - 0.5f); // scaneline after the last line drawn

    for (int y = ystart; y < yend; y++)
    {
        // calculate the start and end X coordinates
        // 0.5 - for calcualtating based on pixel centers
        // x0 = m0 * (change in y) + x
        // m0 * dy = dx <- this is the change in x, which we neeed, add on
        // our original x coord to get back to triangle position
        const float x0 = m0 * ((float)(y) + 0.5f - v0.y) + v0.x;
        const float x1 = m1 * ((float)(y) + 0.5f - v1.y) + v1.x;

        // calculate start and end pixels
        const int xstart = (int)ceil(x0 - 0.5f);
        const int xend   = (int)ceil(x1 - 0.5f);

        for (int x = xstart; x < xend; x++)
        {
            Bitmap_Draw_Pixel(x, y, colour, Bitmap);
        }
    }
}

void Draw_Flat_Bottom_Triangle(const vec2 v0, const vec2 v1, const vec2 v2, unsigned int colour, bitmap *Bitmap)
{
    const float m0 = (v1.x - v0.x) / (v1.y - v0.y);
    const float m1 = (v2.x - v0.x) / (v2.y - v0.y);

    // calculate start and end scanlines
    // ceil - rounds a number up to the next largest integer
    const int ystart = (int)ceil(v0.y - 0.5f);
    const int yend   = (int)ceil(v2.y - 0.5f); // scaneline after the last line drawn

    for (int y = ystart; y < yend; y++)
    {
        const float x0 = m0 * ((float)(y) + 0.5f - v0.y) + v0.x;
        const float x1 = m1 * ((float)(y) + 0.5f - v0.y) + v0.x;

        // calculate start and end pixels
        const int xstart = (int)ceil(x0 - 0.5f);
        const int xend   = (int)ceil(x1 - 0.5f);

        for (int x = xstart; x < xend; x++)
        {
            Bitmap_Draw_Pixel(x, y, colour, Bitmap);
        }
    }
}

void Bitmap_Draw_Triangle(vec2 v0, vec2 v1, vec2 v2, unsigned int colour, bitmap *Bitmap)
{
    // sort verticies by Y
    if (v1.y < v0.y)
        _vec2_swap(&v0, &v1);
    if (v2.y < v1.y)
        _vec2_swap(&v1, &v2);
    if (v1.y < v0.y)
        _vec2_swap(&v0, &v1);

    if (v0.y == v1.y) // Flat top triangle
    {
        // Sort top vertivies by x
        if (v1.x < v0.x)
            _vec2_swap(&v0, &v1);
        Draw_Flat_Top_Triangle(v0, v1, v2, colour, Bitmap);
    }
    else if (v1.y == v2.y) // Flat bottom triangle
    {
        // Sort bottom vertivies by x
        if (v2.x < v1.x)
            _vec2_swap(&v1, &v2);
        Draw_Flat_Bottom_Triangle(v0, v1, v2, colour, Bitmap);
    }
    else // General triangle
    {
        // https://youtu.be/9A5TVh6kPLA?t=1029
        // Find splitting vertex (along long side)
        const float alpha_split = (v1.y - v0.y) / (v2.y - v0.y);
        const vec2  v           = {
            v0.x + (v2.x - v0.x) * alpha_split,
            v0.y + (v2.y - v0.y) * alpha_split,
        };

        if (v1.x < v.x) // Major Right
        {
            Draw_Flat_Bottom_Triangle(v0, v1, v, colour, Bitmap);
            Draw_Flat_Top_Triangle(v1, v, v2, colour, Bitmap);
        }
        else // Major left
        {
            Draw_Flat_Bottom_Triangle(v0, v, v1, colour, Bitmap);
            Draw_Flat_Top_Triangle(v, v1, v2, colour, Bitmap);
        }
    }
}

#endif // __BITMAP_H__