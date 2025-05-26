/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_freq.c
*/

#include <stdlib.h>
#include "server.h"

int check_freq(server_t *server, char **av, size_t *av_idx)
{
    int freq = atoi(av[*av_idx]);

    if (freq <= 0) {
        return ERROR;
    }
    *av_idx += 1;
    server->frequence = freq;
    return SUCCESS;
}
