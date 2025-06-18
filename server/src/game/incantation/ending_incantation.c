/*
** EPITECH PROJECT, 2025
** server
** File description:
** end_incantation.c
*/

#include "server.h"

static
void level_up_all_client(client_t *incantator, response_t *response)
{
    for (int i = 0; incantator->data.incantation.client_group[i] != NULL;
        i++) {
        incantator->data.incantation.client_group[i]->data.level++;
    }
    sprintf(response->response, "Current level: %d", incantator->data.level);
}

int handle_ending_incantation(server_t *server, response_t *response,
    request_t *request)
{
    if (check_if_incantation_failed(&request->client->data,
        server->clients, &server->map[response->client->data.x]
        [response->client->data.y]) == ERROR)
        return ERROR;
    level_up_all_client(response->client, response);
    // send_pie_all(server, response->client->data.incantation.client_group);
    return SUCCESS;
}
