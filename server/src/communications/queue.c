/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** queue.c
*/

#include "macro.h"
#include "server.h"
#include <pthread.h>

int queue_add_request(server_t *server, request_t *request)
{
    pthread_mutex_lock(&server->queue_request.mutex);
    if (server->queue_request.len == QUEUE_MAX_SIZE) {
        pthread_mutex_unlock(&server->queue_request.mutex);
        return ERROR;
    }
    server->queue_request.queue[server->queue_request.len] = *request;
    server->queue_request.tail = (server->queue_request.tail + 1) % QUEUE_MAX_SIZE;
    server->queue_request.len += 1;
    pthread_mutex_unlock(&server->queue_request.mutex);
    return SUCCESS;
}

int queue_pop_request(server_t *server, request_t *request)
{
    pthread_mutex_lock(&server->queue_request.mutex);
    if (server->queue_request.len == 0) {
        pthread_mutex_unlock(&server->queue_request.mutex);
        return ERROR;
    }
    *request = server->queue_request.queue[server->queue_request.head];
    server->queue_request.head = (server->queue_request.head + 1) % QUEUE_MAX_SIZE;
    server->queue_request.len -= 1;
    pthread_mutex_unlock(&server->queue_request.mutex);
    return SUCCESS;
}

int queue_add_response(server_t *server, response_t *response)
{
    pthread_mutex_lock(&server->queue_response.mutex);
    if (server->queue_response.len == QUEUE_MAX_SIZE) {
        pthread_mutex_unlock(&server->queue_response.mutex);
        return ERROR;
    }
    server->queue_response.queue[server->queue_response.len] = *response;
    server->queue_response.tail = (server->queue_response.tail + 1) % QUEUE_MAX_SIZE;
    server->queue_response.len += 1;
    pthread_mutex_unlock(&server->queue_response.mutex);
    return SUCCESS;
}

int queue_pop_response(server_t *server, response_t *response)
{
    pthread_mutex_lock(&server->queue_response.mutex);
    if (server->queue_response.len == 0) {
        pthread_mutex_unlock(&server->queue_response.mutex);
        return ERROR;
    }
    *response = server->queue_response.queue[server->queue_response.head];
    server->queue_response.head = (server->queue_response.head + 1) % QUEUE_MAX_SIZE;
    server->queue_response.len -= 1;
    pthread_mutex_unlock(&server->queue_response.mutex);
    return SUCCESS;
}