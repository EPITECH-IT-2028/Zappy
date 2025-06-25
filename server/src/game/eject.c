/*
** EPITECH PROJECT, 2025
** server
** File description:
** eject.c
*/

#include "macro.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>

static
direction_offset_t get_eject_offset(direction_t direction)
{
    direction_offset_t offset = {0, 0};

    switch (direction) {
        case LEFT:
            offset.x = -1;
            offset.y = 0;
            break;
        case RIGHT:
            offset.x = 1;
            offset.y = 0;
            break;
        case UP:
            offset.x = 0;
            offset.y = -1;
            break;
        case DOWN:
            offset.x = 0;
            offset.y = 1;
    }
    return offset;
}

static
int destroy_egg(server_t *server, int x, int y)
{
    if (server->map[x][y].eggs_count <= 0)
        return ERROR;
    while (server->map[x][y].eggs_count > 0) {
        send_edi(server, &server->map[x][y].eggs[0]);
        remove_egg(&server->map[x][y], 0);
    }
    return SUCCESS;
}

static
client_t **init_players_to_eject(map_t *tile, int *count_to_eject,
    client_t *ejector)
{
    client_t **players_to_eject = NULL;

    if (tile->nbr_of_players <= 1)
        return NULL;
    players_to_eject = malloc(sizeof(client_t *) * tile->nbr_of_players);
    if (!players_to_eject)
        return NULL;
    for (int i = 0; i < tile->nbr_of_players; i++) {
        if (tile->players[i] != ejector)
            players_to_eject[i] = tile->players[i];
    }
    *count_to_eject = tile->nbr_of_players - ACTUAL_PLAYER;
    return players_to_eject;
}

static
int knockback_players(server_t *server, client_t *ejector, int x, int y)
{
    direction_offset_t offset = get_eject_offset(ejector->data.direction);
    int count_to_eject = 0;
    client_t **players_to_eject =
        init_players_to_eject(&server->map[x][y], &count_to_eject, ejector);

    if (!players_to_eject || count_to_eject <= 0) {
        free(players_to_eject);
        return ERROR;
    }
    for (int i = 0; i < count_to_eject; i++) {
        remove_player_map(&server->map[x][y], players_to_eject[i]);
        players_to_eject[i]->data.x = (players_to_eject[i]->data.x + offset.x +
            server->params.width) % server->params.width;
        players_to_eject[i]->data.y = (players_to_eject[i]->data.y + offset.y +
            server->params.height) % server->params.height;
        add_player_map(server, server->map,
            players_to_eject[i]);
    }
    free(players_to_eject);
    return SUCCESS;
}

int handle_eject(server_t *server, response_t *response, request_t *request)
{
    int x = request->client->data.x;
    int y = request->client->data.y;
    bool has_eggs = false;
    bool has_players = false;

    if (destroy_egg(server, x, y) == SUCCESS)
        has_eggs = true;
    if (server->map[x][y].nbr_of_players > 1) {
        knockback_players(server, request->client, x, y);
        has_players = true;
    }
    if (has_eggs || has_players)
        snprintf(response->response, BUFFER_SIZE, "ok");
    else
        snprintf(response->response, BUFFER_SIZE, "ko");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        EJECT_TIME);
    return SUCCESS;
}
