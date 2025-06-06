/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** vision.c
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
    if (current_case.eggs > 0)
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
    strcat(response, ", ");
}

static
void handle_left_direction(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    int target_x = 0;
    int target_y = 0;
    int width = server->params.width;
    int height = server->params.height;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = -vision_width; j <= vision_width; j++) {
            target_x = (((client_data->x - i) % width) + width) % width;
            target_y = (((client_data->y + j) % height) + height) % height;
            add_to_look(response->response, server->map[target_y][target_x]);
        }
        if (vision_width + 1 < server->params.height)
            vision_width++;
    }
}

static
void handle_right_direction(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    int target_x = 0;
    int target_y = 0;
    int width = server->params.width;
    int height = server->params.height;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = -vision_width; j <= vision_width; j++) {
            target_x = (((client_data->x + i) % width) + width) % width;
            target_y = (((client_data->y + j) % height) + height) % height;
            add_to_look(response->response, server->map[target_y][target_x]);
        }
        if (vision_width + 1 < server->params.height)
            vision_width++;
    }
}

static
void handle_up_direction(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    int target_x = 0;
    int target_y = 0;
    int width = server->params.width;
    int height = server->params.height;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = -vision_width; j <= vision_width; j++) {
            target_x = (((client_data->x + j) % width) + width) % width;
            target_y = (((client_data->y - i) % height) + height) % height;
            add_to_look(response->response, server->map[target_y][target_x]);
        }
        if (vision_width + 1 < server->params.height)
            vision_width++;
    }
}

static
void handle_down_direction(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    int target_x = 0;
    int target_y = 0;
    int width = server->params.width;
    int height = server->params.height;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = -vision_width; j <= vision_width; j++) {
            target_x = (((client_data->x + j) % width) + width) % width;
            target_y = (((client_data->y + i) % height) + height) % height;
            add_to_look(response->response, server->map[target_y][target_x]);
        }
        if (vision_width + 1 < server->params.height)
            vision_width++;
    }
}

static
void handle_direction(client_data_t *client_data, server_t *server,
    response_t *response, request_t *request)
{
    switch (client_data->direction) {
        case LEFT:
            handle_left_direction(server, response, request);
            break;
        case RIGHT:
            handle_right_direction(server, response, request);
            break;
        case UP:
            handle_up_direction(server, response, request);
            break;
        case DOWN:
            handle_down_direction(server, response, request);
            break;
    }
}

int handle_vision(server_t *server, response_t *response, request_t *request)
{
    if (!server || !response || !request) {
        return ERROR;
    }
    handle_direction(&request->client->data, server, response, request);
    sprintf(response->response, "[");
    response->response[strlen(response->response)] = ']';
    response->response[strlen(response->response)] = '\0';
    return SUCCESS;
}
