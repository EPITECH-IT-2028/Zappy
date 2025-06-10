/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg.c
*/

#include "server.h"
#include <stdlib.h>

int count_total_eggs(server_t *server)
{
    int total_eggs = 0;

    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++) {
            total_eggs += server->map[x][y].eggs_count;
        }
    }
    return total_eggs;
}

void remove_egg(map_t *tile, int index)
{
    egg_t *new_eggs = NULL;

    if (index < 0 || index >= tile->eggs_count)
        return;
    if (index != tile->eggs_count - 1)
        tile->eggs[index] = tile->eggs[tile->eggs_count - 1];
    tile->eggs_count--;
    if (tile->eggs_count > 0) {
        new_eggs = realloc(tile->eggs, sizeof(egg_t) * tile->eggs_count);
        if (new_eggs)
            tile->eggs = new_eggs;
    }
}

static
int assign_egg_position(map_t *tile, client_t *client, int target)
{
    if (target < tile->eggs_count) {
        client->data.x = tile->eggs[target].x;
        client->data.y = tile->eggs[target].y;
        tile->players++;
        remove_egg(tile, target);
        return true;
    }
    return false;
}

int assign_random_egg_position(server_t *server, client_t *client)
{
    int total = count_total_eggs(server);
    int target = rand() % total;
    int width = server->params.width;
    int height = server->params.height;
    int nb_tiles = width * height;
    int x = 0;
    int y = 0;

    if (total == 0)
        return ERROR;
    for (int i = 0; i < nb_tiles; i++) {
        x = i / height;
        y = i % height;
        if (assign_egg_position(&server->map[x][y], client, target))
            return SUCCESS;
        target -= server->map[x][y].eggs_count;
    }
    return ERROR;
}
