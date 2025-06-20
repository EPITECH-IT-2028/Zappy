/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_events.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"

/**
 * @brief Sends a PBC (Player Broadcast) message to all graphic clients
 *
 * This function formats a broadcast message with the client's ID and the
 * provided message, then sends it to all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param client The client who sent the broadcast
 * @param message The text content of the broadcast
 */
void send_pbc(server_t *server, client_t *client, const char *message)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "pbc #%d %s",
        client->data.id, message);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

void send_pex(server_t *server, client_t *client)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "pex #%d", client->data.id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Sends a PDR (Player Drop Resource) message to all graphic clients
 *
 * This function formats a message indicating the player's ID and the resource
 * ID that was dropped then sends it to all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param client The client who dropped the resource
 * @param resource_id The ID of the resource that was dropped
 */
void send_pdr(server_t *server, client_t *client, int resource_id)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "pdr #%d %d",
        client->data.id, resource_id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Sends a PGT (Player Get Resource) message to all graphic clients
 *
 * This function formats a message indicating the player's ID and the resource
 * ID that was obtained, then sends it to
 * all clients that are marked as graphic.
 *
 * @param server Server instance containing client list
 * @param client The client who obtained the resource
 * @param resource_id The ID of the resource that was obtained
 */
void send_pgt(server_t *server, client_t *client, int resource_id)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "pgt #%d %d",
        client->data.id, resource_id);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}
