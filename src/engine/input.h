#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MAX_BUTTONS
} buttons;

typedef enum
{
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
} keys;

void input_process_key(keys key, bool pressed);

bool input_is_key_down(const keys key);
bool input_is_key_up(const keys key);
bool input_was_key_down(const keys key);
bool input_was_key_up(const keys key);

#endif // __INPUT_H__