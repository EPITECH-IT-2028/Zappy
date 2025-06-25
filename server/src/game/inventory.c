/*
** EPITECH PROJECT, 2025
** server
** File description:
** inventory.c
*/

#include "macro.h"
#include "server.h"
#include <stdio.h>

int handle_inventory(server_t *server, response_t *response,
    request_t *request)
{
    char buffer[BUFFER_SIZE];
    if (!request || !response || !server)
        return ERROR;
    sprintf(buffer,
        "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras "
        "%d, thystame %d]",
        response->client->data.inventory.food,
        response->client->data.inventory.linemate,
        response->client->data.inventory.deraumere,
        response->client->data.inventory.sibur,
        response->client->data.inventory.mendiane,
        response->client->data.inventory.phiras,
        response->client->data.inventory.thystame);
    add_buffer_to_response(buffer, &response->response, &response->size);
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        INVENTORY_TIME);
    return SUCCESS;
}
