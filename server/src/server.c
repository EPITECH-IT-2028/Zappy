/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.c
*/

#include "server.h"
#include <stddef.h>

static void init_server(server_t *server)
{
    server->client_per_team = -1;
    server->teams_count = 0;
    server->teams_names = NULL;
    server->frequence = -1;
    server->height = -1;
    server->width = -1;
    server->port = -1;
}

int server(int ac, char **av)
{
    server_t server;

    init_server(&server);
    if (check_params(&server, ac, av)) {
        free_server(&server);
        return ERROR;
    }
    free_server(&server);
    return SUCCESS;
}
