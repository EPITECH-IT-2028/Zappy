/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** broadcast.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * @brief Calculates the shortest distance between two coordinates
 * on a wrapped map
 *
 * This function determines the shortest path between two coordinates,
 * taking into
 * account the possibility of wrapping around the map edges (like a torus).
 *
 * @param coord1 First coordinate
 * @param coord2 Second coordinate
 * @param map_size Size of the map dimension (width or height)
 * @return The shortest signed distance between the coordinates
 *         - Positive indicates coord2 is ahead of coord1
 *         - Negative indicates coord2 is behind coord1
 */
static
int calculate_shortest_distance_component(int coord1, int coord2, int map_size)
{
    int direct = ABS(coord2 - coord1);
    int wrapped = map_size - direct;

    if (direct < wrapped) {
        return (coord2 - coord1);
    } else if (coord2 > coord1) {
        return -(wrapped);
    } else {
        return wrapped;
    }
}

/**
 * @brief Calculates the player's rotation based on their direction
 *
 * This function sets the player's rotation angle based on their current
 * direction (UP, RIGHT, DOWN, LEFT).
 *
 * @param client Client data containing the player's direction
 * @param player_rotation Pointer to store the calculated rotation angle
 */
static
void calculate_direction_tile_test(const client_data_t *client,
    double *player_rotation)
{
    switch (client->direction) {
        case UP:
            *player_rotation = 270;
            break;
        case RIGHT:
            *player_rotation = 0;
            break;
        case DOWN:
            *player_rotation = 90;
            break;
        case LEFT:
            *player_rotation = 180;
            break;
    }
}

/**
 * @brief Calculates the direction tile (1-8) for a broadcast message
 *
 * This function determines which direction tile to send to a client
 * receiving a broadcast message based on the relative positions of
 * the emitter and receiver on the game map.
 *
 * @param server Server instance containing map dimensions
 * @param emitter Client data of the message sender
 * @param client Client data of the message receiver
 * @return Direction tile value (0-8)
 *         - 0: same tile as emitter
 *         - 1-8: direction tiles around emitter
 */
static
int calculate_direction_tile(server_t *server, const client_data_t *emitter,
    const client_data_t *client)
{
    int dx = 0;
    int dy = 0;
    double angle_deg = 0;
    int tile = 0;
    double player_rotation = 0;

    dx = calculate_shortest_distance_component(client->x, emitter->x,
        server->params.width);
    dy = calculate_shortest_distance_component(client->y, emitter->y,
        server->params.height);
    angle_deg = atan2(dy, dx) * HALF_CIRCLE_DEG / M_PI;
    if (angle_deg < 0)
        angle_deg += FULL_CIRCLE_DEG;
    calculate_direction_tile_test(client, &player_rotation);
    angle_deg = fmod(angle_deg - player_rotation +
        FULL_CIRCLE_DEG, FULL_CIRCLE_DEG);
    tile = ((int)((angle_deg + DIRECTION_TOLERANCE) /
        DEGREES_PER_DIRECTION) % NUM_DIRECTIONS) + 1;
    return tile;
}

/**
 * @brief Determines which clients receive a broadcast and calculates their
 * direction tiles
 *
 * This function iterates through all connected clients and marks which ones
 * should receive the broadcast message. For each receiver, it calculates
 * the appropriate direction tile representing where the sound came from.
 *
 * @param server Server instance containing client list
 * @param emitter The client who sent the broadcast
 * @param results Array to store results for each potential receiver
 */
static
void transmit_sound(server_t *server, client_t *emitter,
    sound_result_t *results)
{
    client_t *client = NULL;

    if (!server || !emitter || !results)
        return;
    for (int i = 1; i < server->nfds; i++) {
        if (!server->clients[i]) {
            results[i].received = false;
            continue;
        }
        client = server->clients[i];
        if (client->data.is_graphic || client->fd == emitter->fd) {
            results[i].received = false;
            continue;
        }
        results[i].direction_tile = calculate_direction_tile(server,
            &emitter->data, &client->data);
        results[i].received = true;
    }
}

/**
 * @brief Sends the broadcast message to all receiving players
 *
 * This function iterates through the results list and sends the message
 * to all players marked as receivers, formatting the message with
 * the appropriate direction tile.
 *
 * @param server Server instance containing client list
 * @param results Array indicating which clients receive the message and
 * their direction tiles
 * @param message The text content to broadcast
 */
static
void send_broadcast_to_player(server_t *server,
    sound_result_t *results, const char *message)
{
    client_t *receiver = NULL;
    char sound_message[BUFFER_SIZE] = {0};

    for (int i = 1; i < server->nfds; i++) {
        if (!results[i].received)
            continue;
        receiver = server->clients[i];
        if (!receiver || receiver->data.is_graphic)
            continue;
        snprintf(sound_message, sizeof(sound_message),
                "message %d, %s\n", results[i].direction_tile, message);
        send_code(receiver->fd, sound_message);
    }
}

/**
 * @brief Orchestrates the broadcasting of a message from one client to others
 *
 * This function handles the complete broadcast process from setting up
 * the receiver list to sending the formatted messages to each recipient.
 *
 * @param server Server instance
 * @param request The client request containing sender information
 * @param message The message text to broadcast
 * @return SUCCESS on successful broadcast, ERROR otherwise
 */
static
int client_broadcast_sound(server_t *server, request_t *request,
    const char *message)
{
    sound_result_t *results = NULL;

    if (!server || !server->clients || !request || !request->client ||
        request->client->data.is_graphic)
        return ERROR;
    results = calloc(server->nfds, sizeof(sound_result_t));
    if (!results)
        return ERROR;
    transmit_sound(server, request->client, results);
    send_broadcast_to_player(server, results, message);
    free(results);
    return SUCCESS;
}

/**
 * @brief Handles a broadcast command from a client
 *
 * This function processes a broadcast request, extracts the message content,
 * sends it to all appropriate receivers, and prepares the response
 * for the sending client.
 *
 * @param server Server instance
 * @param response Response structure to fill with result
 * @param request The client's broadcast request
 * @return SUCCESS if broadcast was processed successfully, ERROR otherwise
 */
int handle_broadcast(server_t *server, response_t *response,
    request_t *request)
{
    client_t *client = request->client;
    char *broadcast = NULL;

    if (!client || client->data.is_graphic ||
        !server || !response || !request) {
        return ERROR;
    }
    broadcast = get_text_in_commands(request->request, WORD_BROADCAST_LENGTH);
    if (!broadcast) {
        return ERROR;
    }
    if (client_broadcast_sound(server, request, broadcast) == ERROR) {
        free(broadcast);
        return ERROR;
    }
    send_pbc(server, client, broadcast);
    free(broadcast);
    add_buffer_to_response("ok", &response->response, &response->size);
    return SUCCESS;
}
