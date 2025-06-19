/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connection_event.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdio.h>

/**
 * @brief Sends the pnw (player new) event to the GUI client.
 *
 * @param server The server instance.
 * @param index The index of the player in the server's client list.
 * @param gui The index of the GUI client to send the event to.
 */
void send_pnw(server_t *server, int index, int gui)
{
    char response[BUFFER_SIZE];
    client_t *client = server->clients[index];

    snprintf(response, BUFFER_SIZE, "pnw #%d %d %d %d %d %s",
        client->data.id, client->data.x, client->data.y,
        client->data.orientation, client->data.level, client->data.team_name);
    send_code(server->clients[gui]->fd, response);
}

/**
 * @brief Sends the pdi (player disconnect) event to the GUI client.
 *
 * @param server The server instance.
 * @param index The index of the player in the server's client list.
 * @param gui The index of the GUI client to send the event to.
 */
void send_pdi(server_t *server, int index, int gui)
{
    char response[BUFFER_SIZE];
    client_t *client = server->clients[index];

    snprintf(response, BUFFER_SIZE, "pdi #%d", client->data.id);
    send_code(server->clients[gui]->fd, response);
}

/**
 * @brief Sends the pdi (player disconnect) event to all GUI clients.
 *
 * @param server The server instance.
 * @param index The index of the player in the server's client list.
 */
void send_pdi_all(server_t *server, int index)
{
    for (int i = 1; i < server->nfds; i++) {
        if (i != index && server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_pdi(server, index, i);
        }
    }
}
