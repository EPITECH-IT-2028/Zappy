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
    request_t request;
    response_t response;

    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            response.client_fd = request.client_fd;
            // NEED TO ADD PROCESS TO HANDLE ALL CLIENTS COMMANDS
            // BY USING SERVER, RESPONSE AND REQUEST
            if (queue_add_response(server, &response) == ERROR) {
                fprintf(stderr, "Error: Queue was full request will"
                " not be sent.\n"
                );
            }
        }
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
