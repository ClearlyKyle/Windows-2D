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
static input_state state = {0};

void input_update(void)
{
    memcpy(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    memcpy(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, bool pressed)
{
    // Only handle this if the state actually changed.
    if (state.keyboard_current.keys[key] != pressed)
    {
        // Update internal state
        state.keyboard_current.keys[key] = pressed;
    }
}

// KEYBOARD input
bool input_key_is_down(const keys key)
{
    return state.keyboard_current.keys[key] == true;
}

bool input_key_is_up(const keys key)
{
    return state.keyboard_current.keys[key] == false;
}

// Pressed with no repeating
bool input_key_is_pressed(const keys key)
{
    return (state.keyboard_previous.keys[key] == false && state.keyboard_current.keys[key] != false);
}

bool input_key_was_down(const keys key)
{
    return state.keyboard_previous.keys[key] == true;
}

bool input_key_was_up(const keys key)
{
    return state.keyboard_previous.keys[key] == false;
}

// MOUSE input
void intput_process_mouse(buttons button, bool pressed)
{
    state.mouse_current.buttons[button] = pressed;
}

bool input_mouse_is_botton_down(const buttons button)
{
    return state.mouse_current.buttons[button] == true;
}

bool input_mouse_is_botton_up(const buttons button)
{
    return state.mouse_current.buttons[button] == false;
}

void input_mouse_process_move(signed int x, signed int y)
{
    // Only process if actually different
    if (state.mouse_current.x != x || state.mouse_current.y != y)
    {
        // Update internal state.
        state.mouse_current.x = x;
        state.mouse_current.y = y;
    }
}

// void input_process_mouse_wheel(i8 z_delta)
//{
// }