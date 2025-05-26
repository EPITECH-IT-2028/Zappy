/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_teams_names.c
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

int check_teams_names(server_t *server, char **av, size_t *av_idx)
{
    for (; av[*av_idx] != NULL; *av_idx += 1) {
        if (av[*av_idx][0] == '-')
            break;
        if (!server->teams_names) {
            server->teams_names =
                malloc(sizeof(char *) * (server->teams_count + 2));
        }
        if (server->teams_names) {
            server->teams_names = realloc(
                server->teams_names,
                sizeof(char *) * (server->teams_count + 2));
        }
        if (server->teams_names == NULL)
            return ERROR;
        server->teams_names[server->teams_count] = strdup(av[*av_idx]);
        server->teams_names[server->teams_count + 1] = NULL;
        server->teams_count += 1;
    }
    return SUCCESS;
}
