#include "input.h"

typedef struct keyboard_state
{
    bool keys[256];
} keyboard_state;

typedef struct mouse_state
{
    unsigned int x;
    unsigned int y;
    unsigned int buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state
{
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state    mouse_current;
    mouse_state    mouse_previous;
} input_state;

// Internal input state
static bool        input_initialised = false;
static input_state state             = {0};

void input_init(void)
{
    input_initialised = true;
}

void input_update(double delta_time)
{
    if (input_initialised)
        return;

    memcpy(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    memcpy(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

// LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param)
//{
//     switch (msg)
//     {
//     case WM_ERASEBKGND:
//         // Notify the OS that erasing will be handled by the application to prevent flicker.
//         return 1;
//     case WM_CLOSE:
//         // TODO: Fire an event for the application to quit.
//         event_context data = {};
//         event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
//         return 0;
//     case WM_DESTROY:
//         PostQuitMessage(0);
//         return 0;
//     case WM_SIZE:
//     {
//         // Get the updated size.
//         RECT r;
//         GetClientRect(hwnd, &r);
//         u32 width  = r.right - r.left;
//         u32 height = r.bottom - r.top;

//        // Fire the event. The application layer should pick this up, but not handle it
//        // as it shouldn be visible to other parts of the application.
//        event_context context;
//        context.data.u16[0] = (u16)width;
//        context.data.u16[1] = (u16)height;
//        event_fire(EVENT_CODE_RESIZED, 0, context);
//    }
//    break;
//    case WM_KEYDOWN:
//    case WM_SYSKEYDOWN:
//    case WM_KEYUP:
//    case WM_SYSKEYUP:
//    {
//        // Key pressed/released
//        b8   pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
//        keys key     = (u16)w_param;

//        // Check for extended scan code.
//        b8 is_extended = (HIWORD(l_param) & KF_EXTENDED) == KF_EXTENDED;

//        // Keypress only determines if _any_ alt/ctrl/shift key is pressed. Determine which one if so.
//        if (w_param == VK_MENU)
//        {
//            key = is_extended ? KEY_RALT : KEY_LALT;
//        }
//        else if (w_param == VK_SHIFT)
//        {
//            // Annoyingly, KF_EXTENDED is not set for shift keys.
//            u32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
//            u32 scancode   = ((l_param & (0xFF << 16)) >> 16);
//            key            = scancode == left_shift ? KEY_LSHIFT : KEY_RSHIFT;
//        }
//        else if (w_param == VK_CONTROL)
//        {
//            key = is_extended ? KEY_RCONTROL : KEY_LCONTROL;
//        }

//        // Pass to the input subsystem for processing.
//        input_process_key(key, pressed);

//        // Return 0 to prevent default window behaviour for some keypresses, such as alt.
//        return 0;
//    }
//    case WM_MOUSEMOVE:
//    {
//        // Mouse move
//        i32 x_position = GET_X_LPARAM(l_param);
//        i32 y_position = GET_Y_LPARAM(l_param);

//        // Pass over to the input subsystem.
//        input_process_mouse_move(x_position, y_position);
//    }
//    break;
//    case WM_MOUSEWHEEL:
//    {
//        i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
//        if (z_delta != 0)
//        {
//            // Flatten the input to an OS-independent (-1, 1)
//            z_delta = (z_delta < 0) ? -1 : 1;
//            input_process_mouse_wheel(z_delta);
//        }
//    }
//    break;
//    case WM_LBUTTONDOWN:
//    case WM_MBUTTONDOWN:
//    case WM_RBUTTONDOWN:
//    case WM_LBUTTONUP:
//    case WM_MBUTTONUP:
//    case WM_RBUTTONUP:
//    {
//        b8      pressed      = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
//        buttons mouse_button = BUTTON_MAX_BUTTONS;
//        switch (msg)
//        {
//        case WM_LBUTTONDOWN:
//        case WM_LBUTTONUP:
//            mouse_button = BUTTON_LEFT;
//            break;
//        case WM_MBUTTONDOWN:
//        case WM_MBUTTONUP:
//            mouse_button = BUTTON_MIDDLE;
//            break;
//        case WM_RBUTTONDOWN:
//        case WM_RBUTTONUP:
//            mouse_button = BUTTON_RIGHT;
//            break;
//        }

//        // Pass over to the input subsystem.
//        if (mouse_button != BUTTON_MAX_BUTTONS)
//        {
//            input_process_button(mouse_button, pressed);
//        }
//    }
//    break;
//    }

//    return DefWindowProcA(hwnd, msg, w_param, l_param);
//}