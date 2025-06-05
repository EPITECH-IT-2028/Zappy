/*
** EPITECH PROJECT, 2025
** src
** File description:
** map_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

static
void send_map_size(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "msz %d %d", server->params.width,
        server->params.height);
    send_code(server->clients[index]->fd, response);
}

static
void send_map_content(server_t *server, int index)
{
    char response[BUFFER_SIZE];
    int x = 0;
    int y = 0;

    for (x = 0; x < server->params.width; x++) {
        for (y = 0; y < server->params.height; y++) {
            snprintf(response, BUFFER_SIZE, "bct %d %d %d %d %d %d %d %d",
                x, y,
                server->map[x][y].food,
                server->map[x][y].linemate,
                server->map[x][y].deraumere,
                server->map[x][y].sibur,
                server->map[x][y].mendiane,
                server->map[x][y].phiras);
            send_code(server->clients[index]->fd, response);
        }
    }
}

static
void send_index_content(server_t *server, int index, int x, int y)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "bct %d %d %d %d %d %d %d %d",
        x, y,
        server->map[y][x].food,
        server->map[y][x].linemate,
        server->map[y][x].deraumere,
        server->map[y][x].sibur,
        server->map[y][x].mendiane,
        server->map[y][x].phiras);
    send_code(server->clients[index]->fd, response);
}

void map_commands(server_t *server, int index, char *buffer)
{
    int x = 0;
    int y = 0;

    if (buffer == NULL || server == NULL) {
        perror("map_commands: NULL argument");
        return;
    }
    if (strcmp(buffer, "msz") == 0)
        return send_map_size(server, index);
    else if (strcmp(buffer, "mct") == 0)
        return send_map_content(server, index);
    if (sscanf(buffer, "bct %d %d", &x, &y) != 2)
        return send_code(server->clients[index]->fd, "sbp");
    if (x < 0 || x >= server->params.width || y < 0
        || y >= server->params.height)
        return send_code(server->clients[index]->fd, "sbp");
    return send_index_content(server, index, x, y);
}
