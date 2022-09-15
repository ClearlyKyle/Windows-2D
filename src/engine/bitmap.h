#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <windows.h>
#include <stdbool.h>

typedef struct
{
    void      *Memory;
    BITMAPINFO Info;
    int        Width;
    int        Height;
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
    unsigned int *pixel = (unsigned int *)Bitmap->Memory;
    pixel += Y * Bitmap->Width + X;
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

#endif // __BITMAP_H__