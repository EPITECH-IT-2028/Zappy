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
        if (strncmp(tab_ai_commands[i].request, request->request,
            strlen(tab_ai_commands[i].request)) == 0) {
            result = tab_ai_commands[i].func(server, response, request);
            break;
        }
    }
    return result;
}

static
int is_client_on_cd(client_data_t *client_data)
{
    struct timespec current_time;

    clock_gettime(CLOCK_MONOTONIC, &current_time);
    if (client_data->action_end_time.tv_sec > current_time.tv_sec ||
        (client_data->action_end_time.tv_sec == current_time.tv_sec &&
         client_data->action_end_time.tv_nsec > current_time.tv_nsec)) {
        return SUCCESS;
    }
    return ERROR;
}

static
void handle_request(server_t *server, response_t *response, request_t *request)
{
    response->client = request->client;
    if (is_client_on_cd(&response->client->data) == SUCCESS) {
        return;
    }
    request->client->data.is_busy = false;
    memset(&request->client->data.action_end_time, 0,
        sizeof(request->client->data.action_end_time));
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

    server->server_timer_count = get_current_timer_units(server);
    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            handle_request(server, &response, &request);
        }
        if (has_time_passed(server, server->server_timer_count,
            FOOD_DURATION)) {
            remove_food(server);
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
