/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look.c
*/

#include "macro.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

static
void add_to_look(char *response, map_t current_case)
{
    if (current_case.deraumere > 0)
        strcat(response, " deraumere");
    if (current_case.eggs != NULL)
        strcat(response, " egg");
    if (current_case.food > 0)
        strcat(response, " food");
    if (current_case.linemate > 0)
        strcat(response, " linemate");
    if (current_case.mendiane > 0)
        strcat(response, " mendiane");
    if (current_case.phiras > 0)
        strcat(response, " phiras");
    if (current_case.players > 0)
        strcat(response, " player");
    if (current_case.sibur > 0)
        strcat(response, " sibur");
    if (current_case.thystame > 0)
        strcat(response, " thystame");
    strcat(response, ",");
}

static
direction_offset_t get_direction_offset(direction_t direction, int i, int j)
{
    direction_offset_t offset = {0, 0};

    switch (direction) {
        case LEFT:
            offset.x = -i;
            offset.y = j;
            break;
        case RIGHT:
            offset.x = i;
            offset.y = j;
            break;
        case UP:
            offset.x = j;
            offset.y = -i;
            break;
        case DOWN:
            offset.x = j;
            offset.y = i;
    }
    return offset;
}

static
void handle_vision_direction(server_t *server, response_t *response,
    request_t *request, direction_t direction)
{
    client_data_t *client_data = &request->client->data;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    int width = server->params.width;
    int height = server->params.height;
    direction_offset_t offset = {0};
    int target_x = 0;
    int target_y = 0;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = -vision_width; j <= vision_width; j++) {
            offset = get_direction_offset(direction, i, j);
            target_x = (((client_data->x + offset.x) % width) + width) % width;
            target_y = (((client_data->y + offset.y) %
                height) + height) % height;
            add_to_look(response->response, server->map[target_y][target_x]);
        }
        if (2 * (vision_width + 1) + 1 <= width)
            vision_width++;
    }
}

static
void handle_direction(client_data_t *client_data, server_t *server,
    response_t *response, request_t *request)
{
    switch (client_data->direction) {
        case LEFT:
            handle_vision_direction(server, response, request, LEFT);
            break;
        case RIGHT:
            handle_vision_direction(server, response, request, RIGHT);
            break;
        case UP:
            handle_vision_direction(server, response, request, UP);
            break;
        case DOWN:
            handle_vision_direction(server, response, request, DOWN);
            break;
    }
}

int handle_look(server_t *server, response_t *response, request_t *request)
{
    if (!server || !response || !request) {
        return ERROR;
    }
    sprintf(response->response, "[");
    handle_direction(&request->client->data, server, response, request);
    response->response[strlen(response->response) - REMOVE_USELESS_COMMA]
        = ' ';
    strcat(response->response, "]");
    response->response[strlen(response->response)] = '\0';
    response->client->data.is_busy = true;
    response->client->data.action_end_time = server->timer_count +
        (LOOK_TIME * server->params.frequence);
    return SUCCESS;
}
