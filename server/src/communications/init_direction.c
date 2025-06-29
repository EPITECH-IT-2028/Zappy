/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_direction.c
*/

#include "server.h"
#include <stdlib.h>

/**
 * @brief Initialize a random direction for a client
 *
 * This function randomly selects a direction from the predefined
 *
 * @param direction Pointer to the direction_t variable to initialize
 */
void init_direction(direction_t *direction)
{
    static const direction_t directions[MAX_DIRECTION] =
        {UP, RIGHT, DOWN, LEFT};
    int pos = 0;

    pos = rand() % MAX_DIRECTION;
    *direction = directions[pos];
}
