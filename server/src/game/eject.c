/*
** EPITECH PROJECT, 2025
** server
** File description:
** eject.c
*/

#include "macro.h"
#include "server.h"
#include <stdio.h>

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
    for (int i = 0; i < server->map[x][y].eggs_count; i++) {
        send_edi(server, &server->map[x][y].eggs[i]);
        remove_egg(&server->map[x][y], i);
    }
    return SUCCESS;
}

static
int knockback_players(server_t *server, client_t *ejector, int x, int y)
{
    client_t **players = server->map[x][y].players;
    int count = server->map[x][y].nbr_of_players;
    direction_offset_t offset = get_eject_offset(ejector->data.direction);
    int new_x = 0;
    int new_y = 0;

    for (int i = 0; i < count; i++) {
        if (players[i]->data.id == ejector->data.id)
            continue;
        new_x = (players[i]->data.x + offset.x + server->params.width) %
                server->params.width;
        new_y = (players[i]->data.y + offset.y + server->params.height) %
                server->params.height;
        players[i]->data.x = new_x;
        players[i]->data.y = new_y;
    }
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
