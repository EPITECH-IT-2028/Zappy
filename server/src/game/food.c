/*
** EPITECH PROJECT, 2025
** server
** File description:
** food.c
*/

#include "server.h"
#include <utils.h>
#include <unistd.h>

/**
 * @brief Check if a client has died and handle death or send status
 *
 * This function checks if a client's food has reached -1 (death) and
 * either sends a death message or updates the client's status.
 *
 * @param server Pointer to the server structure
 * @param i Index of the client in the clients array
 */
static
void check_death(server_t *server, int i)
{
    client_t *client = server->clients[i];

    if (client->data.inventory.food == -1) {
        send_code(client->fd, "dead\n");
        shutdown(client->fd, SHUT_RDWR);
    } else {
        send_pin(server, i);
    }
}

/**
 * @brief Remove one food unit from all connected non-graphic clients
 *
 * This function decreases the food count for all active players and
 * checks for death conditions after food consumption.
 *
 * @param server Pointer to the server structure containing all clients
 */
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
