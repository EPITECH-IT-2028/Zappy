/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** events_server.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <strings.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static
int define_index(server_t *server)
{
    int idx = 1;

    for (; idx < server->nfds; idx++) {
        if (server->fds[idx].fd == -1)
            break;
    }
    return idx;
}

static
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
    server->fds = new_fds;
    server->clients = new_clients;
    return SUCCESS;
}

static
void init_fds(server_t *server, int index, int client_fd)
{
    server->fds[index].fd = client_fd;
    server->fds[index].events = POLLIN;
    server->fds[index].revents = 0;
    if (index == server->nfds)
        server->nfds++;
}

static
void accept_client(server_t *server, int client_fd)
{
    int index = define_index(server);

    if (index == server->nfds) {
        if (resize_fds(server, server->nfds + NFDS_REALLOC_NUMBER) == ERROR) {
            close(client_fd);
            return;
        }
        server->clients[index] = NULL;
    }
    server->clients[index] = malloc(sizeof(client_t));
    if (server->clients[index] == NULL) {
        perror("malloc failed");
        close(client_fd);
        return;
    }
    init_client_struct(server->clients[index], client_fd);
    init_fds(server, index, client_fd);
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
        accept_client(server, client_fd);
        send_code(client_fd, "WELCOME");
        printf("New connection\n");
    }
    return 0;
}

static
void remove_player(server_t *server, int index)
{
    pthread_mutex_lock(&server->clients_mutex);
    if (server->clients[index] == NULL) {
        pthread_mutex_unlock(&server->clients_mutex);
        return;
    }
    send_pdi(server, index);
    if (server->fds[index].fd != -1) {
        close(server->fds[index].fd);
        server->fds[index].fd = -1;
    }
    cleanup_client_data(server, index);
    while (server->nfds > 1 && server->fds[server->nfds - 1].fd == -1)
        server->nfds--;
    pthread_mutex_unlock(&server->clients_mutex);
    printf("Client %d disconnected\n", index);
}

static
void handle_client(server_t *server, int index, char *buffer, int bytes)
{
    const char *buffer_end = NULL;

    if (bytes <= 0)
        return remove_player(server, index);
    if (server->clients[index] == NULL)
        return;
    buffer[bytes] = '\0';
    printf("Received from client %d: %s\n", index, buffer);
    buffer_end = strchr(buffer, '\n');
    if (buffer_end == NULL) {
        server->clients[index]->data.request = strdup(buffer);
        return;
    }
    if (server->clients[index]->data.request)
        strcat(server->clients[index]->data.request, buffer);
    remove_newline(buffer);
    if (server->clients[index]->data.team_name == NULL)
        connection_command(server, index, buffer);
    else if (server->clients[index]->data.is_graphic)
        send_code(server->clients[index]->fd, "ko");
    else
        check_player_command(server, index, buffer);
}

void handle_all_client(server_t *server)
{
    char buffer[BUFFER_SIZE];
    int bytes = 0;

    for (int i = 1; i < server->nfds; i++) {
        if (server->fds[i].fd == -1)
            continue;
        if (server->fds[i].revents & POLLIN) {
            bytes = read(server->fds[i].fd, buffer, BUFFER_SIZE - 1);
            handle_client(server, i, buffer, bytes);
        }
    }
}
