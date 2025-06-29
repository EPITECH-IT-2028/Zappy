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

/**
 * @brief Display usage information for the Zappy server
 *
 * This function prints the correct usage syntax for the server
 * including all required parameters and their descriptions.
 *
 * @return ERROR to indicate program should exit after showing help
 */
int help_flag(void)
{
    printf(
        "USAGE: ./zappy_server -p port -x width -y "
        "height -n name1 name2 ... -c clientsNb -f freq\n");
    return ERROR;
}
