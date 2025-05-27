/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** init_struct.c
*/

#include "macro.h"
#include "server.h"

void init_client_struct(client_t *clients)
{
    clients->fd = -1;
    clients->addr_len = sizeof(clients->addr);
    clients->fd_open = -1;
    clients->connected = false;
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
    if (server->fds == NULL || server->clients == NULL) {
        return ERROR;
    }
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
