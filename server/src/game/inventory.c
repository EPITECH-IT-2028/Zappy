/*
** EPITECH PROJECT, 2025
** server
** File description:
** inventory.c
*/

#include "server.h"
#include <stdio.h>

int handle_inventory(server_t *server, response_t *response,
    request_t *request)
{
    if (!request || !response || !server)
        return ERROR;
    sprintf(response->response,
        "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras "
        "%d, thystame %d]",
        response->client->data.inventory.food,
        response->client->data.inventory.linemate,
        response->client->data.inventory.deraumere,
        response->client->data.inventory.sibur,
        response->client->data.inventory.mendiane,
        response->client->data.inventory.phiras,
        response->client->data.inventory.thystame);
    response->client->data.is_busy = true;
    response->client->data.action_end_time = server->timer_count +
        (INVENTORY_TIME * server->params.frequence);
    return SUCCESS;
}
