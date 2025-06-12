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
    size_t len = 0;

    for (int i = 0; tab_ai_commands[i].request != NULL; i++) {
        len = strlen(tab_ai_commands[i].request);
        if (strncmp(tab_ai_commands[i].request, request->request, len) == 0 &&
            (request->request[len] == '\0' || request->request[len] == ' ')) {
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
void check_if_queue_is_full(server_t *server, response_t *response)
{
    if (queue_add_response(server, response) == ERROR) {
        fprintf(stderr, "Error: Queue was full response will not be sent.\n");
    }
}

static
void handle_request(server_t *server, response_t *response, request_t *request)
{
    response->client = request->client;
    if (is_client_on_cd(&response->client->data) == SUCCESS) {
        queue_add_request(server, request);
        return;
    }
    request->client->data.is_busy = false;
    memset(&request->client->data.action_end_time, 0, sizeof(struct timespec));
    if (check_request(server, response, request) == ERROR) {
        sprintf(response->response, "ko");
        check_if_queue_is_full(server, response);
        return;
    }
    memcpy(&request->client->data.pending_response, response,
        sizeof(response_t));
}

static
void add_queue_to_response(server_t *server)
{
    if (server->nfds <= 1)
        return;
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL || server->fds[i].fd == -1)
            continue;
        if (!server->clients[i]->data.is_busy)
            continue;
        if (is_client_on_cd(&server->clients[i]->data) == ERROR) {
            check_if_queue_is_full(server,
                &server->clients[i]->data.pending_response);
            server->clients[i]->data.is_busy = false;
        }
    }
}

static
void *game(void *arg)
{
    server_t *server = (server_t *)arg;
    request_t request;
    response_t response;

    clock_gettime(CLOCK_MONOTONIC, &server->server_timer);
    server->server_timer_count = get_current_timer_units(server);
    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            handle_request(server, &response, &request);
        }
        add_queue_to_response(server);
        if (has_time_passed(server, server->server_timer_count,
            FOOD_DURATION)) {
            remove_food(server);
            server->server_timer_count = get_current_timer_units(server);
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
