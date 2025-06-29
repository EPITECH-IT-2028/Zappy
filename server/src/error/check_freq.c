/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_freq.c
*/

#include <stdlib.h>
#include "server.h"
#include "macro.h"

/**
 * @brief Validate and set the server frequency parameter
 *
 * This function checks if the provided frequency value is valid
 * (greater than 0) and sets it in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if valid frequency, ERROR otherwise
 */
int check_freq(params_t *params, char **av, size_t *av_idx)
{
    int freq = atoi(av[*av_idx]);

    if (freq <= 0) {
        return ERROR;
    }
    *av_idx += 1;
    params->frequence = freq;
    return SUCCESS;
}
