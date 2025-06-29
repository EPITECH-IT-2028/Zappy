/*
** EPITECH PROJECT, 2025
** communications
** File description:
** utils_events_server.c
*/

#include "server.h"
#include <stdlib.h>

/**
 * @brief Find the next available index in the server's client array
 *
 * Searches for an unused slot in the file descriptor array starting
 * from index 1 (skipping the server socket at index 0).
 *
 * @param server Pointer to the server structure
 * @return Index of the next available slot
 */
int define_index(server_t *server)
{
    int idx = 1;

    for (; idx < server->nfds; idx++) {
        if (server->fds[idx].fd == -1)
            break;
    }
    return idx;
}

/**
 * @brief Resize the server's file descriptor and client arrays
 *
 * Reallocates memory for both the pollfd array and client pointer
 * array to accommodate more connections. Initializes new slots.
 *
 * @param server Pointer to the server structure
 * @param new_size New size for the arrays
 * @return SUCCESS on successful resize, ERROR on allocation failure
 */
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

/**
 * @brief Initialize a file descriptor entry for a new client
 *
 * Sets up the pollfd structure for a new client connection and
 * updates the server's file descriptor count if necessary.
 *
 * @param server Pointer to the server structure
 * @param index Index where to place the new client
 * @param client_fd File descriptor of the new client connection
 */
void init_fds(server_t *server, int index, int client_fd)
{
    server->fds[index].fd = client_fd;
    server->fds[index].events = POLLIN;
    server->fds[index].revents = 0;
    if (index == server->nfds)
        server->nfds++;
}
