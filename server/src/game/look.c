/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look.c
*/

#include "macro.h"
#include "server.h"
#include <string.h>

static
void add_to_look(char *response, map_t current_case)
{
    for (int i = 0; i < current_case.nbr_of_players; i++)
        strcat(response, " player");
    for (int i = 0; i < current_case.deraumere; i++)
        strcat(response, " deraumere");
    for (int i = 0; i < current_case.eggs_count; i++)
        strcat(response, " egg");
    for (int i = 0; i < current_case.food; i++)
        strcat(response, " food");
    for (int i = 0; i < current_case.linemate; i++)
        strcat(response, " linemate");
    for (int i = 0; i < current_case.mendiane; i++)
        strcat(response, " mendiane");
    for (int i = 0; i < current_case.phiras; i++)
        strcat(response, " phiras");
    for (int i = 0; i < current_case.sibur; i++)
        strcat(response, " sibur");
    for (int i = 0; i < current_case.thystame; i++)
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
            offset.y = -j;
            break;
        case UP:
            offset.x = -j;
            offset.y = -i;
            break;
        case DOWN:
            offset.x = j;
            offset.y = i;
    }
    return offset;
}

static
void process_vision(server_t *server, client_data_t *client_data,
    direction_t direction, char *buffer)
{
    int width = server->params.width;
    int height = server->params.height;
    int vision_depth = DEFAULT_VISION_DEPTH + (client_data->level - 1);
    int vision_width = 0;
    direction_offset_t offset;
    int target_x = 0;
    int target_y = 0;

    for (int i = 0; i < vision_depth; i++) {
        for (int j = vision_width; j >= - vision_width; j--) {
            offset = get_direction_offset(direction, i, j);
            target_x = (((client_data->x + offset.x) % width) + width) % width;
            target_y = (((client_data->y + offset.y) % height) + height)
                % height;
            add_to_look(buffer, server->map[target_x][target_y]);
        }
        if (2 * (vision_width + 1) + 1 <= width)
            vision_width++;
    }
}

static
void handle_vision_direction(server_t *server, response_t *response,
    request_t *request, direction_t direction)
{
    client_data_t *client_data = &request->client->data;
    char buffer[BUFFER_SIZE] = {0};

    process_vision(server, client_data, direction, buffer);
    add_buffer_to_response(buffer, &response->response, &response->size);
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
    add_buffer_to_response("[", &response->response, &response->size);
    handle_direction(&request->client->data, server, response, request);
    response->response[response->size - 2][strlen(response->response[
        response->size - 2]) - REMOVE_USELESS_COMMA] = ' ';
    add_buffer_to_response("]", &response->response, &response->size);
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        LOOK_TIME);
    return SUCCESS;
}
