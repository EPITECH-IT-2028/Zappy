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
    for (int i = 1; server->clients[i] != NULL; i++) {
        server->clients[i]->data.inventory.food--;
        if (server->clients[i]->data.inventory.food == -1) {
            // TO DO: handle death of a client
            // TEMPORARY CODE: send dead to client
            send_code(server->clients[i]->fd, "dead");
        }
    }
}
