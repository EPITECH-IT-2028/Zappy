/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_port.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>

int check_port(server_t *server, char **av, size_t *av_idx)
{
    int port = atoi(av[*av_idx]);

    if (port <= 0 || port > 65535) {
        return ERROR;
    }
    *av_idx += 1;
    server->port = port;
    return SUCCESS;
}
