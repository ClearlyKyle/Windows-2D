#pragma once

#include <stdint.h>

typedef struct ArrayList
{
    size_t len;
    size_t capacity;
    size_t item_size;
    void  *items;
} ArrayList;

ArrayList *array_list_create(size_t item_size, size_t initial_capacity);
size_t     array_list_append(ArrayList *list, void *item);
void      *array_list_get(ArrayList *list, size_t index);
uint8_t    array_list_remove(ArrayList *list, size_t index);