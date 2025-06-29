/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** look.c
*/

#include "macro.h"
#include "server.h"
#include <string.h>

/**
 * @brief Add all items and entities on a tile to the look response
 *
 * This function appends all players, resources, and eggs present
 * on a map tile to the look command response string.
 *
 * @param response String buffer to append the tile contents to
 * @param current_case Map tile containing the items to list
 */
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

/**
 * @brief Calculate direction offset for vision tiles based on direction
 *
 * This function computes the x and y offsets for tiles in the player's
 * vision cone based on their facing direction and tile coordinates.
 *
 * @param direction The direction the player is facing
 * @param i Distance from player (depth in vision)
 * @param j Lateral offset from center of vision
 * @return Direction offset structure with x and y coordinates
 */
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

/**
 * @brief Process the player's vision cone and build the look response
 *
 * This function calculates all tiles visible to the player based on
 * their level and direction, then adds their contents to the buffer.
 *
 * @param server Pointer to the server structure
 * @param client_data Pointer to the client's data including position
 * @param direction The direction the player is facing
 * @param buffer String buffer to store the vision results
 */
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

/**
 * @brief Handle vision processing for a specific direction
 *
 * This function processes the player's vision in a given direction
 * and adds the results to the response structure.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @param direction The direction to process vision for
 */
static
void handle_vision_direction(server_t *server, response_t *response,
    request_t *request, direction_t direction)
{
    client_data_t *client_data = &request->client->data;
    char buffer[BUFFER_SIZE] = {0};

    process_vision(server, client_data, direction, buffer);
    add_buffer_to_response(buffer, &response->response, &response->size);
}

/**
 * @brief Handle vision processing based on client's current direction
 *
 * This function determines the client's facing direction and calls
 * the appropriate vision handler for that direction.
 *
 * @param client_data Pointer to the client's data including direction
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 */
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

/**
 * @brief Handle the look command to show player's vision
 *
 * This function processes the look command by building a formatted
 * response showing all visible tiles and their contents.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @return SUCCESS on successful look processing, ERROR on failure
 */
int handle_look(server_t *server, response_t *response, request_t *request)
{
    if (!server || !response || !request)
        return ERROR;
    add_buffer_to_response("[", &response->response, &response->size);
    handle_direction(&request->client->data, server, response, request);
    response->response[response->size - 1][strlen(response->response[
        response->size - 1]) - REMOVE_USELESS_COMMA] = ' ';
    add_buffer_to_response("]", &response->response, &response->size);
    return SUCCESS;
}
