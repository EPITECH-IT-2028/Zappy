/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation.c
*/

#include "incantation_condition.h"
#include "macro.h" 
#include "utils.h"
#include <stdio.h>
#include "server.h"
#include <stdlib.h>
#include <string.h>

int check_if_incantation_failed(
    client_data_t *incantator,
    client_t **clients,
    map_t *unit_space
)
{
    if (!incantator || !clients)
        return ERROR;
    if (incantator->x != incantator->incantation.x
        || incantator->y != incantator->incantation.y
        || unit_space->nbr_of_players < tab_incantation[incantator->level - 1].player
        || unit_space->linemate < tab_incantation[incantator->level - 1].linemate
        || unit_space->deraumere < tab_incantation[incantator->level - 1].deraumere
        || unit_space->sibur < tab_incantation[incantator->level - 1].sibur
        || unit_space->mendiane < tab_incantation[incantator->level - 1].mendiane
        || unit_space->phiras < tab_incantation[incantator->level - 1].phiras
        || unit_space->thystame < tab_incantation[incantator->level - 1].thystame
    )
        return ERROR;
    return SUCCESS;
}

/**
 * Check if all conditions are met for an incantation ritual
 * Validates both resource requirements and player level requirements
 *
 * @param server Pointer to the server structure
 * @param request Pointer to the client request
 * @return SUCCESS if all conditions are met, ERROR otherwise
 */
static
int check_incantation_condition(server_t *server, request_t *request)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    uint8_t count_player_lvl_needed = 0;
    uint8_t lvl_needed = client->level;

    if (unit_space->nbr_of_players < tab_incantation[client->level - 1].player
        || unit_space->linemate < tab_incantation[client->level - 1].linemate
        || unit_space->deraumere < tab_incantation[client->level - 1].deraumere
        || unit_space->sibur < tab_incantation[client->level - 1].sibur
        || unit_space->mendiane < tab_incantation[client->level - 1].mendiane
        || unit_space->phiras < tab_incantation[client->level - 1].phiras
        || unit_space->thystame < tab_incantation[client->level - 1].thystame
    )
        return ERROR;
    for (int i = 0; i < unit_space->nbr_of_players; i++) {
        if (unit_space->players[i]->data.level == lvl_needed)
            count_player_lvl_needed++;
    }
    if (count_player_lvl_needed < tab_incantation[client->level - 1].player) {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * Freeze all players on the current tile during incantation ritual
 * Sets all players as busy and assigns them the same action end time
 * to prevent them from performing other actions during the ritual
 * 
 * @param server Pointer to the server structure
 * @param request Pointer to the client request containing position data
 * @return SUCCESS always (function cannot fail)
 */
static
int freeze_every_player(server_t *server, request_t *request, client_t **clients_frozen)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    uint8_t nbr_clients_frozen = 1;
    uint8_t lvl_needed = client->level;
    int i = 1;

    clients_frozen[INDEX_INCANTATOR] = malloc(sizeof(client_t));
    if (!clients_frozen[INDEX_INCANTATOR])
        return ERROR;
    memcpy(clients_frozen[INDEX_INCANTATOR], client, sizeof(client_t));
    for (int j = 0; j < unit_space->nbr_of_players; j++) {
        if (unit_space->players[j]->data.level == lvl_needed && 
            unit_space->players[j] != request->client) {
            clients_frozen[i] = malloc(sizeof(client_t));
            if (!clients_frozen[i])
                return ERROR;
            memcpy(clients_frozen[i], unit_space->players[j], sizeof(client_t));
            i++;
            nbr_clients_frozen++;
            if (nbr_clients_frozen >= tab_incantation[client->level - 1].player)
                break;
        }
    }
    for (int j = 0; j < i; j++) {
        clients_frozen[j]->data.incantation.is_incantating = true;
        clients_frozen[j]->data.incantation.x = client->x;
        clients_frozen[j]->data.incantation.y = client->y;
        clients_frozen[j]->data.incantation.id_incantator = clients_frozen[INDEX_INCANTATOR]->data.id;
        clients_frozen[j]->data.is_busy = true;
        clients_frozen[j]->data.action_end_time = get_action_end_time(server,
            INCANTATION_TIME);
    }
    return SUCCESS;
}

static
void add_new_incantation_request(server_t *server, request_t *request, client_t **incantators)
{
    request_t new_request;

    new_request.client = request->client;
    memcpy(new_request.request, request->request, BUFFER_SIZE);
    queue_add_request(server, &new_request);
    for (int i = 0; incantators[i] != NULL; i++) {
        send_code(incantators[i]->fd, "Elevation underway");
    }
}

static
void level_up_all_client(server_t *server, client_t *incantator, response_t *response)
{
    incantator->data.level++; 
    for (int i = 0; i < server->nfds; i++) {
        if (server->clients[i]->data.incantation.id_incantator == incantator->data.id) {
            server->clients[i]->data.level++;
        }
    }
    sprintf(response->response, "Current level: %d", incantator->data.level); 
}

/**
 * Handle incantation ritual request from a client
 * Validates all preconditions and initiates the ritual by
 * freezing all players
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure
 * (unused but required by interface)
 * @param request Pointer to the client request containing player data
 * @return SUCCESS if incantation can proceed, ERROR if conditions not met
 */
int handle_incantation(server_t *server, response_t *response,
    request_t *request)
{
    client_t **incantators = NULL;;

    if (!server || !response || !request)
        return ERROR;
    if (request->client->data.incantation.is_incantating == true) {
        if (check_if_incantation_failed(&request->client->data,
            server->clients, &server->map[response->client->data.x]
            [response->client->data.y]) == ERROR)
            return ERROR;
        level_up_all_client(server, response->client, response);
        return SUCCESS;
    }
    incantators = malloc(sizeof(client_t *) * (tab_incantation[response->client->data.level - 1].player + 1));
    if (!incantators)
        return ERROR;
    for (int i = 0; i <= tab_incantation[response->client->data.level - 1].player; i++)
        incantators[i] = NULL;
    if (check_incantation_condition(server, request) == ERROR)
        return ERROR;
    if (freeze_every_player(server, request, incantators) == ERROR)
        return ERROR;
    send_pic_all(server, incantators);
    add_new_incantation_request(server, request, incantators);
    return SUCCESS;
}
