/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** help_flag.c
*/

#include <stddef.h>
#include <stdio.h>
#include "server.h"

int help_flag(__attribute__((unused)) server_t *server,
    __attribute__((unused)) char **av,
    __attribute__((unused)) size_t *av_idx)
{
    printf(
        "USAGE: ./zappy_server -p port -x width -y "
        "height -n name1 name2 ... -c clientsNb -f freq\n");
    return ERROR;
}
