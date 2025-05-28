/*
** EPITECH PROJECT, 2025
** server
** File description:
** connection_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

static
bool has_team_name(server_t *server, const char *buffer)
{
    return find_team_index(server, buffer) != ERROR;
}

static
void set_data(server_t *server, int index, const char *name, bool is_graphic)
{
    server->clients[index]->data.is_graphic = is_graphic;
    server->clients[index]->data.team_name = strdup(name);
}

static
int send_ai(server_t *server, int index, char *buffer, char *response)
{
    int team_index = find_team_index(server, buffer);
    int remaining_slots = 0;

    if (server->teams[team_index].clients_count >=
        server->params.client_per_team) {
        send_code(server->clients[index]->fd, "ko");
        return ERROR;
    }
    server->teams[team_index].clients_count++;
    set_data(server, index, buffer, false);
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    sprintf(response, "%d\n%d %d", remaining_slots, server->params.width,
        server->params.height);
    return SUCCESS;
}

void connection_command(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    if (strcmp(buffer, GRAPHIC_NAME) == 0) {
        set_data(server, index, GRAPHIC_NAME, true);
        sprintf(response, "msz %d %d", server->params.width,
            server->params.height);
    } else if (has_team_name(server, buffer)) {
        return send_ai(server, index, buffer, response) == ERROR
            ? send_code(server->clients[index]->fd, "ko")
            : send_code(server->clients[index]->fd, response);
    } else {
        return send_code(server->clients[index]->fd, "ko");
    }
    send_code(server->clients[index]->fd, response);
}
