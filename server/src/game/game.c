/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game.c
*/

#include "ai_commands.h"
#include "macro.h"
#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static
int check_request(response_t *response, request_t *request)
{
    int result = 0;

    for (int i = 0; tab_ai_commands[i].request != NULL; i++) {
        if (strcmp(tab_ai_commands[i].request, request->request) == 0) {
            result = tab_ai_commands[i].func(response, request);
            break;
        }
    }
    return result;
}

static
void handle_request(server_t *server, response_t *response, request_t *request)
{
    response->client = request->client;
    check_request(response, request);
    if (queue_add_response(server, response) == ERROR) {
        fprintf(stderr, "Error: Queue was full request will"
        " not be sent.\n"
        );
    }
}

void *game(void *arg)
{
    server_t *server = (server_t *)arg;
    request_t request;
    response_t response;

    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            handle_request(server, &response, &request);
        }
    }
    return NULL;
}

int game_loop(server_t *server)
{
    if (pthread_create(&server->threads.game_thread, NULL, game, server)) {
        return ERROR;
    }
    return SUCCESS;
}
