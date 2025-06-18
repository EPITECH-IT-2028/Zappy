/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** incantation.c
*/

#include "incantation_condition.h"
#include "macro.h"
#include "utils.h"
#include <stdint.h>
#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int remove_needed_ressources(map_t *tile, uint8_t level)
{
    if (!tile || level > 1 || level <= 8) {
        return ERROR;
    }
    tile->deraumere -= tab_incantation[level].deraumere;
    tile->linemate -= tab_incantation[level].linemate;
    tile->mendiane -= tab_incantation[level].mendiane;
    tile->phiras -= tab_incantation[level].phiras;
    tile->sibur -= tab_incantation[level].sibur;
    tile->thystame -= tab_incantation[level].thystame;
    return SUCCESS;
}

int check_if_incantation_failed(
    client_data_t *incantator,
    client_t **clients,
    map_t *tile
)
{
    if (!incantator || !clients || !tile)
        return ERROR;
    if (incantator->x != incantator->incantation.x
        || incantator->y != incantator->incantation.y
        || tile->nbr_of_players < tab_incantation[incantator->level - 1].player
        || tile->linemate < tab_incantation[incantator->level - 1].linemate
        || tile->deraumere < tab_incantation[incantator->level - 1].deraumere
        || tile->sibur < tab_incantation[incantator->level - 1].sibur
        || tile->mendiane < tab_incantation[incantator->level - 1].mendiane
        || tile->phiras < tab_incantation[incantator->level - 1].phiras
        || tile->thystame < tab_incantation[incantator->level - 1].thystame
    )
        return ERROR;
    return SUCCESS;
}

/**
 * Check if resources are sufficient for incantation
 */
static
int check_resources_requirements(map_t *unit_space, uint8_t level)
{
    if (unit_space->nbr_of_players < tab_incantation[level - 1].player
        || unit_space->linemate < tab_incantation[level - 1].linemate
        || unit_space->deraumere < tab_incantation[level - 1].deraumere
        || unit_space->sibur < tab_incantation[level - 1].sibur
        || unit_space->mendiane < tab_incantation[level - 1].mendiane
        || unit_space->phiras < tab_incantation[level - 1].phiras
        || unit_space->thystame < tab_incantation[level - 1].thystame)
        return ERROR;
    return SUCCESS;
}

/**
 * Count players with required level on the tile
 */
static
uint8_t count_players_with_level(map_t *unit_space, uint8_t level_needed)
{
    uint8_t count = 0;

    for (int i = 0; i < unit_space->nbr_of_players; i++) {
        if (unit_space->players[i]->data.level == level_needed)
            count++;
    }
    return count;
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
    uint8_t count_player_lvl_needed;

    if (check_resources_requirements(unit_space, client->level) == ERROR)
        return ERROR;
    count_player_lvl_needed = count_players_with_level(unit_space,
        client->level);
    if (count_player_lvl_needed < tab_incantation[client->level - 1].player)
        return ERROR;
    return SUCCESS;
}

/**
 * Freeze all players on the current tile during incantation ritual
 * Sets all players as busy and assigns them the same action end time
 * to prevent them from performing other actions during the ritual
 * @param server Pointer to the server structure
 * @param request Pointer to the client request containing position data
 * @return SUCCESS always (function cannot fail)
 */
static
int freeze_every_player(server_t *server, request_t *request)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    uint8_t nbr_of_incantators;

    nbr_of_incantators = build_incantation_group(request->client, unit_space);
    setup_main_incantator(server, client);
    setup_group_members(server, client, nbr_of_incantators);
    return SUCCESS;
}

/**
 * Send elevation message to all incantators
 */
static
void notify_incantators(client_t **incantators)
{
    if (!incantators)
        return;
    for (int i = 0; incantators && incantators[i] != NULL; i++) {
        if (incantators[i]->connected && incantators[i]->fd > 0)
            send_code(incantators[i]->fd, "Elevation underway");
    }
}

static
void add_new_incantation_request(server_t *server, request_t *request)
{
    request_t new_request;

    if (!request->client || !request->client->data.incantation.client_group)
        return;
    request->client->data.incantation.is_incantating = true;
    new_request.client = request->client;
    memcpy(new_request.request, request->request, BUFFER_SIZE);
    notify_incantators(request->client->data.incantation.client_group);
    queue_add_request(server, &new_request);
}

/**
 * Initialize and start new incantation
 */
static
int start_new_incantation(server_t *server, response_t *response,
    request_t *request)
{
    uint8_t nbr = 0;

    if (check_incantation_condition(server, request) == ERROR) {
        return ERROR;
    }
    if (request->client->data.incantation.client_group) {
        free(request->client->data.incantation.client_group);
        request->client->data.incantation.client_group = NULL;
    }
    nbr = build_incantation_group(
        request->client,
        &server->map[request->client->data.x][request->client->data.y]
    );
    if (nbr < tab_incantation[response->client->data.level - 1].player)
        return ERROR;
    if (freeze_every_player(server, request) == ERROR)
        return ERROR;
    send_pic_all(server, request->client->data.incantation.client_group);
    add_new_incantation_request(server, request);
    return SUCCESS;
}

/**
 * Handle incantation ritual request from a client
 * Validates all preconditions and initiates the ritual by
 * freezing all players
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure
 * @param request Pointer to the client request containing player data
 * @return SUCCESS if incantation can proceed, ERROR if conditions not met
 */
int handle_oncoming_incantation(server_t *server, response_t *response,
    request_t *request)
{
    if (!server || !response || !request)
        return ERROR;
    if (request->client->data.incantation.is_incantating == true)
        return handle_ending_incantation(server, response, request);
    return start_new_incantation(server, response, request);
}
