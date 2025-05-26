/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_clients_nb.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"

int check_clients_nb(server_t *server, char **av, size_t *av_idx)
{
    int client_nb = atoi(av[*av_idx]);

    if (client_nb == 0) {
        return ERROR;
    }
    if (client_nb < MIN_CLIENT) {
        return ERROR;
    }
    *av_idx += 1;
    server->client_per_team = client_nb;
    return SUCCESS;
}
