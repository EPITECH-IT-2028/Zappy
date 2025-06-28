/*
** EPITECH PROJECT, 2025
** server
** File description:
** egg_events.c
*/

#include "server.h"
#include "utils.h"

/**
 * @brief Loops through all eggs in a tile and sends them to the GUI clients
 *
 * This function iterates through the eggs in a given tile and sends each egg's
 * information to the GUI clients using the `send_enw` function.
 *
 * @param server Server instance containing the map and egg data
 * @param tile The tile containing eggs to be sent
 */
static
void loop_eggs(server_t *server, map_t *tile)
{
    for (int i = 0; i < tile->eggs_count; i++) {
        send_enw(server, &tile->eggs[i]);
    }
}

/**
 * @brief Sends all eggs to the GUI clients
 *
 * This function iterates through the server's map and sends information
 * about each egg to the GUI clients by calling `loop_eggs`.
 *
 * @param server Server instance containing the map and egg data
 */
void send_all_eggs_to_gui(server_t *server)
{
    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++) {
            loop_eggs(server, &server->map[x][y]);
        }
    }
}

/**
 * @brief Sends an ENW (Egg New) message to all graphic clients
 *
 * This function formats a message indicating the egg's ID, player ID,
 * and its coordinates, then sends it to all clients that are marked as
 * graphic.
 *
 * @param server Server instance containing client list
 * @param egg The egg for which the message is being sent
 */
void send_enw(server_t *server, egg_t *egg)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "enw #%d #%d %d %d\n",
        egg->id, egg->player_id, egg->x, egg->y);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Sends a PFK (Player Fork) message to all graphic clients
 *
 * This function formats a message indicating the player's ID who has forked
 * and sends it to all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param client The client who has forked
 */
void send_pfk(server_t *server, client_t *client)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "pfk #%d\n", client->data.id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Sends an egg broadcast message to all graphic clients
 *
 * This function formats a message indicating the egg's ID and sends it
 * to all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param egg The egg for which the message is being sent
 */
void send_ebo(server_t *server, egg_t *egg)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "ebo #%d\n", egg->id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Sends an EDI (Egg Destroyed) message to all graphic clients
 *
 * This function formats a message indicating
 * the egg's ID that has been destroyed
 * and sends it to all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param egg The egg that has been destroyed
 */
void send_edi(server_t *server, egg_t *egg)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "edi #%d\n", egg->id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}
