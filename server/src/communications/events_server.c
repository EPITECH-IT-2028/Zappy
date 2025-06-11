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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static
void accept_client(server_t *server, int client_fd)
{
    server->clients[server->nfds] = malloc(sizeof(client_t));
    if (server->clients[server->nfds] == NULL) {
        perror("Error while allocating new clients");
        return;
    }
    init_client_struct(server->clients[server->nfds], client_fd);
    server->fds[server->nfds].fd = client_fd;
    server->fds[server->nfds].events = POLLIN;
    server->fds[server->nfds].revents = 0;
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

static
void remove_player(server_t *server, int index)
{
    int team_index = find_team_index(server,
        server->clients[index]->data.team_name);

    if (!server->clients[index]->data.is_graphic &&
        team_index != ERROR && server->teams[team_index].clients_count > 0) {
        server->teams[team_index].clients_count--;
    }
    close(server->fds[index].fd);
    free(server->clients[index]);
    server->clients[index] = NULL;
    server->fds[index].fd = -1;
    printf("Client %d disconnected\n", index);
}

static
void handle_client(server_t *server, int index, char *buffer, int bytes)
{
    const char *buffer_end = NULL;

    if (bytes <= 0) {
        remove_player(server, index);
        return;
    }
    buffer[bytes] = '\0';
    printf("Received from client %d: %s\n", index, buffer);
    buffer_end = strchr(buffer, '\n');
    if (buffer_end == NULL) {
        send_code(server->clients[index]->fd, "ko");
        return;
    }
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
