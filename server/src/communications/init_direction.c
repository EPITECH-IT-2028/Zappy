/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_direction.c
*/

#include "server.h"
#include <stdlib.h>

void init_direction(direction_t *direction)
{
    static const direction_t directions[MAX_DIRECTION] =
        {UP, RIGHT, DOWN, LEFT};
    int pos = 0;

    pos = rand() % MAX_DIRECTION;
    *direction = directions[pos];
}
