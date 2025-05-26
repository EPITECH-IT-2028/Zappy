/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_width.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"

int check_width(server_t *server, char **av, size_t *av_idx)
{
    int width = atoi(av[*av_idx]);

    if (width == 0) {
        return ERROR;
    }
    if (width < MIN_X) {
        return ERROR;
    }
    *av_idx += 1;
    server->width = width;
    return SUCCESS;
}
