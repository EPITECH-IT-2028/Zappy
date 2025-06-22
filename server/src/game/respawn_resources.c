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
    switch (type) {
        case 0: return server->density.food;
        case 1: return server->density.linemate;
        case 2: return server->density.deraumere;
        case 3: return server->density.sibur;
        case 4: return server->density.mendiane;
        case 5: return server->density.phiras;
        case 6: return server->density.thystame;
        default: return 0;
    }
}

int respawn_resources(server_t *server)
{
    const float table[TOTAL_RESOURCES] = {
        FOOD, LINEMATE, DERAUMERE, SIBUR, MENDIANE, PHIRAS, THYSTAME
    };
    int map_size = 0;

    if (!server)
        return ERROR;
    
    printf("Current resource density:\n");
    printf("  Food: %d\n", server->density.food);
    printf("  Linemate: %d\n", server->density.linemate);
    printf("  Deraumere: %d\n", server->density.deraumere);
    printf("  Sibur: %d\n", server->density.sibur);
    printf("  Mendiane: %d\n", server->density.mendiane);
    printf("  Phiras: %d\n", server->density.phiras);
    printf("  Thystame: %d\n", server->density.thystame);
    map_size = server->params.width * server->params.height;
    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        if (check_resource_density(server, i) < map_size * table[i]) {
            add_resource_random(server, map_size * table[i] - check_resource_density(server, i), i);
        }
    }
    printf("After resource density:\n");
    printf("  Food: %d\n", server->density.food);
    printf("  Linemate: %d\n", server->density.linemate);
    printf("  Deraumere: %d\n", server->density.deraumere);
    printf("  Sibur: %d\n", server->density.sibur);
    printf("  Mendiane: %d\n", server->density.mendiane);
    printf("  Phiras: %d\n", server->density.phiras);
    printf("  Thystame: %d\n", server->density.thystame);
    return SUCCESS;
}
