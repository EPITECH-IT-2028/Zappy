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
int set_data(server_t *server, int index, const char *name, bool is_graphic)
{
    server->clients[index]->data.is_graphic = is_graphic;
    server->clients[index]->data.team_name = strdup(name);
    if (server->clients[index]->data.team_name == NULL)
        perror("strdup failed");
    return SUCCESS;
}

static
int send_ai(server_t *server, int index, char *buffer, char *response)
{
    int team_index = find_team_index(server, buffer);
    int remaining_slots = 0;

    if (server->teams[team_index].clients_count >=
        server->params.client_per_team || team_index == ERROR) {
        send_code(server->clients[index]->fd, "ko");
        return ERROR;
    }
    server->teams[team_index].clients_count++;
    if (set_data(server, index, buffer, false) == ERROR)
        return ERROR;
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    snprintf(response, BUFFER_SIZE, "%d\n%d %d", remaining_slots,
        server->params.width, server->params.height);
    return SUCCESS;
}

static
int send_gui(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    if (set_data(server, index, buffer, true) == ERROR)
        return ERROR;
    snprintf(response, BUFFER_SIZE, "msz %d %d", server->params.width,
        server->params.height);
    send_code(server->clients[index]->fd, response);
    return SUCCESS;
}

void connection_command(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    if (strcmp(buffer, GRAPHIC_NAME) == 0) {
        set_data(server, index, GRAPHIC_NAME, true);
        send_gui(server, index, buffer);
    } else if (has_team_name(server, buffer)) {
        return send_ai(server, index, buffer, response) == ERROR
            ? send_code(server->clients[index]->fd, "ko")
            : send_code(server->clients[index]->fd, response);
    } else {
        return send_code(server->clients[index]->fd, "ko");
    }
    send_code(server->clients[index]->fd, response);
}
