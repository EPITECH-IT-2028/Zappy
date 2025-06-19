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
void loop_eggs(server_t *server, int index, map_t *tile)
{
    for (int i = 0; i < tile->eggs_count; i++) {
        send_enw(server, index, &tile->eggs[i]);
    }
}

void send_all_eggs_to_gui(server_t *server, int index)
{
    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++) {
            loop_eggs(server, index, &server->map[x][y]);
        }
    }
}

void send_enw(server_t *server, int index, egg_t *egg)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "enw #%d #%d %d %d",
        egg->id, egg->player_id, egg->x, egg->y);
    send_code(server->clients[index]->fd, response);
}
