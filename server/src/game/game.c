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
int check_request(server_t *server, response_t *response, request_t *request)
{
    int result = 0;

    for (int i = 0; tab_ai_commands[i].request != NULL; i++) {
        if (strcmp(tab_ai_commands[i].request, request->request) == 0) {
            result = tab_ai_commands[i].func(server, response, request);
            break;
        }
    }
    return result;
}

static
void handle_request(server_t *server, response_t *response, request_t *request)
{
    int current_timer = 0;

    pthread_mutex_lock(&server->threads.timer_mutex);
    current_timer = server->timer_count;
    pthread_mutex_unlock(&server->threads.timer_mutex);
    response->client = request->client;
    if (request->client->data.action_end_time >= current_timer) {
        return;
    }
    request->client->data.is_busy = false;
    request->client->data.action_end_time = 0;
    if (check_request(server, response, request) == ERROR) {
        sprintf(response->response, "ko");
    }
    if (queue_add_response(server, response) == ERROR) {
        fprintf(stderr, "Error: Queue was full request will"
        " not be sent.\n"
        );
    }
}

static
void *game(void *arg)
{
    server_t *server = (server_t *)arg;
    request_t request;
    response_t response;
    int last_food_removal = 0;
    int timer = 0;

    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            handle_request(server, &response, &request);
        }
        pthread_mutex_lock(&server->threads.timer_mutex);
        timer = server->timer_count;
        pthread_mutex_unlock(&server->threads.timer_mutex);
        if (timer / FOOD_DURATION > last_food_removal) {
            remove_food(server);
            last_food_removal = timer / FOOD_DURATION;
        }
    }
    return NULL;
}

static
void *timer(void *arg)
{
    server_t *server = (server_t *)arg;

    while (server->running) {
        usleep(1000000 / server->params.frequence);
        pthread_mutex_lock(&server->threads.timer_mutex);
        server->timer_count++;
        pthread_mutex_unlock(&server->threads.timer_mutex);
    }
    return NULL;
}

int game_loop(server_t *server)
{
    if (pthread_create(&server->threads.game_thread, NULL, game, server)) {
        return ERROR;
    }
    if (pthread_create(&server->threads.timer_thread, NULL, timer, server)) {
        return ERROR;
    }
    return SUCCESS;
}
