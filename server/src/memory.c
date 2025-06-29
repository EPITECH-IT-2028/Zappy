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

/**
 * @brief Frees memory allocated for team names in params structure
 * @param params Pointer to parameters structure containing team names
 *
 * Iterates through the teams_names array and frees each team name string
 * before freeing the array itself.
 */
void free_params(params_t *params)
{
    if (params->teams_names) {
        for (int i = 0; i < params->teams_count; i += 1) {
            free(params->teams_names[i]);
        }
        free(params->teams_names);
    }
}

/**
 * @brief Frees memory allocated for the game map
 * @param map 2D array representing the map
 * @param params Parameters containing map dimensions
 *
 * Releases all memory for the 2D map structure by freeing each row
 * and then freeing the array of rows.
 */
void free_map(map_t **map, params_t *params)
{
    if (map == NULL || params == NULL)
        return;
    for (int i = 0; i < params->width; i++) {
        free(map[i]);
    }
    free(map);
}

/**
 * @brief Clears all pending responses for a client with proper mutex
 * protection
 * @param client Pointer to the client whose responses need to be cleared
 *
 * Acquires a mutex lock, frees each response string in the array,
 * frees the response array itself, and resets the size counter.
 * Properly handles synchronization to avoid race conditions.
 */
static
void clear_pending_response(client_t *client)
{
    pthread_mutex_lock(&client->data.pending_mutex);
    if (!client->data.pending_response.response) {
        pthread_mutex_unlock(&client->data.pending_mutex);
        return;
    }
    for (int i = 0; i < client->data.pending_response.size; i++) {
        if (client->data.pending_response.response[i]) {
            free(client->data.pending_response.response[i]);
        }
    }
    free(client->data.pending_response.response);
    client->data.pending_response.response = NULL;
    client->data.pending_response.size = 0;
    pthread_mutex_unlock(&client->data.pending_mutex);
}

/**
 * @brief Decrements the client count for a team when a client disconnects
 * @param server Server structure containing teams information
 * @param client Client that is being removed from a team
 *
 * Finds the team index for the client's team, decrements the client count
 * if appropriate (non-graphic client and valid team), and frees the team
 * name string.
 */
static
void decrement_team_count(server_t *server, client_t *client)
{
    int team_index = 0;

    if (client->data.team_name) {
        team_index = find_team_index(server, client->data.team_name);
        if (!client->data.is_graphic && team_index != ERROR &&
            server->teams[team_index].clients_count > 0) {
            server->teams[team_index].clients_count--;
        }
        free(client->data.team_name);
        client->data.team_name = NULL;
    }
}

/**
 * @brief Cleans up and frees resources associated with a client
 * @param server Server structure containing the client
 * @param index Index of the client to clean up
 *
 * Decrements team client count if applicable, frees team name,
 * cleans up pending responses with proper mutex protection,
 * and finally frees the client structure itself.
 */
void cleanup_client_data(server_t *server, int index)
{
    client_t *client;

    if (server->clients[index] == NULL)
        return;
    client = server->clients[index];
    if (!client->data.is_graphic && client->data.team_name)
        remove_player_map(
        &server->map[client->data.x][client->data.y], client);
    decrement_team_count(server, client);
    clear_pending_response(client);
    pthread_mutex_destroy(&client->data.pending_mutex);
    if (client->buffer) {
        free(client->buffer);
        client->buffer = NULL;
    }
    server->clients[index] = NULL;
    free(client);
}

/**
 * @brief Performs complete cleanup of the server structure
 * @param server Pointer to server structure to be freed
 *
 * Stops the server, joins the game thread, and systematically
 * frees all resources: clients, mutexes, map, parameters,
 * teams, and file descriptors.
 */
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

/**
 * @brief Frees an array of incantator clients
 * @param incantators NULL-terminated array of incantator client pointers
 *
 * Iterates through the array and frees each incantator pointer
 * before freeing the array itself.
 */
void free_incantators(client_t **incantators)
{
    if (!incantators)
        return;
    for (int i = 0; incantators[i] != NULL; i++)
        free(incantators[i]);
    free(incantators);
}
