/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** queue.c
*/

#include "macro.h"
#include "server.h"
#include <pthread.h>

/*
** @brief Adds the next request from the server's queue.
**
** Safely appends request to queue.
**
** @param server Pointer to the server structure.
** @param request Pointer of the request.
**
** @return SUCCESS (0) if successful, ERROR (84) if the queue is full.
*/
int queue_add_request(server_t *server, request_t *request)
{
    pthread_mutex_lock(&server->queue_request.mutex);
    if (server->queue_request.len == QUEUE_MAX_SIZE) {
      pthread_mutex_unlock(&server->queue_request.mutex);
      return ERROR;
    }
    server->queue_request.queue[server->queue_request.tail] = *request;
    server->queue_request.tail =
        (server->queue_request.tail + 1) % QUEUE_MAX_SIZE;
    server->queue_request.len += 1;
    pthread_mutex_unlock(&server->queue_request.mutex);
    return SUCCESS;
}

/*
** @brief Pops the next request from the server's queue.
**
** Safely retrieves and removes the head request from the queue.
**
** @param server Pointer to the server structure.
** @param request Pointer to store the popped request.
**
** @return SUCCESS (0) if successful, ERROR (84) if the queue is empty.
*/
int queue_pop_request(server_t *server, request_t *request)
{
    pthread_mutex_lock(&server->queue_request.mutex);
    if (server->queue_request.len == 0) {
      pthread_mutex_unlock(&server->queue_request.mutex);
      return ERROR;
    }
    *request = server->queue_request.queue[server->queue_request.head];
    server->queue_request.head =
        (server->queue_request.head + 1) % QUEUE_MAX_SIZE;
    server->queue_request.len -= 1;
    pthread_mutex_unlock(&server->queue_request.mutex);
    return SUCCESS;
}

/*
** @brief Adds a response to the server's response queue.
**
** This function locks the response queue mutex, checks if the queue is full,
** and if not, adds the given response to the queue. It updates the queue's
** tail and length accordingly. The mutex is unlocked before returning.
**
** @param server Pointer to the server structure containing the response queue.
** @param response Pointer to the response to be added to the queue.
**
** @return SUCCESS if the response is successfully added, ERROR if the queue is
** full.
*/
int queue_add_response(server_t *server, response_t *response)
{
    pthread_mutex_lock(&server->queue_response.mutex);
    if (server->queue_response.len == QUEUE_MAX_SIZE) {
      pthread_mutex_unlock(&server->queue_response.mutex);
      return ERROR;
    }
    server->queue_response.queue[server->queue_response.tail] = *response;
    server->queue_response.tail =
        (server->queue_response.tail + 1) % QUEUE_MAX_SIZE;
    server->queue_response.len += 1;
    pthread_mutex_unlock(&server->queue_response.mutex);
    return SUCCESS;
}

/*
** @brief Removes and retrieves the next response from the server's response
** queue.
**
** This function safely pops a response from the queue by locking the queue's
** mutex, checking if the queue is empty, and updating the queue's head and
** length accordingly.
**
** @param server Pointer to the server structure containing the response queue.
** @param response Pointer to the response structure where the popped response
** will be stored.
**
** @return SUCCESS if a response was successfully popped, ERROR if the queue is
** empty.
*/
int queue_pop_response(server_t *server, response_t *response)
{
    pthread_mutex_lock(&server->queue_response.mutex);
    if (server->queue_response.len == 0) {
      pthread_mutex_unlock(&server->queue_response.mutex);
      return ERROR;
    }
    *response = server->queue_response.queue[server->queue_response.head];
    server->queue_response.head =
        (server->queue_response.head + 1) % QUEUE_MAX_SIZE;
    server->queue_response.len -= 1;
    pthread_mutex_unlock(&server->queue_response.mutex);
    return SUCCESS;
}
