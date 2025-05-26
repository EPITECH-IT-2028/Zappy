/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** free_server.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>

void free_server(params_t *params)
{
    if (params->teams_names) {
        for (int i = 0; i < params->teams_count; i += 1) {
            free(params->teams_names[i]);
        }
        free(params->teams_names);
    }
}
