/*
** EPITECH PROJECT, 2025
** server
** File description:
** inventory.c
*/

#include "macro.h"
#include "server.h"
#include <bits/time.h>
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
    response->client->data.action_end_time = get_action_end_time(server,
        INVENTORY_TIME);
    return SUCCESS;
}
