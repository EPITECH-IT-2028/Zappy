/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** events_server.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <sys/poll.h>

static
void accept_client(server_t *server, int client_fd)
{
    server->clients[server->nfds] = malloc(sizeof(client_t));
    if (server->clients[server->nfds] == NULL) {
        perror("Error while allocating new clients");
        return;
    }
    init_client_struct(server->clients[server->nfds]);
    server->fds[server->nfds].fd = client_fd;
    server->fds[server->nfds].events = POLLIN;
    server->nfds++;
}

int get_new_connection(server_t *server)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int client_fd = 0;

    if (server->fds[SERVER_INDEX].revents & POLLIN) {
        client_fd = accept(server->fd, (struct sockaddr *)&addr,
            &addr_len);
        if (client_fd == -1)
            return ERROR;
        server->fds = realloc(server->fds,
                sizeof(struct pollfd) * (server->nfds + 1));
        server->clients = realloc(server->clients,
                sizeof(client_t *) * (server->nfds + 1));
        if (server->fds == NULL || server->clients == NULL)
            return ERROR;
        accept_client(server, client_fd);
        send_code(client_fd, "WELCOME");
        printf("New connection\n");
    }
    return 0;
}
