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
    char **team_names = server->params.teams_names;
    int i = 0;

    while (team_names[i] != NULL) {
        if (strcmp(team_names[i], buffer) == 0)
            return true;
        i++;
    }
    return false;
}


void connection_command(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    if (strcmp(buffer, GRAPHIC_NAME) == 0) {
        server->clients[index]->data.is_graphic = true;
        server->clients[index]->data.team_name = strdup(GRAPHIC_NAME);
    } else if (has_team_name(server, buffer)) {
        server->clients[index]->data.team_name = strdup(buffer);
        server->clients[index]->data.is_graphic = false;
    } else {
        send_code(server->clients[index]->fd, "ko");
        return;
    }
    sprintf(response, "%d\n%d %d", 3, server->params.width,
            server->params.height); // TODO: Add real values
    send_code(server->clients[index]->fd, response);
}
