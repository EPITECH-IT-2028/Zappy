/*
** EPITECH PROJECT, 2025
** server
** File description:
** set.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static
int remove_or_add_ressource(ressources_t *resources, const char *resource,
    bool from_inv_to_map)
{
    for (int i = 0; i < TOTAL_RESOURCES; i++) {
        if (strcmp(resource, resources[i].name) != 0)
            continue;
        if (!from_inv_to_map && *resources[i].inv > 0) {
            (*resources[i].inv)--;
            (*resources[i].space)++;
            return SUCCESS;
        }
        if (from_inv_to_map && *resources[i].space > 0) {
            (*resources[i].inv)++;
            (*resources[i].space)--;
            return SUCCESS;
        }
    }
    return ERROR;
}

int check_if_ressources_exists(client_data_t *client, const char *resource,
    map_t *unit_space, bool from_inv_to_map)
{
    ressources_t resources[] = {
        {"food", &client->inventory.food, &unit_space->food},
        {"linemate", &client->inventory.linemate, &unit_space->linemate},
        {"deraumere", &client->inventory.deraumere, &unit_space->deraumere},
        {"sibur", &client->inventory.sibur, &unit_space->sibur},
        {"mendiane", &client->inventory.mendiane, &unit_space->mendiane},
        {"phiras", &client->inventory.phiras, &unit_space->phiras},
        {"thystame", &client->inventory.thystame, &unit_space->thystame}
    };

    if (remove_or_add_ressource(resources, resource,
        from_inv_to_map) == SUCCESS) {
        return SUCCESS;
    }
    return ERROR;
}

int handle_set(server_t *server, response_t *response, request_t *request)
{
    char *ressource = NULL;
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];

    if (!server || !response || !request)
        return ERROR;
    ressource = get_text_in_commands(request->request, WORD_SET_LENGTH);
    if (!ressource)
        return ERROR;
    if (check_if_ressources_exists(client, ressource, unit_space, false)
        == ERROR) {
        free(ressource);
        return ERROR;
    }
    snprintf(response->response, BUFFER_SIZE, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        SET_TIME);
    free(ressource);
    return SUCCESS;
}
