/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_width.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"
#include "macro.h"

/**
 * @brief Validate and set the map width parameter
 *
 * This function checks if the provided width value is valid
 * (greater than minimum) and sets it in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if valid width, ERROR otherwise
 */
int check_width(params_t *params, char **av, size_t *av_idx)
{
    int width = atoi(av[*av_idx]);

    if (width == 0) {
        return ERROR;
    }
    if (width < MIN_X) {
        return ERROR;
    }
    *av_idx += 1;
    params->width = width;
    return SUCCESS;
}
