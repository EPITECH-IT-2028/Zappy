/*
** EPITECH PROJECT, 2025
** server
** File description:
** map_events.c
*/

#include "server.h"
#include "utils.h"

void send_bct(server_t *server, int x, int y)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "bct %d %d %d %d %d %d %d %d %d\n",
        x, y,
        server->map[x][y].food,
        server->map[x][y].linemate,
        server->map[x][y].deraumere,
        server->map[x][y].sibur,
        server->map[x][y].mendiane,
        server->map[x][y].phiras,
        server->map[x][y].thystame);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] != NULL &&
            server->clients[i]->data.is_graphic) {
            send_code(server->clients[i]->fd, response);
        }
    }
}
