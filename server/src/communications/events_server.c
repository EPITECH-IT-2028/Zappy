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
        send_code(client_fd, "WELCOME\n");
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
void append_to_client_buffer(client_t *client, char *buffer, int bytes)
{
    char *client_buffer = client->buffer;
    char *new_buffer = NULL;

    if (client_buffer == NULL) {
        client->buffer = malloc(bytes + 1);
        if (client->buffer == NULL)
            return;
        memcpy(client->buffer, buffer, bytes);
        client->buffer[bytes] = '\0';
        return;
    }
    new_buffer = realloc(client_buffer, sizeof(char)
        * (strlen(client_buffer) + bytes + 1));
    if (new_buffer == NULL)
        return;
    client->buffer = new_buffer;
    strcat(client->buffer, buffer);
}

static
void process_client_command(server_t *server, int index)
{
    client_t *client = server->clients[index];

    remove_newline(client->buffer);
    if (client->data.team_name == NULL)
        connection_command(server, index, client->buffer);
    else if (client->data.is_graphic)
        send_code(client->fd, "ko\n");
    else
        check_player_command(server, index, client->buffer);
}

static
void handle_client(server_t *server, int index, char *buffer, int bytes)
{
    if (!server || !buffer || !server->clients || !server->clients[index])
        return;
    if (bytes <= 0)
        return remove_player(server, index);
    buffer[bytes] = '\0';
    printf("Received from client %d: %s\n", index, buffer);
    append_to_client_buffer(server->clients[index], buffer, bytes);
    if (strchr(buffer, '\n') != NULL)
        process_client_command(server, index);
    free(server->clients[index]->buffer);
    server->clients[index]->buffer = NULL;
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
