/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** movement.c
*/

#include "macro.h"
#include "server.h"
#include <stdlib.h>

static
void set_offset(direction_offset_t *offset, client_t *client)
{
    switch (client->data.direction) {
        case LEFT:
            offset->x = -1;
            offset->y = 0;
            break;
        case RIGHT:
            offset->x = 1;
            offset->y = 0;
            break;
        case UP:
            offset->x = 0;
            offset->y = -1;
            break;
        case DOWN:
            offset->x = 0;
            offset->y = 1;
            break;
    }
}

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

static
int remove_player(map_t *map, client_t *client)
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

static
int add_player(server_t *server, map_t **map, client_t *client)
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

int move_forward(server_t *server, response_t *response, request_t *request)
{
    client_t *client = request->client;
    map_t **map = server->map;
    direction_offset_t offset;
    int last_x = client->data.x;
    int last_y = client->data.y;

    if (!server || !response || !request || !client)
        return ERROR;
    set_offset(&offset, client);
    client->data.x += offset.x;
    client->data.y += offset.y;
    if (map[last_x][last_y].nbr_of_players > 0)
        remove_player(&map[last_x][last_y], client);
    add_player(server, map, client);
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time =
        get_action_end_time(server, FORWARD_TIME);
    return SUCCESS;
}

int rotate_right(server_t *server, response_t *response, request_t *request)
{
    client_t *client = request->client;

    if (!server || !response || !request || !client)
        return ERROR;
    client->data.direction = (client->data.direction + 1 + MAX_DIRECTION)
        % MAX_DIRECTION;
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time =
        get_action_end_time(server, RIGHT_TIME);
    return SUCCESS;
}

int rotate_left(server_t *server, response_t *response, request_t *request)
{
    client_t *client = request->client;

    if (!server || !response || !request || !client)
        return ERROR;
    client->data.direction = (client->data.direction - 1 + MAX_DIRECTION)
        % MAX_DIRECTION;
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time =
        get_action_end_time(server, LEFT_TIME);
    return SUCCESS;
}
