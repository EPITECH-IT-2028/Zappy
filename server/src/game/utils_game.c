/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_game.c
*/

#include "server.h"
#include <stdlib.h>
#include <string.h>

void sleep_time(server_t *server)
{
    struct timespec sleep_time = {0, 1000000};

    if (server->queue_request.len == 0) {
        nanosleep(&sleep_time, NULL);
    }
}

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

void check_if_queue_is_full(server_t *server, response_t *response)
{
    if (queue_add_response(server, response) == ERROR) {
        fprintf(stderr, "Error: Queue was full response will not be sent.\n");
    }
}

int cleanup_pending_response(response_t *pending)
{
    if (!pending->response || pending->size < 0 ||
        pending->size > MAX_REQUEST_PER_CLIENT) {
        free(pending->response);
        pending->response = NULL;
        pending->size = 0;
        return SUCCESS;
    }
    for (int i = 0; i < pending->size; i++) {
        if (pending->response[i]) {
            free(pending->response[i]);
            pending->response[i] = NULL;
        }
    }
    free(pending->response);
    pending->response = NULL;
    pending->size = 0;
    return SUCCESS;
}

int copy_response_data(response_t *dest, response_t *src, int count)
{
    dest->response = malloc(sizeof(char *) * (count + 1));
    if (!dest->response)
        return ERROR;
    for (int i = 0; i < count; i++) {
        dest->response[i] = strdup(src->response[i]);
        if (dest->response[i])
            continue;
        for (int j = 0; j < i; j++)
            free(dest->response[j]);
        free(dest->response);
        dest->response = NULL;
        dest->size = 0;
        return ERROR;
    }
    dest->response[count] = NULL;
    return SUCCESS;
}
