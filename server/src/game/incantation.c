/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation.c
*/

#include "incantation_condition.h"
#include "server.h"

/**
 * Check if all conditions are met for an incantation ritual
 * Validates both resource requirements and player level requirements
 *
 * @param server Pointer to the server structure
 * @param request Pointer to the client request
 * @return SUCCESS if all conditions are met, ERROR otherwise
 */
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
int freeze_every_player(server_t *server, request_t *request)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    client_t *clients_frozen[tab_incantation[client->level - 1].player];
    uint8_t nbr_clients_frozen = 1;
    uint8_t lvl_needed = client->level;
    int i = 0;

    clients_frozen[0] = request->client;
    for (; nbr_clients_frozen < unit_space->nbr_of_players;) {
        if (unit_space->players[i]->data.level == lvl_needed) {
            clients_frozen[i] = unit_space->players[i]; 
            i++;
        }
    }
    for (; i >= 0; i--) {
        clients_frozen[i]->data.is_busy = true;
        clients_frozen[i]->data.action_end_time = get_action_end_time(server,
            INCANTATION_TIME);
    }
    return SUCCESS;
}

/**
 * Handle incantation ritual request from a client
 * Validates all preconditions and initiates the ritual by freezing all players
 * 
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure (unused but required by interface)
 * @param request Pointer to the client request containing player data
 * @return SUCCESS if incantation can proceed, ERROR if conditions not met
 */
int handle_incantation(server_t *server, response_t *response,
    request_t *request) {
    if (!server || !response || !request)
        return ERROR;

    if (check_incantation_condition(server, request) == ERROR) {
        return ERROR;
    }
    if (freeze_every_player(server, request) == ERROR) {
        return ERROR;
    }
    return SUCCESS;
}
