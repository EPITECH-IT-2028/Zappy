/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.c
*/

#include "server.h"
#include "macro.h"
#include "utils.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**
 * @brief Free memory allocated for a response structure
 *
 * This function safely frees all memory allocated for response
 * strings and the response array itself.
 *
 * @param response Pointer to the response structure to free
 */
static
void free_response(response_t *response)
{
    if (response->response) {
        for (int i = 0; response->response[i] != NULL; i++) {
            free(response->response[i]);
        }
        free(response->response);
    }
    return;
}

/**
 * @brief Handle and send responses from the response queue
 *
 * This function processes responses from the queue and sends them
 * to the appropriate connected clients.
 *
 * @param server Pointer to the server structure
 */
static
void handle_request(server_t *server)
{
    response_t response = {0};
    int actual_count = 0;

    if (queue_pop_response(server, &response) == ERROR)
        return;
    if (!response.client || !response.client->connected)
        return free_response(&response);
    if (!response.response)
        return;
    for (int i = 0; response.response[i] != NULL; i++)
        actual_count++;
    for (int i = 0; i < actual_count; i++) {
        send_code(response.client->fd, response.response[i]);
        free(response.response[i]);
        response.response[i] = NULL;
    }
    send_code(response.client->fd, "\n");
    free(response.response);
    response.response = NULL;
}

/**
 * @brief Main server event loop
 *
 * This function runs the main server loop, handling client connections,
 * processing requests, and managing responses until shutdown.
 *
 * @param server Pointer to the server structure
 * @return SUCCESS on clean shutdown, ERROR on failure
 */
static
int server_loop(server_t *server)
{
    while (server->running) {
        if (poll(server->fds, server->nfds, 10) < 0) {
            perror("poll failed");
            return ERROR;
        }
        if (get_new_connection(server) == ERROR) {
            perror("get_new_connection failed");
            return ERROR;
        }
        handle_all_client(server);
        handle_request(server);
    }
    return SUCCESS;
}

/**
 * @brief Initialize and bind the server socket
 *
 * This function initializes the server structure, creates the socket,
 * sets socket options, binds to the address, and starts listening.
 *
 * @param server Pointer to the server structure to initialize
 * @param params Pointer to the server parameters
 * @return SUCCESS on successful binding, ERROR on failure
 */
static
int bind_server(server_t *server, params_t *params)
{
    if (init_server_struct(server, params) == ERROR) {
        perror("init_server_struct failed");
        return ERROR;
    }
    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
            sizeof(int)) == -1) {
        perror("setsockopt failed");
        return ERROR;
    }
    if (bind(server->fd, (struct sockaddr *)&server->addr,
            sizeof(server->addr)) == -1) {
        perror("bind failed");
        return ERROR;
    }
    if (listen(server->fd, SOMAXCONN) == -1) {
        perror("listen failed");
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Main server entry point
 *
 * This function orchestrates the complete server startup process,
 * parameter validation, server initialization, and main loop execution.
 *
 * @param ac Argument count
 * @param av Array of command line arguments
 * @return SUCCESS on successful execution, ERROR on failure
 */
int server(int ac, char **av)
{
    server_t *server = malloc(sizeof(server_t));

    srand(time(NULL));
    if (server == NULL)
        return ERROR;
    if (check_params(&server->params, ac, av) == ERROR) {
        free_params(&server->params);
        free(server);
        return ERROR;
    }
    if (bind_server(server, &server->params) == ERROR) {
        free_server(server);
        return ERROR;
    }
    if (game_loop(server) == ERROR || server_loop(server) == ERROR) {
        free_server(server);
        return ERROR;
    }
    free_server(server);
    return SUCCESS;
}
