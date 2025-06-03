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

static
void handle_request(server_t *server)
{
    response_t response;

    if (queue_pop_response(server, &response) == SUCCESS) {
        send_code(response.client->fd, response.response);
    }
}

static
void handle_request(server_t *server)
{
    response_t response;

    if (queue_pop_response(server, &response) == SUCCESS) {
        send_code(response.client->fd, response.response);
    }
}

static
int server_loop(server_t *server)
{
    while (server->running) {
        if (poll(server->fds, server->nfds, 0) < 0) {
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

int server(int ac, char **av)
{
    server_t *server = malloc(sizeof(server_t));

    srand(time(NULL));
    if (server == NULL)
        return ERROR;
    if (check_params(&server->params, ac, av) == ERROR) {
        free_server(server);
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
