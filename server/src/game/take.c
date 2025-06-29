/*
** EPITECH PROJECT, 2025
** server
** File description:
** take.c
*/

#include "macro.h"
#include "server.h"

/**
 * @brief Handles the "Take" command for taking resources from the map
 *
 * This function processes a client's request to take a resource from
 * the map into their inventory. It validates the operation,
 * updates the game state, and sends appropriate notifications.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to response structure to fill with result
 * @param request Pointer to the client's set request
 * @return SUCCESS if resource was taken successfully, ERROR otherwise
 */
int handle_take(server_t *server, response_t *response, request_t *request)
{
    client_data_t *client = &request->client->data;
    int id = -1;

    if (!server || !response || !request)
        return ERROR;
    id = check_ressource_update(server, request, client, false);
    if (id == ERROR)
        return ERROR;
    send_pgt(server, request->client, id);
    send_bct(server, client->x, client->y);
    add_buffer_to_response("ok", &response->response, &response->size);
    return SUCCESS;
}
