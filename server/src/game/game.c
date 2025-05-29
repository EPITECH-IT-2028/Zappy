/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game.c
*/

#include "macro.h"
#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *game(void *arg)
{
    server_t *server = (server_t *)arg;

    while (server->running) {
        pthread_mutex_lock(&server->threads.data_mutex);
    }
    return NULL;
}

int game_loop(server_t *server) {
    if (pthread_create(&server->threads.game_thread, NULL, game, server) != 0) {
        return ERROR;
    }
    pthread_join(server->threads.game_thread, NULL);
    return SUCCESS;
}
