/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game_events.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

static
void loop_eggs(server_t *server, int index, map_t map)
{
    char response[BUFFER_SIZE];

    if (map.eggs_count == 0)
        return;
    for (int i = 0; i < map.eggs_count; i++) {
        snprintf(response, BUFFER_SIZE, "enw #%d #%d %d %d",
            map.eggs[i].id, map.eggs[i].player_id, map.eggs[i].x,
                 map.eggs[i].y);
        send_code(server->clients[index]->fd, response);
    }
}

static
void egg_laid(server_t *server, int index)
{
    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++) {
            loop_eggs(server, index, server->map[x][y]);
        }
    }
}

static
event_t *init_event(void)
{
    static event_t events[] = {
        {"enw", egg_laid},
        {NULL, NULL}
    };

    return events;
}

void game_events(server_t *server, int index, char *buffer)
{
    event_t *event = init_event();

    for (int j = 0; event[j].name != NULL; j++) {
        if (strcmp(buffer, event[j].name) == 0) {
            event[j].f(server, index);
            return;
        }
    }
    send_code(server->clients[index]->fd, "suc");
}
