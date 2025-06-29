/*
** EPITECH PROJECT, 2025
** server
** File description:
** player_event.c
*/

#include "server.h"
#include "utils.h"

/**
 * @brief Send pin (player inventory) message to all graphic clients
 *
 * This function formats a message containing the player's ID,
 * position, and complete inventory contents.
 *
 * @param server Pointer to the server instance containing client list
 * @param index Index of the client in the server's client array
 */
void send_pin(server_t *server, int index)
{
    client_t *client = server->clients[index];
    char response[BUFFER_SIZE];

    if (client == NULL || client->data.team_name == NULL)
        return;
    snprintf(response, BUFFER_SIZE, "pin #%d %d %d %d %d %d %d %d %d %d\n",
            client->data.id, client->data.x, client->data.y,
            client->data.inventory.food, client->data.inventory.linemate,
            client->data.inventory.deraumere, client->data.inventory.sibur,
            client->data.inventory.mendiane, client->data.inventory.phiras,
            client->data.inventory.thystame);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Send ppo (player position) message to all graphic clients
 *
 * This function formats a message containing the player's ID,
 * coordinates, and facing direction.
 *
 * @param server Pointer to the server instance containing client list
 * @param client Pointer to the client whose position to send
 */
void send_ppo(server_t *server, client_t *client)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "ppo #%d %d %d %d\n",
        client->data.id, client->data.x, client->data.y,
        client->data.direction + 1);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}
