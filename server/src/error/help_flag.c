/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** help_flag.c
*/

#include <stddef.h>
#include <stdio.h>
#include "server.h"
#include "macro.h"

int help_flag(void)
{
    printf(
        "USAGE: ./zappy_server -p port -x width -y "
        "height -n name1 name2 ... -c clientsNb -f freq\n");
    return ERROR;
}
