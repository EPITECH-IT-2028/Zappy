/*
** EPITECH PROJECT, 2025
** server
** File description:
** connect_nbr.c
*/

#include "server.h"
#include "utils.h"
#include <stdio.h>

int handle_connect_nbr(server_t *server, response_t *response,
    request_t *request)
{
    int remaining_slots = 0;
    int team_index = 0;

    if (!server || !response || !request)
        return ERROR;
    team_index = find_team_index(server, request->client->data.team_name);
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    sprintf(response->response, "%d", remaining_slots);
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        INVENTORY_TIME);
    return SUCCESS;
}
