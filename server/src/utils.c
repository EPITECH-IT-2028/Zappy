/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils.c
*/

#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <stdio.h>
#include "macro.h"

void send_code(int fd, const char *msg)
{
    char buffer[2048];

    snprintf(buffer, sizeof(buffer), "%s\n", msg);
    if (write(fd, buffer, strlen(buffer)) < 0)
        perror("write failed");
}

void remove_newline(char *buffer)
{
    int len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int find_team_index(server_t *server, const char *team_name)
{
    if (!server || !team_name || !server->teams)
        return ERROR;
    for (int i = 0; i < server->params.teams_count; i++) {
        if (strcmp(server->teams[i].name, team_name) == 0)
            return i;
    }
    return ERROR;
}

/**
    * Finds the AI client by its player ID.
    * The player ID is the index of the AI in the list of clients,
    * excluding graphic clients and clients without a team name.
    *
    * @param server The server instance.
    * @param player_id The ID of the AI player to find.
    * @return The client_t pointer if found, NULL otherwise.
*/
client_t *find_ai_by_id(server_t *server, unsigned int player_id)
{
    unsigned int cmpt = 0;

    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL)
            continue;
        if (server->clients[i]->data.is_graphic)
            continue;
        if (server->clients[i]->data.team_name == NULL)
            continue;
        if (cmpt == player_id) {
            return server->clients[i];
        }
        cmpt++;
    }
    return NULL;
}

char *get_broadcast_text(const char *raw_broadcast)
{
    char *broadcast_text = NULL;
    const char *start = raw_broadcast + WORD_BROADCAST_LENGTH;

    if (!raw_broadcast)
        return NULL;
    start = raw_broadcast + WORD_BROADCAST_LENGTH;
    while (*start == ' ')
        ++start;
    broadcast_text = strdup(start);
    return broadcast_text;
}
