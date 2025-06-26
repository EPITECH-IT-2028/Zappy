/*
** EPITECH PROJECT, 2025
** server
** File description:
** respawn_resources.c
*/

#include "server.h"
#include <stdlib.h>


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
