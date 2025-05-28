/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** init_struct.c
*/

#include "macro.h"
#include "server.h"
#include <string.h>

void init_client_struct(client_t *clients, int fd)
{
    clients->fd = fd;
    clients->addr_len = sizeof(clients->addr);
    clients->fd_open = 0;
    clients->connected = false;
    clients->data.team_name = NULL;
    clients->data.x = 0;
    clients->data.y = 0;
    clients->data.level = 1;
    clients->data.is_graphic = false;
}

static
int init_teams_struct(teams_t *teams, params_t *params)
{
    for (int i = 0; i < params->teams_count; i++) {
        teams[i].name = strdup(params->teams_names[i]);
        if (teams[i].name == NULL)
            return ERROR;
        teams[i].clients_count = 0;
    }
    return SUCCESS;
}

int init_server_struct(server_t *server, params_t *params)
{
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd == -1)
        return ERROR;
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(params->port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr_len = sizeof(server->addr);
    server->fds = malloc(sizeof(struct pollfd));
    server->clients = malloc(sizeof(client_t));
    server->teams = malloc(sizeof(teams_t) * params->teams_count);
    if (server->fds == NULL || server->clients == NULL ||
        server->teams == NULL)
        return ERROR;
    init_teams_struct(server->teams, params);
    server->clients[SERVER_INDEX] = NULL;
    server->nfds = 1;
    server->fds[SERVER_INDEX].fd = server->fd;
    server->fds[SERVER_INDEX].events = POLLIN;
    server->running = true;
    return SUCCESS;
}

void init_params(params_t *params)
{
    params->client_per_team = -1;
    params->teams_count = 0;
    params->teams_names = NULL;
    params->frequence = -1;
    params->height = -1;
    params->width = -1;
    params->port = -1;
}
