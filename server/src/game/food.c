/*
** EPITECH PROJECT, 2025
** server
** File description:
** food.c
*/

#include "server.h"
#include "utils.h"
#include <unistd.h>

void remove_food(server_t *server)
{
    if (server->nfds == 1)
        return;
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL || server->fds[i].fd == -1)
            continue;
        if (server->clients[i]->connected) {
            server->clients[i]->data.inventory.food--;
            if (server->clients[i]->data.inventory.food == -1) {
                // TO DO: handle death of a client
                // TEMPORARY CODE: send dead to client
                send_code(server->clients[i]->fd, "dead");
            }
        }
    }
    server->server_timer_count = get_current_timer_units(server);
}
