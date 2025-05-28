/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game.c
*/

#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *game(void *arg)
{
    server_t *server = (server_t *)arg;

    printf("Here is the port used: %d\n", server->params.port);
    // while (server->running) {
    //     printf("Hello World\n");
    //     sleep(1);
    // }
    return NULL;
}
