/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_events.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"

static
void egg_laid(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++) {
            if (server->map[x][y].eggs != 0) {
                snprintf(response, BUFFER_SIZE, "enw #%d #%d %d %d",
                server->map[x][y].eggs,
                -1,
                x, y);
                send_code(server->clients[index]->fd, response);
            }
            send_code(server->clients[index]->fd, response);
        }
    }
}
