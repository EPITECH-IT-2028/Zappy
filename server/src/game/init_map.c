/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_map.c
*/

#include "server.h"
#include "macro.h"
#include <stdlib.h>

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
        egg = create_egg(server->egg_ids, x, y, UNASSIGNED_PLAYER_ID, 
            UNASSIGNED_PLAYER_ID);
        if (!egg)
            return ERROR;
        if (place_egg(&server->map[x][y], egg) == ERROR)
            return ERROR;
        server->egg_ids++;
    }
    return SUCCESS;
}

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
