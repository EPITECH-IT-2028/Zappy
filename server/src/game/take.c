/*
** EPITECH PROJECT, 2025
** server
** File description:
** take.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

static
int check_if_ressources_exists(client_data_t *client, const char *ressource,
    map_t *unit_space)
{
    struct { const char *name; int *inv; int *space; } resources[] = {
        {"food", &client->inventory.food, &unit_space->food},
        {"linemate", &client->inventory.linemate, &unit_space->linemate},
        {"deraumere", &client->inventory.deraumere, &unit_space->deraumere},
        {"sibur", &client->inventory.sibur, &unit_space->sibur},
        {"mendiane", &client->inventory.mendiane, &unit_space->mendiane},
        {"phiras", &client->inventory.phiras, &unit_space->phiras},
        {"thystame", &client->inventory.thystame, &unit_space->thystame}
    };

    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        if (!strcmp(ressource, resources[i].name) && *resources[i].space > 0) {
            (*resources[i].inv)++;
            (*resources[i].space)--;
            return SUCCESS;
        }
    }
    return ERROR;
}

int handle_take(server_t *server, response_t *response, request_t *request)
{
    char *ressource = NULL;
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];

    if (!server || !response || !request)
        return ERROR;
    ressource = get_text_in_commands(request->request, WORD_TAKE_LENGTH);    
    if (check_if_ressources_exists(client, ressource, unit_space) == ERROR)
        return ERROR;
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        TAKE_TIME);
    return SUCCESS;
}
