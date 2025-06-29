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

/**
 * @brief Initialize teams structure from parameters
 *
 * This function creates and initializes team structures using
 * the team names provided in the server parameters.
 *
 * @param teams Pointer to the teams array to initialize
 * @param params Pointer to server parameters containing team names
 * @return SUCCESS on successful initialization, ERROR on failure
 */
static
int init_teams_struct(teams_t *teams, params_t *params)
{
    for (int i = 0; i < params->teams_count; i++) {
        teams[i].id = i;
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
        qr->queue[i].response = NULL;
        qr->queue[i].size = 1;
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
    server->params = *params;
    server->map = malloc(sizeof(map_t *) * params->width);
    if (server->map == NULL)
        return ERROR;
    init_density(server, &server->density);
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

/**
 * @brief Initialize server structure with all components
 *
 * This function performs complete server initialization including
 * socket creation, memory allocation, and all subsystem setup.
 *
 * @param server Pointer to the server structure to initialize
 * @param params Pointer to validated server parameters
 * @return SUCCESS on successful initialization, ERROR on failure
 */
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

/**
 * @brief Initialize parameters structure to default values
 *
 * This function sets all parameter fields to their default values,
 * typically -1 to indicate unset parameters.
 *
 * @param params Pointer to the parameters structure to initialize
 */
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
