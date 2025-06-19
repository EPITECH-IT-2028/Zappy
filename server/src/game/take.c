/*
** EPITECH PROJECT, 2025
** server
** File description:
** take.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

int handle_take(server_t *server, response_t *response, request_t *request)
{
    char *ressource = NULL;
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];

    if (!server || !response || !request)
        return ERROR;
    ressource = get_text_in_commands(request->request, WORD_TAKE_LENGTH);
    if (!ressource)
        return ERROR;
    if (check_if_ressources_exists(client, ressource, unit_space, true)
        == ERROR) {
        free(ressource);
        return ERROR;
    }
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        TAKE_TIME);
    free(ressource);
    return SUCCESS;
}
