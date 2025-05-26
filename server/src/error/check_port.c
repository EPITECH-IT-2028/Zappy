/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_port.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>
#include "macro.h"

int check_port(params_t *params, char **av, size_t *av_idx)
{
    int port = atoi(av[*av_idx]);

    if (port <= 0 || port > 65535) {
        return ERROR;
    }
    *av_idx += 1;
    params->port = port;
    return SUCCESS;
}
