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
    if (is_graphic) {
        server->clients[index]->data.id = -1;
    } else {
        server->clients[index]->data.id = server->ids;
        server->ids++;
    }
    if (server->clients[index]->data.team_name == NULL)
        perror("strdup failed");
    return SUCCESS;
}

static
void send_new_player_to_gui(server_t *server, int index)
{
    pthread_mutex_lock(&server->clients_mutex);
    send_pnw(server, index);
    pthread_mutex_unlock(&server->clients_mutex);
}

static
int send_ai(server_t *server, int index, char *buffer, char *response)
{
    int team_index = find_team_index(server, buffer);
    int remaining_slots = 0;

    if (server->teams[team_index].clients_count >=
        server->params.client_per_team || team_index == ERROR) {
        send_code(server->clients[index]->fd, "ko\n");
        return ERROR;
    }
    server->teams[team_index].clients_count++;
    if (set_data(server, index, buffer, false) == ERROR)
        return ERROR;
    if (assign_random_egg_position(server, server->clients[index]) == ERROR)
        return ERROR;
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    snprintf(response, BUFFER_SIZE, "%d\n%d %d\n", remaining_slots,
        server->params.width, server->params.height);
    send_new_player_to_gui(server, index);
    return SUCCESS;
}

static
int send_gui(server_t *server, int index, char *buffer)
{
    if (set_data(server, index, buffer, true) == ERROR)
        return ERROR;
    map_commands(server, index, "msz");
    time_commands(server, index, "sgt");
    map_commands(server, index, "mct");
    player_commands(server, index, "tna");
    send_all_eggs_to_gui(server);
    return SUCCESS;
}

void connection_command(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    printf("Connection command request\n");
    if (strcmp(buffer, GRAPHIC_NAME) == 0) {
        send_gui(server, index, buffer);
        return;
    } else if (has_team_name(server, buffer)) {
        return send_ai(server, index, buffer, response) == ERROR
            ? send_code(server->clients[index]->fd, "ko\n")
            : send_code(server->clients[index]->fd, response);
    } else {
        return send_code(server->clients[index]->fd, "ko\n");
    }
    send_code(server->clients[index]->fd, response);
    printf("Connection response sended\n");
}
