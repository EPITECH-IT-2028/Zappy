/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** free_server.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>

void free_server(server_t *server)
{
    if (server->teams_names) {
        for (int i = 0; i < server->teams_count; i += 1) {
            free(server->teams_names[i]);
        }
        free(server->teams_names);
    }
}
