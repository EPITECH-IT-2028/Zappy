/*
** EPITECH PROJECT, 2025
** server
** File description:
** eject.c
*/

#include "macro.h"
#include "server.h"
#include <stdlib.h>

/**
 * @brief Get the direction offset for ejecting players
 *
 * This function returns the x and y offset coordinates based on
 * the ejector's facing direction for moving ejected players.
 *
 * @param direction The direction the ejector is facing
 * @return Direction offset structure with x and y coordinates
 */
static
direction_offset_t get_eject_offset(direction_t direction)
{
    direction_offset_t offset = {0, 0};

    switch (direction) {
        case LEFT:
            offset.x = -1;
            offset.y = 0;
            break;
        case RIGHT:
            offset.x = 1;
            offset.y = 0;
            break;
        case UP:
            offset.x = 0;
            offset.y = -1;
            break;
        case DOWN:
            offset.x = 0;
            offset.y = 1;
    }
    return offset;
}

/**
 * @brief Destroy all eggs on a tile and send notifications
 *
 * This function removes all eggs from a tile and sends egg destroyed
 * notifications to all connected clients.
 *
 * @param server Pointer to the server structure
 * @param x X coordinate of the tile
 * @param y Y coordinate of the tile
 * @return SUCCESS if eggs were destroyed, ERROR if no eggs present
 */
static
int destroy_egg(server_t *server, int x, int y)
{
    if (server->map[x][y].eggs_count <= 0)
        return ERROR;
    while (server->map[x][y].eggs_count > 0) {
        send_edi(server, &server->map[x][y].eggs[0]);
        remove_egg(&server->map[x][y], 0);
    }
    return SUCCESS;
}

/**
 * @brief Initialize list of players to be ejected from a tile
 *
 * This function creates an array of players to eject, excluding
 * the ejector from the list of players on the tile.
 *
 * @param tile Pointer to the map tile containing players
 * @param count_to_eject Pointer to store the number of players to eject
 * @param ejector Pointer to the client performing the eject
 * @return Array of players to eject, or NULL if none or on error
 */
static
client_t **init_players_to_eject(map_t *tile, int *count_to_eject,
    client_t *ejector)
{
    client_t **players_to_eject = NULL;

    if (tile->nbr_of_players <= 1)
        return NULL;
    players_to_eject = malloc(sizeof(client_t *) * tile->nbr_of_players);
    if (!players_to_eject)
        return NULL;
    for (int i = 0; i < tile->nbr_of_players; i++) {
        if (tile->players[i] != ejector)
            players_to_eject[i] = tile->players[i];
    }
    *count_to_eject = tile->nbr_of_players - ACTUAL_PLAYER;
    return players_to_eject;
}

/**
 * @brief Knockback players from a tile in the ejector's direction
 *
 * This function moves all players (except the ejector) from a tile
 * to an adjacent tile based on the ejector's facing direction.
 *
 * @param server Pointer to the server structure
 * @param ejector Pointer to the client performing the eject
 * @param x X coordinate of the tile to eject from
 * @param y Y coordinate of the tile to eject from
 * @return SUCCESS on successful ejection, ERROR on failure
 */
static
int knockback_players(server_t *server, client_t *ejector, int x, int y)
{
    direction_offset_t offset = get_eject_offset(ejector->data.direction);
    int count_to_eject = 0;
    client_t **players_to_eject =
        init_players_to_eject(&server->map[x][y], &count_to_eject, ejector);

    if (!players_to_eject || count_to_eject <= 0) {
        free(players_to_eject);
        return ERROR;
    }
    for (int i = 0; i < count_to_eject; i++) {
        remove_player_map(&server->map[x][y], players_to_eject[i]);
        players_to_eject[i]->data.x = (players_to_eject[i]->data.x + offset.x +
            server->params.width) % server->params.width;
        players_to_eject[i]->data.y = (players_to_eject[i]->data.y + offset.y +
            server->params.height) % server->params.height;
        add_player_map(server, server->map,
            players_to_eject[i]);
    }
    free(players_to_eject);
    return SUCCESS;
}

/**
 * @brief Send appropriate response based on eject action results
 *
 * This function sends "ok" if eggs or players were ejected,
 * otherwise sends "ko" if nothing was affected by the eject.
 *
 * @param response Pointer to the response structure to fill
 * @param has_eggs Boolean indicating if eggs were destroyed
 * @param has_players Boolean indicating if players were ejected
 * @return SUCCESS on successful response, ERROR on failure
 */
static
int send_correct_response(response_t *response, bool has_eggs,
    bool has_players)
{
    if (has_eggs || has_players) {
        if (add_buffer_to_response("ok", &response->response, &response->size)
            == ERROR)
            return ERROR;
    } else
        if (add_buffer_to_response("ko", &response->response, &response->size)
            == ERROR)
            return ERROR;
    return SUCCESS;
}

/**
 * @brief Handle the eject command from a client
 *
 * This function processes an eject command by destroying eggs and
 * ejecting players from the client's current tile.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @return SUCCESS on successful eject handling, ERROR on failure
 */
int handle_eject(server_t *server, response_t *response, request_t *request)
{
    int x = request->client->data.x;
    int y = request->client->data.y;
    bool has_eggs = false;
    bool has_players = false;

    if (destroy_egg(server, x, y) == SUCCESS)
        has_eggs = true;
    if (server->map[x][y].nbr_of_players > 1) {
        knockback_players(server, request->client, x, y);
        has_players = true;
    }
    if (send_correct_response(response, has_eggs, has_players) == ERROR)
        return ERROR;
    return SUCCESS;
}
