/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** init_struct.c
*/

#include "macro.h"
#include "server.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

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
    clients->data.pending_requests = 0;
    pthread_mutex_init(&clients->data.pending_mutex, NULL);
}

static
void init_queue_request_struct(queue_request_t *qr)
{
    qr->head = 0;
    qr->tail = 0;
    qr->len = 0;
    pthread_mutex_init(&qr->mutex, NULL);
    for (size_t i = 0; i < QUEUE_MAX_SIZE; i++) {
        qr->queue[i].client = NULL;
        memset(qr->queue[i].request, 0, BUFFER_SIZE);
    }
}

static
void init_queue_response_struct(queue_response_t *qr)
{
    qr->head = 0;
    qr->tail = 0;
    qr->len = 0;
    pthread_mutex_init(&qr->mutex, NULL);
    for (size_t i = 0; i < QUEUE_MAX_SIZE; i++) {
        qr->queue[i].client = NULL;
        memset(qr->queue[i].response, 0, BUFFER_SIZE);
    }
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

static
int init_queues(server_t *server)
{
    init_queue_request_struct(&server->queue_request);
    init_queue_response_struct(&server->queue_response);
    return SUCCESS;
}

static
int init_map_struct(server_t *server, params_t *params)
{
    server->map = malloc(sizeof(map_t *) * params->width);
    if (server->map == NULL)
        return ERROR;
    for (int x = 0; x < params->width; x++) {
        server->map[x] = malloc(sizeof(map_t) * params->height);
        if (server->map[x] == NULL)
            return ERROR;
        for (int y = 0; y < params->height; y++)
            server->map[x][y] = (map_t){0};
    }
    if (place_resources(server) == ERROR)
        return ERROR;
    return SUCCESS;
}

int init_server_struct(server_t *server, params_t *params)
{
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(params->port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr_len = sizeof(server->addr);
    server->fds = malloc(sizeof(struct pollfd));
    server->clients = malloc(sizeof(client_t));
    server->teams = malloc(sizeof(teams_t) * params->teams_count);
    if (server->fds == NULL || server->clients == NULL ||
        server->teams == NULL || init_queues(server) == ERROR ||
        init_teams_struct(server->teams, params) == ERROR ||
        init_map_struct(server, params) == ERROR || server->fd < 0)
        return ERROR;
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
