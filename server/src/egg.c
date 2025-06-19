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

egg_t *create_egg(int id, int x, int y, int player_id)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg) {
        perror("malloc failed");
        return NULL;
    }
    egg->id = id;
    egg->x = x;
    egg->y = y;
    egg->player_id = player_id;
    return egg;
}

int place_egg(map_t *tile, egg_t *egg)
{
    egg_t *new_eggs = realloc(tile->eggs, sizeof(egg_t) *
                        (tile->eggs_count + 1));

    if (!new_eggs) {
        perror("realloc failed");
        return ERROR;
    }
    tile->eggs = new_eggs;
    tile->eggs[tile->eggs_count] = *egg;
    tile->eggs_count++;
    free(egg);
    return SUCCESS;
}

static
int assign_egg_position(map_t *tile, client_t *client, int target)
{
    client_t **new_players = NULL;

    if (target < tile->eggs_count) {
        client->data.x = tile->eggs[target].x;
        client->data.y = tile->eggs[target].y;
        new_players = realloc(tile->players, sizeof(client_t *) *
            (tile->nbr_of_players + 1));
        if (!new_players)
            return false;
        tile->players = new_players;
        tile->players[tile->nbr_of_players] = client;
        tile->nbr_of_players++;
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
    egg_t *target_egg = NULL;

    if (total == 0)
        return ERROR;
    for (int i = 0; i < nb_tiles; i++) {
        x = i / height;
        y = i % height;
        target_egg = &server->map[x][y].eggs[target];
        if (assign_egg_position(&server->map[x][y], client, target)) {
            send_ebo(server, target_egg);
            return SUCCESS;
        }
        target -= server->map[x][y].eggs_count;
    }
    return ERROR;
}
