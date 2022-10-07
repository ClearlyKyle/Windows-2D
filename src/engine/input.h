#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,
    BUTTON_MAX_BUTTONS
} buttons;

typedef enum
{
    KEY_R = 82,
    KEY_W = 87,
    KEY_A = 65,
    KEY_S = 0x53,
    KEY_D = 0x44,

    KEY_RSHIFT,
    KEY_LSHIFT,
    KEY_RALT,
    KEY_LALT,
    KEY_RCONTROL,
    KEY_LCONTROL

} keys;

void input_process_key(keys key, bool pressed);
void intput_process_mouse(buttons button, bool pressed);
void input_update(void);

// KEYBOARD input
bool input_key_is_down(const keys key);
bool input_key_is_up(const keys key);
bool input_key_is_pressed(const keys key);
bool input_key_was_down(const keys key);
bool input_key_was_up(const keys key);

// MOUSE input
bool input_mouse_is_botton_down(const buttons button);
bool input_mouse_is_botton_up(const buttons button);
void input_mouse_process_move(signed int x, signed int y);
#endif // __INPUT_H__