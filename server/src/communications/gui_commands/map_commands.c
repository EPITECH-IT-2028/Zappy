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
#include <stdio.h>

/**
 * @brief Send map size information to a GUI client
 *
 * This function sends the map dimensions (width and height) to
 * the specified GUI client using the msz command format.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 */
static
void send_map_size(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "msz %d %d\n", server->params.width,
        server->params.height);
    send_code(server->clients[index]->fd, response);
}

/**
 * @brief Send complete map content to a GUI client
 *
 * This function sends the resource content of every tile on the map
 * to the specified GUI client using bct command format.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 */
static
void send_map_content(server_t *server, int index)
{
    char response[BUFFER_SIZE];
    int x = 0;
    int y = 0;

    for (x = 0; x < server->params.width; x++) {
        for (y = 0; y < server->params.height; y++) {
            snprintf(response, BUFFER_SIZE,
                "bct %d %d %d %d %d %d %d %d %d\n",
                x, y,
                server->map[x][y].food,
                server->map[x][y].linemate,
                server->map[x][y].deraumere,
                server->map[x][y].sibur,
                server->map[x][y].mendiane,
                server->map[x][y].phiras,
                server->map[x][y].thystame);
            send_code(server->clients[index]->fd, response);
        }
    }
}

/**
 * @brief Send content of a specific tile to a GUI client
 *
 * This function sends the resource content of a specific tile
 * to the GUI client using bct command format.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 * @param x X coordinate of the tile
 * @param y Y coordinate of the tile
 */
static
void send_index_content(server_t *server, int index, int x, int y)
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
    send_code(server->clients[index]->fd, response);
}

/**
 * @brief Handle map-related commands from GUI clients
 *
 * This function processes map commands (msz, mct, bct) from GUI clients
 * and sends appropriate responses with map information.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client sending the command
 * @param buffer Command string to process
 */
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
        return send_code(server->clients[index]->fd, "sbp\n");
    if (x < 0 || x >= server->params.width || y < 0
        || y >= server->params.height)
        return send_code(server->clients[index]->fd, "sbp\n");
    return send_index_content(server, index, x, y);
}
