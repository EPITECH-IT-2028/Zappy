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
    int id = -1;

    if (!server || !response || !request)
        return ERROR;
    id = check_ressource_update(server, request, client, false);
    if (id == ERROR)
        return ERROR;
    send_pgt(server, request->client, id);
    add_buffer_to_response("ok", &response->response, &response->size);
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        TAKE_TIME);
    return SUCCESS;
}
