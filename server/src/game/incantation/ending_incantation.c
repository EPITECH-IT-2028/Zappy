/*
** EPITECH PROJECT, 2025
** server
** File description:
** end_incantation.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"

static
void notify_incantators_end(client_t **incantators)
{
    char current_level[BUFFER_SIZE] = {0};

    if (!incantators)
        return;
    for (int i = 0; incantators[i] != NULL; i++) {
        printf("%d\n", i);
        if (incantators[i]->connected && incantators[i]->fd > 0) {
            snprintf(current_level, BUFFER_SIZE, "Current level: %d", incantators[i]->data.level);
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
}

int handle_ending_incantation(server_t *server, response_t *response,
    request_t *request)
{
    if (check_if_incantation_failed(&request->client->data,
        server->clients, &server->map[response->client->data.x]
        [response->client->data.y]) == ERROR)
        return ERROR;
    level_up_all_client(response->client);
    notify_incantators_end(response->client->data.incantation.client_group);
    // send_pie_all(server, response->client->data.incantation.client_group);
    return SUCCESS;
}
