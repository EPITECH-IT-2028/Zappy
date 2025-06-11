/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** movement.c
*/

#include "server.h"

static
void set_offset(direction_offset_t *offset, client_t *client)
{
    switch (client->data.direction) {
        case UP:
            offset->x = 0;
            offset->y = -1;
            break;
        case DOWN:
            offset->x = 0;
            offset->y = 1;
            break;
        case LEFT:
            offset->x = -1;
            offset->y = 0;
            break;
        case RIGHT:
            offset->x = 1;
            offset->y = 0;
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
    if (map[last_x][last_y].players > 0)
        map[last_x][last_y].players--;
    check_map_bounds(server, client);
    map[client->data.x][client->data.y].players++;
    sprintf(response->response, "ok");
    return SUCCESS;
}

int rotate_right(server_t *server, response_t *response, request_t *request)
{
    client_t *client = request->client;

    if (!server || !response || !request || !client)
        return ERROR;
    client->data.direction = (client->data.direction + 1) % MAX_DIRECTION;
    sprintf(response->response, "ok");
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
    return SUCCESS;
}
