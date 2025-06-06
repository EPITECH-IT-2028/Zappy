/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** free_server.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

void free_params(params_t *params)
{
    if (params->teams_names) {
        for (int i = 0; i < params->teams_count; i += 1) {
            free(params->teams_names[i]);
        }
        free(params->teams_names);
    }
}

void free_map(map_t **map, params_t *params)
{
    if (map == NULL || params == NULL)
        return;
    for (int i = 0; i < params->width; i++) {
        free(map[i]);
    }
    free(map);
}

void free_server(server_t *server)
{
    if (server == NULL)
        return;
    pthread_join(server->threads.game_thread, NULL);
    for (int i = 0; i < server->nfds; i += 1) {
        if (server->clients[i] != NULL) {
            free(server->clients[i]->data.team_name);
            free(server->clients[i]);
        }
    }
    free(server->clients);
    free_map(server->map, &server->params);
    free_params(&server->params);
    free(server->teams);
    free(server->fds);
    free(server);
}
