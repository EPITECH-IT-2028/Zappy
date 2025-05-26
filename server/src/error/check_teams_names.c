/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_teams_names.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "macro.h"
#include "server.h"

int check_teams_names(params_t *params, char **av, size_t *av_idx)
{
    for (; av[*av_idx] != NULL; *av_idx += 1) {
        if (av[*av_idx][0] == '-') {
            break;
        }
        params->teams_names = realloc(
            params->teams_names, sizeof(char *) * (params->teams_count + 1));
        if (params->teams_names == NULL) {
            return ERROR;
        }
        params->teams_names[params->teams_count] = av[*av_idx];
        params->teams_count += 1;
    }
    return SUCCESS;
}
