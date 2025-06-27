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
#include <stdlib.h>
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
int prepare_pending_response(response_t *response, response_t *pending)
{
    int count = 0;

    while (response->response[count] != NULL)
        count++;
    cleanup_pending_response(pending);
    pending->client = response->client;
    pending->size = count;
    if (copy_response_data(pending, response, count) == ERROR)
        return ERROR;
    return SUCCESS;
}

static
int add_pending_response_to_client(response_t *response,
    request_t *request)
{
    response_t *pending = NULL;
    int result;

    if (!response || !request || !response->response || !request->client)
        return ERROR;
    pthread_mutex_lock(&request->client->data.pending_mutex);
    if (!request->client->connected) {
        pthread_mutex_unlock(&request->client->data.pending_mutex);
        return ERROR;
    }
    pending = &request->client->data.pending_response;
    result = prepare_pending_response(response, pending);
    pthread_mutex_unlock(&request->client->data.pending_mutex);
    return result;
}

static
void handle_request(server_t *server, response_t *response,
    request_t *request)
{
    response->client = request->client;
    response->size = 0;
    if (is_client_on_cd(&response->client->data) == SUCCESS) {
        pthread_mutex_lock(&server->queue_request.mutex);
        queue_add_request(server, request);
        pthread_mutex_unlock(&server->queue_request.mutex);
        return;
    }
    request->client->data.is_busy = false;
    memset(&request->client->data.action_end_time, 0, sizeof(struct timespec));
    if (check_request(server, response, request) == ERROR) {
        add_buffer_to_response("ko", &response->response, &response->size);
        check_if_queue_is_full(server, response);
        return;
    }
    add_pending_response_to_client(response, request);
}

static
void handle_incantation(server_t *server, client_t *client)
{
    response_t completion_response;
    request_t completion_request;

    if (client->data.incantation.is_incantating) {
        init_response(&completion_response);
        completion_response.client = client;
        completion_request.client = client;
        strcpy(completion_request.request, "Incantation");
        if (handle_ending_incantation(server, &completion_response,
                &completion_request) == ERROR)
            add_buffer_to_response("ko", &completion_response.response,
                &completion_response.size);
        if (completion_response.response && completion_response.size > 0)
            check_if_queue_is_full(server, &completion_response);
    }
}

static
void process_client_response(server_t *server, client_t *client)
{
    if (!client || !client->connected) {
        return;
    }
    pthread_mutex_lock(&client->data.pending_mutex);
    if (client->data.pending_response.response) {
        check_if_queue_is_full(server, &client->data.pending_response);
        client->data.pending_response.response = NULL;
        client->data.pending_response.size = 0;
    }
    pthread_mutex_unlock(&client->data.pending_mutex);
    handle_incantation(server, client);
    client->data.is_busy = false;
    memset(&client->data.action_end_time, 0, sizeof(struct timespec));
}

static
void add_queue_to_response(server_t *server)
{
    if (server->nfds <= 1)
        return;
    pthread_mutex_lock(&server->clients_mutex);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL || server->fds[i].fd == -1)
            continue;
        if (!server->clients[i]->data.is_busy)
            continue;
        if (is_client_on_cd(&server->clients[i]->data) == ERROR)
            process_client_response(server, server->clients[i]);
    }
    pthread_mutex_unlock(&server->clients_mutex);
}

static
void check_time_events(server_t *server)
{
    if (has_time_passed(server, server->server_timer_count,
        FOOD_DURATION, &server->server_timer)) {
        remove_food(server);
        server->server_timer_count =
            get_current_timer_units(server, &server->server_timer);
    }
    if (has_time_passed(server, server->density_timer_count,
        DENSITY_DURATION, &server->density_timer)) {
        respawn_resources(server);
        server->density_timer_count =
            get_current_timer_units(server, &server->density_timer);
    }
}

static
void *game(void *arg)
{
    server_t *server = (server_t *)arg;
    request_t request;
    response_t response = {0};

    clock_gettime(CLOCK_MONOTONIC, &server->server_timer);
    clock_gettime(CLOCK_MONOTONIC, &server->density_timer);
    server->server_timer_count =
        get_current_timer_units(server, &server->server_timer);
    server->density_timer_count =
        get_current_timer_units(server, &server->density_timer);
    while (server->running) {
        if (queue_pop_request(server, &request) == SUCCESS) {
            init_response(&response);
            handle_request(server, &response, &request);
        }
        add_queue_to_response(server);
        check_time_events(server);
        sleep_time(server);
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
