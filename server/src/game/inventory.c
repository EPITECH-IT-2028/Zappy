/*
** EPITECH PROJECT, 2025
** server
** File description:
** inventory.c
*/

#include "macro.h"
#include "server.h"
#include <stdio.h>

/**
 * @brief Handle the inventory command to display player's resources
 *
 * This function creates a formatted string containing all the resources
 * in the player's inventory and adds it to the response.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @return SUCCESS on successful inventory display, ERROR on failure
 */
int handle_inventory(server_t *server, response_t *response,
    request_t *request)
{
    char buffer[BUFFER_SIZE] = {0};

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
    return SUCCESS;
}
