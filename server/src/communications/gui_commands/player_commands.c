/*
** EPITECH PROJECT, 2025
** server
** File description:
** player_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

static
void send_teams_name(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    for (int i = 0; i < server->params.teams_count; i++) {
        if (server->teams[i].name != NULL) {
            snprintf(response, BUFFER_SIZE, "tna %s", server->teams[i].name);
            send_code(server->clients[index]->fd, response);
        }
    }
}

static
void player_position(server_t *server, int index, int id)
{
    char response[BUFFER_SIZE];
    client_t *client = find_ai_by_id(server, id);

    if (client == NULL) {
        send_code(server->clients[index]->fd, "sbp");
        return;
    }
    snprintf(response, BUFFER_SIZE, "ppo #%d %d %d %d",
        id,
        client->data.x,
        client->data.y,
        client->data.orientation);
    send_code(server->clients[index]->fd, response);
}

void player_commands(server_t *server, int index, char *buffer)
{
    int player_id;
    
    if (strcmp(buffer, "tna") == 0) {
        send_teams_name(server, index);
    } else if (sscanf(buffer, "ppo #%d", &player_id) == 1) {
        player_position(server, index, player_id);
    } else {
        send_code(server->clients[index]->fd, "suc");
    }
}
