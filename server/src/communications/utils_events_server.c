/*
** EPITECH PROJECT, 2025
** communications
** File description:
** utils_events_server.c
*/

#include "server.h"
#include <stdlib.h>

int define_index(server_t *server)
{
    int idx = 1;

    for (; idx < server->nfds; idx++) {
        if (server->fds[idx].fd == -1)
            break;
    }
    return idx;
}

int resize_fds(server_t *server, int new_size)
{
    struct pollfd *new_fds = realloc(server->fds,
            sizeof(struct pollfd) * new_size);
    client_t **new_clients = realloc(server->clients,
            sizeof(client_t *) * new_size);

    if (new_fds == NULL || new_clients == NULL) {
        perror("realloc failed");
        return ERROR;
    }
    for (int i = server->nfds; i < new_size; i++) {
        new_fds[i].fd = -1;
        new_fds[i].events = 0;
        new_fds[i].revents = 0;
        new_clients[i] = NULL;
    }
    server->fds = new_fds;
    server->clients = new_clients;
    return SUCCESS;
}

void init_fds(server_t *server, int index, int client_fd)
{
    server->fds[index].fd = client_fd;
    server->fds[index].events = POLLIN;
    server->fds[index].revents = 0;
    if (index == server->nfds)
        server->nfds++;
}
