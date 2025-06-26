/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_map.c
*/

#include "server.h"
#include <stdlib.h>

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

static
int find_player_index(map_t *map, client_t *client)
{
    for (int i = 0; i < map->nbr_of_players; i++) {
        if (map->players[i]->data.id == client->data.id) {
            return i;
        }
    }
    return -1;
}

static
void shift_players_array(map_t *map, int player_index)
{
    for (int i = player_index; i < map->nbr_of_players - 1; i++) {
        map->players[i] = map->players[i + 1];
    }
    map->nbr_of_players--;
}

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
