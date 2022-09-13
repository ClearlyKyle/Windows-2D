#include "app.h"

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#include "timer.h"
#include "bitmap.h"

typedef struct Application
{
    HINSTANCE h_instance;
    HWND      hwnd;
    HDC       device_context;

    unsigned int window_width;
    unsigned int window_height;
    unsigned int client_width;
    unsigned int client_height;

    bool running;

    Appfunction_ptr Init;
    Appfunction_ptr Update;
    Appfunction_ptr OnExit;

    double frame_time, last_frame_time; // ms
} Application;

// global app
static Application window_app;

// static void _Init()
//{
//     app.Init();
// }

static void _Update()
{
    window_app.Update();
}

static void _OnExit()
{
    window_app.OnExit();

    if (window_app.hwnd)
    {
        DestroyWindow(window_app.hwnd);
        window_app.hwnd = 0;
    }
}

LRESULT CALLBACK win32_process_message(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

bool Initialise_Window(const char *title, const unsigned x, const unsigned y, const unsigned w, const unsigned h)
{
    const bool center_window = true;

    window_app.h_instance = GetModuleHandleA(0);

    // WindowClass
    const char CLASS_NAME[]   = "GameWindowClass";
    WNDCLASS   WindowClass    = {0};
    WindowClass.lpfnWndProc   = win32_process_message;
    WindowClass.hInstance     = window_app.h_instance;
    WindowClass.lpszClassName = CLASS_NAME;
    WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = NULL; // Transparent

    if (!RegisterClass(&WindowClass))
    {
        MessageBox(0, "RegisterClass(&WindowClass)", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // Window dimensions
    unsigned int window_x      = x;
    unsigned int window_y      = y;
    unsigned int window_width  = w;
    unsigned int window_height = h;

    // unsigned int window_style    = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    long         window_style    = WS_OVERLAPPEDWINDOW;
    unsigned int window_ex_style = WS_EX_APPWINDOW;

    // window_style |= WS_MAXIMIZEBOX;
    // window_style |= WS_MINIMIZEBOX;
    // window_style |= WS_THICKFRAME;

    // Adjust window dimensions to given client area
    // Window dimensions will be client area dimensions + some units for the borders and title bar
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    // Window position
    if (center_window)
    {
        const int ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
        const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        window_x = (ScreenWidth / 2) - (window_width / 2);
        window_y = (ScreenHeight / 2) - (window_height / 2);
    }

    HWND handle = CreateWindowEx(0,          // Optional window styles
                                 CLASS_NAME, // Window class
                                 title,
                                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 window_x, // Size and position
                                 window_y,
                                 window_width,
                                 window_height,
                                 NULL,                  // Parent window
                                 NULL,                  // Menu
                                 window_app.h_instance, // Instance handle
                                 NULL                   // Additional application data
    );

    if (!handle)
    {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    window_app.hwnd          = handle;
    window_app.window_width  = window_width;
    window_app.window_height = window_height;
    window_app.client_width  = w;
    window_app.client_height = h;

    // Show the window
    bool should_activate           = 1;
    int  show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(handle, show_window_command_flags);

    // For StretchDIBits
    window_app.device_context = GetDC(handle);

    return true;
}

typedef uint32_t u32;
static void      Window_Clear_Screen(const unsigned int colour, bitmap *const Bitmap)
{
    const u32 num_of_pixels = Bitmap->Width * Bitmap->Height;
    for (u32 *pixel     = (u32 *)Bitmap->Memory,
             *end_pixel = ((u32 *)Bitmap->Memory + num_of_pixels);
         pixel != end_pixel;
         pixel++)
    {
        *pixel = colour;
    }
    // memset(Bitmap->Memory, colour, num_of_pixels * 4);
}

static void Window_Blit(bitmap *Bitmap)
{
    StretchDIBits(window_app.device_context,
                  0, 0,
                  Bitmap->Width, Bitmap->Height,
                  0, 0,
                  window_app.client_width, window_app.client_height,
                  Bitmap->Memory, &Bitmap->Info,
                  DIB_RGB_COLORS, SRCCOPY);
}

void App_Startup(const int width, const int height, const char *title,
                 Appfunction_ptr init,    // run on application startup
                 Appfunction_ptr update,  // run on every loop
                 Appfunction_ptr on_exit) // run on exit of application
{
    srand((unsigned int)time(NULL));

    const bool window_staus = Initialise_Window("Main Window Title", 0, 100, width, height);
    if (window_staus == false)
    {
        fprintf(stderr, "Error Initialise_Window\n");
        return;
    }
    window_app.Init   = init;
    window_app.Update = update;
    window_app.OnExit = on_exit;

    bitmap     Bitmap        = {0};
    const bool bitmap_status = Bitmap_init(&Bitmap, window_app.hwnd);
    if (!bitmap_status)
    {
        fprintf(stderr, "Error Init_Bitmap\n");
        return;
    }

    timer      Timer        = {0};
    const bool timer_status = Timer_Init(&Timer);
    if (!timer_status)
    {
        fprintf(stderr, "Error Timer_Init\n");
        return;
    }

    bool Running = true;

    while (Running)
    {
        MSG Message;
        while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
                Running = false;
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }

        Timer_Update(&Timer);

        if (Timer.ElapsedMilliSeconds > 500)
        {
            _Update();
            Timer_Start(&Timer);
        }

        Window_Clear_Screen(0x333333, &Bitmap);

        // 0xRRGGBB
        Bitmap_Draw_Pixel(0, 0, 0xFF0000, &Bitmap);
        Bitmap_Draw_Pixel(799, 799, 0x0000BB, &Bitmap);
        Bitmap_Draw_Rectangle(50, 50, 100, 100, 0x00FF00, &Bitmap);

        Window_Blit(&Bitmap);
    }
}

LRESULT CALLBACK win32_process_message(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(Window, Message, WParam, LParam);
}
