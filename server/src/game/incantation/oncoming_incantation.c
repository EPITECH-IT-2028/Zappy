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

/**
 * @brief Remove required resources from a tile for incantation
 *
 * This function removes the resources needed for an incantation of
 * the specified level from the given map tile.
 *
 * @param tile Pointer to the map tile to remove resources from
 * @param level Level of the incantation (0-6)
 * @return SUCCESS on successful removal, ERROR on invalid parameters
 */
int remove_needed_ressources(map_t *tile, uint8_t level)
{
    if (!tile || level > 6) {
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

/**
 * @brief Check if an incantation has failed due to changed conditions
 *
 * This function verifies if the incantation should fail by checking
 * if the incantator moved or if resources/players are insufficient.
 *
 * @param incantator Pointer to the incantator's client data
 * @param clients Array of all clients on the server
 * @param tile Pointer to the map tile where incantation takes place
 * @return SUCCESS if conditions are still met, ERROR if failed
 */
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
 * @brief Check if resources are sufficient for incantation
 *
 * This function verifies that a tile has enough resources and players
 * to perform an incantation of the specified level.
 *
 * @param unit_space Pointer to the map tile to check
 * @param level Level of incantation to check requirements for
 * @return SUCCESS if requirements met, ERROR if insufficient resources
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
 * @brief Count players with required level on the tile
 *
 * This function counts how many players on a tile have the exact
 * level needed for the incantation.
 *
 * @param unit_space Pointer to the map tile containing players
 * @param level_needed The level that players must have
 * @return Number of players with the required level
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
 * @brief Freeze all players on tile during incantation ritual
 *
 * This function sets all players as busy and assigns them the same
 * action end time to prevent other actions during the ritual.
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
    uint8_t nbr_of_incantators;

    nbr_of_incantators = build_incantation_group(request->client, unit_space);
    setup_main_incantator(server, client);
    setup_group_members(client, nbr_of_incantators);
    return SUCCESS;
}

/**
 * @brief Send elevation message to all incantators
 *
 * This function notifies all connected incantators that an elevation
 * ritual is underway by sending them a message.
 *
 * @param incantators Array of client pointers participating in ritual
 */
static
void notify_incantators(client_t **incantators)
{
    if (!incantators)
        return;
    for (int i = 0; incantators && incantators[i] != NULL; i++) {
        if (incantators[i]->connected && incantators[i]->fd > 0)
            send_code(incantators[i]->fd, "Elevation underway\n");
    }
}

/**
 * @brief Initialize and start new incantation
 *
 * This function validates all conditions and starts a new incantation
 * by building the group, freezing players, and sending notifications.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure
 * @param request Pointer to the client request
 * @return SUCCESS if incantation started, ERROR if conditions not met
 */
static
int start_new_incantation(server_t *server, response_t *response,
    request_t *request)
{
    uint8_t nbr = 0;

    if (check_incantation_condition(server, request) == ERROR)
        return ERROR;
    if (request->client->data.incantation.client_group != NULL)
        return ERROR;
    nbr = build_incantation_group(
        request->client,
        &server->map[request->client->data.x][request->client->data.y]
    );
    if (nbr < tab_incantation[response->client->data.level - 1].player)
        return ERROR;
    if (freeze_every_player(server, request) == ERROR)
        return ERROR;
    send_pic(server, request->client->data.incantation.client_group);
    notify_incantators(request->client->data.incantation.client_group);
    return SUCCESS;
}

/**
 * @brief Handle incantation ritual request from a client
 *
 * This function validates all preconditions and either initiates
 * a new ritual or handles completion of an ongoing incantation.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure
 * @param request Pointer to the client request containing player data
 * @return SUCCESS if incantation handled, ERROR if conditions not met
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
