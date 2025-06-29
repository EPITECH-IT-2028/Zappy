/*
** EPITECH PROJECT, 2025
** server
** File description:
** connect_nbr.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdio.h>

/**
 * @brief Handle the connect_nbr command to get remaining team slots
 *
 * This function calculates and returns the number of remaining
 * connection slots available for the client's team.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @return SUCCESS on success, ERROR on failure
 */
int handle_connect_nbr(server_t *server, response_t *response,
    request_t *request)
{
    int remaining_slots = 0;
    int team_index = 0;
    char buffer[BUFFER_SIZE] = {0};

    if (!server || !response || !request)
        return ERROR;
    team_index = find_team_index(server, request->client->data.team_name);
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    sprintf(buffer, "%d", remaining_slots);
    if (add_buffer_to_response(buffer, &response->response, &response->size)
        == ERROR)
        return ERROR;
    return SUCCESS;
}
