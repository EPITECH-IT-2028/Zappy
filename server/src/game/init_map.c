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
    const int map_size = server->params.width * server->params.height;

    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        add_resources(server, map_size * table[i], i);
    }
    return SUCCESS;
}
