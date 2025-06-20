/*
** EPITECH PROJECT, 2025
** server
** File description:
** food.c
*/

#include "server.h"
#include <unistd.h>

static
void check_death(server_t *server, int i)
{
    client_t *client = server->clients[i];

    if (client->data.inventory.food == -1) {
        shutdown(client->fd, SHUT_RDWR);
    } else {
        send_pin(server, i);
    }
}

void remove_food(server_t *server)
{
    if (server->nfds == 1)
        return;
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL || server->fds[i].fd == -1
        || server->clients[i]->data.is_graphic)
            continue;
        if (server->clients[i]->data.team_name != NULL) {
            server->clients[i]->data.inventory.food--;
            check_death(server, i);
        }
    }
}
