/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg.c
*/

#include "macro.h"
#include "server.h"
#include <stdlib.h>

int count_total_eggs(server_t *server)
{
    int total_eggs = 0;

    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++)
            total_eggs += server->map[x][y].eggs_count;
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
    } else {
        free(tile->eggs);
        tile->eggs = NULL;
    }
}

egg_t *create_egg(int id, egg_args_t args)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg) {
        perror("malloc failed");
        return NULL;
    }
    egg->id = id;
    egg->x = args.x;
    egg->y = args.y;
    egg->player_id = args.player_id;
    egg->team_id = args.team_id;
    return egg;
}

int place_egg(map_t *tile, egg_t *egg)
{
    egg_t *new_eggs = realloc(tile->eggs, sizeof(egg_t) *
                        (tile->eggs_count + 1));

    if (!new_eggs) {
        perror("realloc failed");
        free(new_eggs);
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

    if (target < 0 || target >= tile->eggs_count)
        return false;
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

static
int assign_and_send(server_t *server, client_t *client,
    map_t *tile, int target)
{
    egg_t target_egg;

    target_egg = tile->eggs[target];
    if (!assign_egg_position(tile, client, target))
        return ERROR;
    send_ebo(server, &target_egg);
    return SUCCESS;
}

static
int loop_eggs(server_t *server, client_t *client, map_t *tile, int team_id)
{
    for (int j = 0; j < tile->eggs_count; j++) {
        if (tile->eggs[j].team_id == team_id)
            return assign_and_send(server, client, tile, j);
    }
    return ERROR;
}

static
int find_and_assign_team_egg(server_t *server, client_t *client, int team_id)
{
    int nb_tiles = server->params.width * server->params.height;
    int x = 0;
    int y = 0;

    for (int i = 0; i < nb_tiles; i++) {
        x = i / server->params.height;
        y = i % server->params.height;
        if (loop_eggs(server, client, &server->map[x][y], team_id) == SUCCESS)
            return SUCCESS;
    }
    return ERROR;
}

int assign_random_egg_position(server_t *server, client_t *client)
{
    if (find_and_assign_team_egg(server, client,
        client->data.team_id) == SUCCESS)
        return SUCCESS;
    if (find_and_assign_team_egg(server, client,
        UNASSIGNED_PLAYER_ID) == SUCCESS)
        return SUCCESS;
    return ERROR;
}
