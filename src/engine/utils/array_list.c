#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_list.h"

ArrayList *array_list_create(size_t item_size, size_t initial_capacity)
{
    ArrayList *list = malloc(sizeof(ArrayList));

    if (!list)
    {
        fprintf(stderr, "Could not allocate memory : ArrayList *list = malloc(sizeof(ArrayList))\n");
        list = NULL;
        return NULL;
    }

    list->item_size = item_size;
    list->capacity  = initial_capacity;
    list->len       = 0;
    list->items     = malloc(item_size * initial_capacity);

    if (!list->items)
    {
        fprintf(stderr, "Could not allocate memory for list->items : %d bytes\n", item_size * initial_capacity);
        list->items = NULL;
        return NULL;
    }

    return list;
}

size_t array_list_append(ArrayList *list, void *item)
{
    if (list->len == list->capacity)
    {
        list->capacity = list->capacity > 0 ? list->capacity * 2 : 1;
        void *items    = realloc(list->items, list->item_size * list->capacity);

        if (!items)
        {
            fprintf(stderr, "Could not allocate memory for expanding capacity\n");
            fprintf(stderr, "list.len : %d\nlist.capacity : %d\n", list->len, list->capacity);
            return -1;
        }
        list->items = items;
    }

    const size_t index = list->len++;

    (void)memcpy((uint8_t *)list->items + index * list->item_size, item, list->item_size);

    return index;
}

void *array_list_get(ArrayList *list, size_t index)
{
    if (index >= list->len)
    {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }

    return (uint8_t *)list->items + index * list->item_size;
}

uint8_t array_list_remove(ArrayList *list, size_t index)
{
    if (list->len == 0)
    {
        fprintf(stderr, "List is empty\n");
        return 1;
    }
    if (index >= list->len)
    {
        fprintf(stderr, "Index out of bounds\n");
        return 1;
    }
    if (list->len == 1)
    {
        list->len = 0;
        return 0;
    }

    --list->len;

    uint8_t       *item_ptr = (uint8_t *)list->items + index * list->item_size;
    const uint8_t *end_ptr  = (uint8_t *)list->items + list->len * list->item_size;
    (void)memcpy(item_ptr, end_ptr, list->item_size);

    return 0;
}