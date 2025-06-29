/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_map.c
*/

#include "server.h"
#include <stdlib.h>

/**
 * @brief Ensures client coordinates stay within map boundaries by
 * wrapping around edges
 * @param server Server containing map parameters
 * @param client Client whose position needs to be checked
 *
 * Implements a toroidal map where going beyond one edge wraps
 * to the opposite edge.
 * Negative x-coordinates wrap to the right edge, and coordinates
 * beyond width wrap to left.
 * Negative y-coordinates wrap to the bottom edge, and coordinates
 * beyond height wrap to top.
 */
static
void check_map_bounds(server_t *server, client_t *client)
{
    if (client->data.x < 0)
        client->data.x = server->params.width - 1;
    else if (client->data.x >= server->params.width)
        client->data.x = 0;
    if (client->data.y < 0)
        client->data.y = server->params.height - 1;
    else if (client->data.y >= server->params.height)
        client->data.y = 0;
}

/**
 * @brief Finds the index of a player in a map tile's player array
 * @param map Pointer to the map tile
 * @param client Client to search for
 *
 * Searches through the players array for a client with matching ID.
 * Skips disconnected players and empty slots.
 *
 * @return Index of the player if found, -1 if not found
 */
static
int find_player_index(map_t *map, client_t *client)
{
    for (int i = 0; i < map->nbr_of_players; i++) {
        if (!map->players[i] || !map->players[i]->connected)
            continue;
        if (map->players[i]->data.id == client->data.id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Shifts elements in the players array to fill
 * the gap after removing a player
 * @param map Pointer to the map tile
 * @param player_index Index of the player being removed
 *
 * Moves all players after the removed player one position
 * forward in the array
 * and decrements the player count.
 */
static
void shift_players_array(map_t *map, int player_index)
{
    for (int i = player_index; i < map->nbr_of_players - 1; i++) {
        map->players[i] = map->players[i + 1];
    }
    map->nbr_of_players--;
}

/**
 * @brief Removes a player from a map tile
 * @param map Pointer to the map tile
 * @param client Client to be removed
 *
 * Finds the player in the tile's player array, removes them
 * by shifting the array, and reallocates memory to reduce the array size.
 * If the tile has no more players, the players array is freed.
 *
 * @return SUCCESS (0) if the player was successfully removed, ERROR on failure
 */
int remove_player_map(map_t *map, client_t *client)
{
    int player_index;
    client_t **temp = NULL;

    if (!map || !client || !map->players)
        return ERROR;
    player_index = find_player_index(map, client);
    if (player_index == -1)
        return ERROR;
    shift_players_array(map, player_index);
    if (map->nbr_of_players > 0) {
        temp = realloc(map->players,
            sizeof(client_t *) * map->nbr_of_players);
        if (!temp)
            return ERROR;
        map->players = temp;
    } else {
        free(map->players);
        map->players = NULL;
    }
    return SUCCESS;
}

/**
 * @brief Adds a player to the map at their current coordinates
 * @param server Server containing map parameters
 * @param map 2D array of map tiles
 * @param client Client to be added to the map
 *
 * Ensures the client's coordinates are within bounds,
 * then adds them to the appropriate
 * map tile by reallocating the players array and incrementing
 * the player count.
 *
 * @return SUCCESS if the player was successfully added, ERROR
 * on memory allocation failure
 */
int add_player_map(server_t *server, map_t **map, client_t *client)
{
    map_t *tile = NULL;
    client_t **temp = NULL;

    check_map_bounds(server, client);
    tile = &map[client->data.x][client->data.y];
    temp = realloc(tile->players,
        sizeof(client_t *) * (tile->nbr_of_players + 1));
    if (!temp)
        return ERROR;
    tile->players = temp;
    tile->players[tile->nbr_of_players] = client;
    tile->nbr_of_players++;
    return SUCCESS;
}
