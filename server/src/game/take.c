/*
** EPITECH PROJECT, 2025
** server
** File description:
** take.c
*/

#include "macro.h"
#include "server.h"

int handle_take(server_t *server, response_t *response, request_t *request)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    int id = -1;

    if (!server || !response || !request)
        return ERROR;
    id = check_ressource_update(server, request, client, unit_space, false);
    if (id == -1)
        return ERROR;
    send_pgt(server, request->client, id);
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        TAKE_TIME);
    return SUCCESS;
}
