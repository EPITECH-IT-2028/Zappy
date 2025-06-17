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
#include <string.h>

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

static
void send_pic(int fd, client_t **incantators, char *response)
{
    char id_incantator[INCANTATION_ID_LEN] = {0};

    for (int i = 0; incantators[i]; i++) {
        if (incantators[i] && incantators[i]->fd > 0) {
            sprintf(id_incantator, "#%d ", incantators[i]->data.id);
            strcat(response, id_incantator);
        }
    }
    send_code(fd, response);
}

void send_pic_all(server_t *server, client_t **incantators)
{
    char response[BUFFER_SIZE];

    if (!incantators || !incantators[INDEX_INCANTATOR])
        return;
    snprintf(response, BUFFER_SIZE, "pic %d %d",
        incantators[INDEX_INCANTATOR]->data.x,
        incantators[INDEX_INCANTATOR]->data.y);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] && server->clients[i]->fd > 0 && 
            server->clients[i]->data.is_graphic &&
            server->clients[i]->connected) {
            send_pic(server->clients[i]->fd, incantators, response); 
        }
    }
}
