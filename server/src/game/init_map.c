/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_map.c
*/

#include "server.h"
#include "macro.h"
#include <stdlib.h>

/**
 * @brief Add initial eggs to the map for all potential players
 *
 * This function creates and places eggs on random map positions
 * for the maximum number of clients that can connect to the server.
 *
 * @param server Pointer to the server structure
 * @return SUCCESS on successful egg placement, ERROR on failure
 */
static
int add_eggs(server_t *server)
{
    int x = 0;
    int y = 0;
    int nbr_of_clients_max = server->params.client_per_team *
        server->params.teams_count;
    egg_t *egg = NULL;

    for (int i = 0; i < nbr_of_clients_max; i++) {
        x = rand() % server->params.width;
        y = rand() % server->params.height;
        egg = create_egg(server->egg_ids,
            (egg_args_t){x, y, UNASSIGNED_PLAYER_ID, UNASSIGNED_PLAYER_ID});
        if (!egg)
            return ERROR;
        if (place_egg(&server->map[x][y], egg) == ERROR)
            return ERROR;
        server->egg_ids++;
    }
    return SUCCESS;
}

/**
 * @brief Increment the specified resource type on a map tile
 *
 * This function increases the count of a specific resource type
 * on a map tile based on the resource type identifier.
 *
 * @param map Pointer to the map tile to modify
 * @param type Resource type identifier (0-6 for different resources)
 */
void increment_resources(map_t *map, int type)
{
    if (type == 0)
        map->food++;
    if (type == 1)
        map->linemate++;
    if (type == 2)
        map->deraumere++;
    if (type == 3)
        map->sibur++;
    if (type == 4)
        map->mendiane++;
    if (type == 5)
        map->phiras++;
    if (type == 6)
        map->thystame++;
}

/**
 * @brief Add a specific number of resources of a given type to the map
 *
 * This function randomly distributes a specified number of resources
 * of a particular type across the game map.
 *
 * @param server Pointer to the server structure
 * @param total Total number of resources to place
 * @param type Resource type identifier to place
 */
static
void add_resources(server_t *server, int total, int type)
{
    int x = 0;
    int y = 0;

    for (int i = 0; i < total; i++) {
        x = rand() % server->params.width;
        y = rand() % server->params.height;
        increment_resources(&server->map[x][y], type);
    }
}

/**
 * @brief Place all initial resources and eggs on the game map
 *
 * This function initializes the game map by distributing all resource
 * types according to their density ratios and placing initial eggs.
 *
 * @param server Pointer to the server structure
 * @return SUCCESS on successful map initialization, ERROR on failure
 */
int place_resources(server_t *server)
{
    const float table[TOTAL_RESOURCES] = {
        FOOD, LINEMATE, DERAUMERE, SIBUR, MENDIANE, PHIRAS, THYSTAME
    };
    int map_size = 0;

    if (!server || !server->map) {
        return ERROR;
    }
    map_size = server->params.width * server->params.height;
    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        add_resources(server, map_size * table[i], i);
    }
    add_eggs(server);
    return SUCCESS;
}
