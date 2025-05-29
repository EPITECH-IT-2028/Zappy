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
