/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_height.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"

int check_height(server_t *server, char **av, size_t *av_idx)
{
    int height = atoi(av[*av_idx]);

    if (height == 0) {
        return ERROR;
    }
    if (height < MIN_Y) {
        return ERROR;
    }
    *av_idx += 1;
    server->height = height;
    return SUCCESS;
}
