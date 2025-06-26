/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** movement.c
*/

#include "macro.h"
#include "server.h"

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
        remove_player_map(&map[last_x][last_y], client);
    add_player_map(server, map, client);
    if (add_buffer_to_response("ok", &response->response, &response->size)
        == ERROR)
        return ERROR;
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
    if (add_buffer_to_response("ok", &response->response, &response->size)
        == ERROR)
        return ERROR;
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
    if (add_buffer_to_response("ok", &response->response, &response->size)
        == ERROR)
        return ERROR;
    response->client->data.is_busy = true;
    response->client->data.action_end_time =
        get_action_end_time(server, LEFT_TIME);
    return SUCCESS;
}
