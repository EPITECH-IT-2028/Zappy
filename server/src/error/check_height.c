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
