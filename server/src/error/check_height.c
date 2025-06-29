/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_height.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"
#include "macro.h"

/**
 * @brief Validate and set the map height parameter
 *
 * This function checks if the provided height value is valid
 * (greater than minimum) and sets it in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if valid height, ERROR otherwise
 */
int check_height(params_t *params, char **av, size_t *av_idx)
{
    int height = atoi(av[*av_idx]);

    if (height == 0) {
        return ERROR;
    }
    if (height < MIN_Y) {
        return ERROR;
    }
    *av_idx += 1;
    params->height = height;
    return SUCCESS;
}
