/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.c
*/

#include "server.h"
#include "macro.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

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

    if (server == NULL) {
        perror("Failed to allocate memory for server or params");
        return ERROR;
    }
    init_params(&server->params);
    if (check_params(&server->params, ac, av) == ERROR)
        return ERROR;
    if (bind_server(server, &server->params) == ERROR) {
        perror("Failed to bind server");
        return ERROR;
    }
    if (game_loop(server) == ERROR)
        return ERROR;
    if (server_loop(server) == ERROR) {
        perror("Server loop failed");
        return ERROR;
    }
    return SUCCESS;
}
