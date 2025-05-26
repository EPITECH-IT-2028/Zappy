/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.c
*/

#include "server.h"
#include "macro.h"

static
int server_loop(server_t *server)
{
    for (;;) {
        if (poll(server->fds, server->nfds, 0) < 0) {
            perror("poll failed");
            return ERROR;
        }
        if (get_new_connection(server) == ERROR) {
            perror("get_new_connection failed");
            return ERROR;
        }
        // handle_all_clients(server);
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

int server(__attribute__((unused)) int ac, __attribute__((unused)) char **av)
{
    server_t *server = malloc(sizeof(server_t));
    params_t *params = malloc(sizeof(params_t));

    if (server == NULL) {
        perror("Failed to allocate memory for server");
        return ERROR;
    }
    params->port = 8080;
    if (bind_server(server, params) == ERROR) {
        free(server);
        free(params);
        perror("Failed to bind server");
        return ERROR;
    }
    if (server_loop(server) == ERROR) {
        perror("Server loop failed");
        return ERROR;
    }
    return SUCCESS;
}
