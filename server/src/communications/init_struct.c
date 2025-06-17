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
#include <time.h>

static
void init_client_inventory(client_data_t *cd)
{
    cd->inventory.food = 10;
    cd->inventory.linemate = 1;
    cd->inventory.deraumere = 0;
    cd->inventory.sibur = 0;
    cd->inventory.mendiane = 0;
    cd->inventory.phiras = 0;
    cd->inventory.thystame = 0;
}

static
void init_incantation_state(incantation_t *inc)
{
    inc->is_incantating = false;
    inc->x = 0;
    inc->y = 0;
    inc->id_incantator = 0;
}

void init_client_struct(client_t *clients, int fd)
{
    clients->fd = fd;
    clients->addr_len = sizeof(clients->addr);
    clients->fd_open = 0;
    clients->connected = true;
    clients->data.team_name = NULL;
    clients->data.x = 0;
    clients->data.y = 0;
    clients->data.id = -1;
    clients->data.orientation = 0;
    clients->data.level = 1;
    clients->data.is_graphic = false;
    clients->data.pending_requests = 0;
    clients->data.pending_response.client = NULL;
    memset(clients->data.pending_response.response, 0, BUFFER_SIZE);
    init_incantation_state(&clients->data.incantation);
    clients->data.is_busy = false;
    init_direction(&clients->data.direction);
    pthread_mutex_init(&clients->data.pending_mutex, NULL);
    init_client_inventory(&clients->data);
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

static
int init_clients_and_fds(server_t *server)
{
    server->clients[SERVER_INDEX] = NULL;
    server->nfds = 1;
    server->fds[SERVER_INDEX].fd = server->fd;
    server->fds[SERVER_INDEX].events = POLLIN;
    server->fds[SERVER_INDEX].revents = 0;
    server->running = true;
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
    server->ids = 0;
    server->egg_ids = 0;
    server->clients = malloc(sizeof(client_t *));
    server->teams = malloc(sizeof(teams_t) * params->teams_count);
    if (pthread_mutex_init(&server->clients_mutex, NULL) != 0)
        return ERROR;
    if (server->fds == NULL || server->clients == NULL ||
        server->teams == NULL || init_queues(server) == ERROR ||
        init_teams_struct(server->teams, params) == ERROR ||
        clock_gettime(CLOCK_MONOTONIC, &server->server_timer) ||
        init_map_struct(server, params) == ERROR || server->fd < 0
        || init_clients_and_fds(server))
        return ERROR;
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
