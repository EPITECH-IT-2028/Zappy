/*
** EPITECH PROJECT, 2025
** server
** File description:
** respawn_resources.c
*/

#include "server.h"
#include <stdlib.h>


/**
 * @brief Adds resources randomly to the game map
 *
 * This function places a specified number of resources of a given type
 * randomly across the game map. It updates both the map tiles and the
 * server's resource density tracking.
 *
 * @param server Pointer to the server structure containing the game map
 * @param missing_resource Number of resources to add
 * @param type Type of resource to add (0-6: food, linemate, etc.)
 */
static
void add_resource_random(server_t *server, int missing_resource, int type)
{
    int x = 0;
    int y = 0;

    for (int i = 0; i < missing_resource; i++) {
        x = rand() % server->params.width;
        y = rand() % server->params.height;
        increment_resources(&server->map[x][y], type);
        increment_resource_density(server, type);
    }
}

/**
 * @brief Checks the current density of a specific resource type
 *
 * This function retrieves the current count of a specific resource type
 * from the server's density tracking structure. It provides a unified
 * interface to access different resource densities by index.
 *
 * @param server Pointer to the server structure
 * @param type Resource type index (0-6: food, linemate, deraumere, etc.)
 * @return Current density count for the resource type, or -1 on error
 */
static
int check_resource_density(server_t *server, int type)
{
    int density_fields[] = {
        server->density.food, server->density.linemate,
        server->density.deraumere, server->density.sibur,
        server->density.mendiane, server->density.phiras,
        server->density.thystame
    };

    if (!server)
        return -1;
    return density_fields[type];
}

/**
 * @brief Manages resource respawning across the game map
 *
 * This function maintains proper resource density by checking current
 * resource levels against target densities and spawning missing resources.
 * It ensures the game world maintains balanced resource distribution
 * according to predefined ratios.
 *
 * @param server Pointer to the server structure containing game state
 * @return SUCCESS if respawn completed successfully, ERROR otherwise
 */
int respawn_resources(server_t *server)
{
    const float table[TOTAL_RESOURCES] = {
        FOOD, LINEMATE, DERAUMERE, SIBUR, MENDIANE, PHIRAS, THYSTAME
    };
    int map_size = 0;

    if (!server)
        return ERROR;
    map_size = server->params.width * server->params.height;
    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        if (check_resource_density(server, i) < (int)(map_size * table[i])) {
            add_resource_random(server, map_size * table[i] -
                check_resource_density(server, i), i);
        }
    }
    return SUCCESS;
}
