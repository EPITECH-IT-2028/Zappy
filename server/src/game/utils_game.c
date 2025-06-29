/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_game.c
*/

#include "server.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Sleep for a short duration if the request queue is empty
 *
 * This function checks if the server's request queue is empty and
 * makes the server sleep for 10 microseconds to avoid busy waiting.
 *
 * @param server Pointer to the server structure
 */
void sleep_time(server_t *server)
{
    struct timespec sleep_time = {0, 10000};

    if (server->queue_request.len == 0) {
        nanosleep(&sleep_time, NULL);
    }
}

/**
 * @brief Check if a client is currently on cooldown
 *
 * This function verifies if a client is still on cooldown by comparing
 * the current time with the client's action end time.
 *
 * @param client_data Pointer to the client data structure
 * @return SUCCESS if client is on cooldown, ERROR otherwise
 */
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

/**
 * @brief Add a response to the queue and check for errors
 *
 * This function attempts to add a response to the server's response
 * queue and prints an error message if the queue is full.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response to be added
 */
void check_if_queue_is_full(server_t *server, response_t *response)
{
    if (queue_add_response(server, response) == ERROR)
        fprintf(stderr,
        "[Error]: Queue was full response will not be sent.\n");
}

/**
 * @brief Clean up and free a pending response structure
 *
 * This function safely cleans up a pending response by freeing all
 * allocated memory and resetting the structure fields to safe values.
 *
 * @param pending Pointer to the response structure to clean up
 * @return SUCCESS on successful cleanup, ERROR on failure
 */
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

/**
 * @brief Copy response data from source to destination structure
 *
 * This function creates a deep copy of response data from the source
 * response structure to the destination, allocating memory as needed.
 *
 * @param dest Pointer to the destination response structure
 * @param src Pointer to the source response structure
 * @param count Number of responses to copy
 * @return SUCCESS on successful copy, ERROR on failure
 */
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
