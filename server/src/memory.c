/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** free_server.c
*/

#include "server.h"
#include "utils.h"
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

void cleanup_client_data(server_t *server, int index)
{
    client_t *client;
    int team_index;

    if (server->clients[index] == NULL)
        return;
    client = server->clients[index];
    if (client->data.team_name) {
        team_index = find_team_index(server, client->data.team_name);
        if (!client->data.is_graphic && team_index != ERROR &&
            server->teams[team_index].clients_count > 0) {
            server->teams[team_index].clients_count--;
        }
        free(client->data.team_name);
        client->data.team_name = NULL;
    }
    pthread_mutex_destroy(&client->data.pending_mutex);
    free(client);
    server->clients[index] = NULL;
}

void free_server(server_t *server)
{
    if (server == NULL)
        return;
    server->running = false;
    pthread_mutex_lock(&server->clients_mutex);
    pthread_join(server->threads.game_thread, NULL);
    for (int i = 0; i < server->nfds; i += 1) {
        if (server->clients[i] != NULL) {
            free(server->clients[i]->data.team_name);
            free(server->clients[i]);
        }
    }
    pthread_mutex_unlock(&server->clients_mutex);
    pthread_mutex_destroy(&server->clients_mutex);
    free(server->clients);
    free_map(server->map, &server->params);
    free_params(&server->params);
    free(server->teams);
    free(server->fds);
    free(server);
}

void free_incantators(client_t **incantators)
{
    if (!incantators)
        return;
    for (int i = 0; incantators[i] != NULL; i++)
        free(incantators[i]);
    free(incantators);
}
