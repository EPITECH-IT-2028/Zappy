/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** movement.c
*/

#include "macro.h"
#include "server.h"

/**
 * @brief Sets the movement offset based on client's current direction
 *
 * This function calculates the X and Y offset values needed to move the
 * client forward in their current facing direction.
 *
 * @param offset Pointer to direction_offset_t structure to store the result
 * @param client Pointer to the client whose direction determines the offset
 */
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

/**
 * @brief Updates the player's position on the game map
 *
 * This function moves the player forward in their current direction,
 * removing them from their previous position and adding them to the new
 * position on the map. The map wraps around edges.
 *
 * @param server Pointer to the server structure containing the game map
 * @param client Pointer to the client to move
 * @return SUCCESS on successful move, ERROR otherwise
 */
static
int update_player_position(server_t *server, client_t *client)
{
    map_t **map = server->map;
    direction_offset_t offset;
    int last_x = client->data.x;
    int last_y = client->data.y;

    set_offset(&offset, client);
    client->data.x += offset.x;
    client->data.y += offset.y;
    if (map[last_x][last_y].nbr_of_players > 0)
        remove_player_map(&map[last_x][last_y], client);
    return add_player_map(server, map, client);
}

/**
 * @brief Handles the "Forward" command for a client
 *
 * This function processes a client's request to move forward one tile in
 * their current facing direction. Updates the client's position on the map
 * and sends appropriate responses.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to response structure to fill with result
 * @param request Pointer to the client's movement request
 * @return SUCCESS if movement completed successfully, ERROR otherwise
 */
int move_forward(server_t *server, response_t *response, request_t *request)
{
    client_t *client = request->client;

    if (!server || !response || !request || !client)
        return ERROR;
    if (update_player_position(server, client) == ERROR)
        return ERROR;
    if (add_buffer_to_response("ok", &response->response, &response->size)
        == ERROR)
        return ERROR;
    send_ppo(server, client);
    return SUCCESS;
}

/**
 * @brief Handles the "Right" command for a client
 *
 * This function processes a client's request to rotate 90 degrees clockwise.
 * Updates the client's facing direction and sends appropriate responses to
 * the client and GUI.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to response structure to fill with result
 * @param request Pointer to the client's rotation request
 * @return SUCCESS if rotation completed successfully, ERROR otherwise
 */
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
    send_ppo(server, client);
    return SUCCESS;
}

/**
 * @brief Handles the "Left" command for a client
 *
 * This function processes a client's request to rotate 90 degrees
 * counter-clockwise. Updates the client's facing direction and sends
 * appropriate responses to the client and GUI.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to response structure to fill with result
 * @param request Pointer to the client's rotation request
 * @return SUCCESS if rotation completed successfully, ERROR otherwise
 */
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
    send_ppo(server, client);
    return SUCCESS;
}
