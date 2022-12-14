#ifndef __APP_H__
#define __APP_H__

#include "./maths/vec2.h"

typedef void (*Appfunction_ptr)(void);
typedef void (*Appfunction_update_ptr)(const double elapsed_time);

void App_Startup(const int width, const int height, const char *title,
                 Appfunction_ptr        init,      // run on application startup
                 Appfunction_update_ptr update,    // run on every loop
                 Appfunction_ptr        on_render, // run on every loop
                 Appfunction_ptr        on_exit);         // run on exit of application

void Window_Draw_Pixel(const int X, const int Y, const unsigned int colour);
void Window_Draw_Rectangle(const int X, const int Y, const int W, const int H, const unsigned int colour);
void Window_Draw_Circle(const int radius, const int X, const int Y, const unsigned int colour);
void Window_Draw_Circle_Outline(const float radius, const float X, const float Y, const unsigned int colour);
void Window_Draw_Triangle(const vec2 v0, const vec2 v1, const vec2 v2, unsigned int colour);

void         Window_Set_Title(const char *title);
unsigned int Window_Height(void);
unsigned int Window_Width(void);

#endif // __APP_H__