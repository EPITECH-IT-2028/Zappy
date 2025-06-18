/*
** EPITECH PROJECT, 2025
** server
** File description:
** end_incantation.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdlib.h>

static
void notify_incantators_end(client_t **incantators)
{
    char current_level[BUFFER_SIZE] = {0};

    if (!incantators)
        return;
    for (int i = 0; incantators[i] != NULL; i++) {
        if (incantators[i]->connected && incantators[i]->fd > 0) {
            snprintf(current_level, BUFFER_SIZE, "Current level: %d",
                incantators[i]->data.level);
            send_code(incantators[i]->fd, current_level);
        }
    }
}

static
void level_up_all_client(client_t *incantator)
{
    for (int i = 0; incantator->data.incantation.client_group[i] != NULL;
        i++) {
        incantator->data.incantation.client_group[i]->data.level++;
    }
    incantator->data.incantation.is_incantating = true;
}

static
void clear_incantation_data(incantation_t *client_incantation_data)
{
    for (int i = 0; client_incantation_data->client_group[i] != NULL; i++) {
        client_incantation_data->client_group[i]->data.incantation.
            incantation_success = false;
        client_incantation_data->client_group[i]->data.incantation.
            is_incantating = false;
        client_incantation_data->client_group[i]->data.incantation.x = 0;
        client_incantation_data->client_group[i]->data.incantation.y = 0;
    }
    free(client_incantation_data->client_group);
    client_incantation_data->client_group = NULL;
}

int handle_ending_incantation(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;

    if (check_if_incantation_failed(&request->client->data,
        server->clients, &server->map[response->client->data.x]
        [response->client->data.y]) == ERROR) {
        send_pie_all(server, response->client->data.incantation.client_group);
        clear_incantation_data(&response->client->data.incantation);
        return ERROR;
    }
    remove_needed_ressources(&server->map[client_data->incantation.x]
        [client_data->incantation.x], client_data->level - 1);
    level_up_all_client(response->client);
    notify_incantators_end(response->client->data.incantation.client_group);
    send_pie_all(server, response->client->data.incantation.client_group);
    clear_incantation_data(&response->client->data.incantation);
    return SUCCESS;
}
