/*
** EPITECH PROJECT, 2025
** server
** File description:
** food.c
*/

#include "server.h"
#include "utils.h"
#include <unistd.h>

static
void check_death(client_t *client)
{
    if (client->data.inventory.food == -1) {
        // TO DO: handle death of a client
        // TEMPORARY CODE: send dead to client
        send_code(client->fd, "dead");
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
            check_death(server->clients[i]);
        }
    }
}
