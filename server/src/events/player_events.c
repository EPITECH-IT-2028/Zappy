/*
** EPITECH PROJECT, 2025
** server
** File description:
** player_event.c
*/

#include "server.h"
#include "utils.h"

void send_pin(server_t *server, int index)
{
    client_t *client = server->clients[index];
    char response[BUFFER_SIZE];

    if (client == NULL || client->data.team_name == NULL)
        return;
    snprintf(response, BUFFER_SIZE, "pin #%d %d %d %d %d %d %d %d %d %d",
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
