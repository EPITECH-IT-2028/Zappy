/*
** EPITECH PROJECT, 2025
** server
** File description:
** set.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Updates the server's resource density counters when a resource is added to the map
 * @param server Server structure
 * @param type Resource type (0-6)
 */
void increment_resource_density(server_t *server, int type)
{
    if (!server)
        return;

    switch (type) {
        case 0: server->density.food++; break;
        case 1: server->density.linemate++; break;
        case 2: server->density.deraumere++; break;
        case 3: server->density.sibur++; break;
        case 4: server->density.mendiane++; break;
        case 5: server->density.phiras++; break;
        case 6: server->density.thystame++; break;
    }
}

/**
 * Updates the server's resource density counters when a resource is removed from the map
 * @param server Server structure
 * @param type Resource type (0-6)
 */
void decrement_resource_density(server_t *server, int type)
{
    if (!server)
        return;

    switch (type) {
        case 0: if (server->density.food > 0) server->density.food--; break;
        case 1: if (server->density.linemate > 0) server->density.linemate--; break;
        case 2: if (server->density.deraumere > 0) server->density.deraumere--; break;
        case 3: if (server->density.sibur > 0) server->density.sibur--; break;
        case 4: if (server->density.mendiane > 0) server->density.mendiane--; break;
        case 5: if (server->density.phiras > 0) server->density.phiras--; break;
        case 6: if (server->density.thystame > 0) server->density.thystame--; break;
    }
}

int remove_or_add_ressource(server_t *server, ressources_t *resources,
    bool from_inv_to_map, uint8_t idx_resource)
{
    if (from_inv_to_map == true && *resources[idx_resource].inv > 0) {
        (*resources[idx_resource].inv)--;
        (*resources[idx_resource].space)++;
        if (server)
            increment_resource_density(server, idx_resource);
        return SUCCESS;
    }
    if (from_inv_to_map == false && *resources[idx_resource].space > 0) {
        (*resources[idx_resource].inv)++;
        (*resources[idx_resource].space)--;
        if (server)
            decrement_resource_density(server, idx_resource);
        return SUCCESS;
    }
    return ERROR;
}

static
int check_if_ressources_exists(server_t *server, client_data_t *client, const char *resource,
    map_t *unit_space, bool from_inv_to_map)
{
    int idx_resource = -1;
    ressources_t resources[] = {
        {"food", &client->inventory.food, &unit_space->food},
        {"linemate", &client->inventory.linemate, &unit_space->linemate},
        {"deraumere", &client->inventory.deraumere, &unit_space->deraumere},
        {"sibur", &client->inventory.sibur, &unit_space->sibur},
        {"mendiane", &client->inventory.mendiane, &unit_space->mendiane},
        {"phiras", &client->inventory.phiras, &unit_space->phiras},
        {"thystame", &client->inventory.thystame, &unit_space->thystame}
    };

    for (int i = 0; i < TOTAL_RESOURCES; i++)
        if (strcmp(resource, resources[i].name) == 0)
            idx_resource = i;
    if (idx_resource == -1)
        return -1;
    if (remove_or_add_ressource(server, resources,
        from_inv_to_map, idx_resource) == ERROR)
        return -1;
    return idx_resource;
}

int check_ressource_update(server_t *server, request_t *request, client_data_t *client,
    map_t *unit_space, bool from_inv_to_map)
{
    char *resource = NULL;
    int id = -1;

    resource = get_text_in_commands(request->request,
        from_inv_to_map == true ? WORD_SET_LENGTH : WORD_TAKE_LENGTH);
    if (!resource)
        return -1;
    id = check_if_ressources_exists(server, client, resource, unit_space,
        from_inv_to_map);
    if (id == -1) {
        free(resource);
        return -1;
    }
    free(resource);
    return id;
}

int handle_set(server_t *server, response_t *response, request_t *request)
{
    client_data_t *client = &request->client->data;
    map_t *unit_space = &server->map[client->x][client->y];
    int id = -1;

    if (!server || !response || !request)
        return ERROR;
    id = check_ressource_update(server, request, client, unit_space, true);
    if (id == -1)
        return ERROR;
    send_pdr(server, request->client, id);
    snprintf(response->response, BUFFER_SIZE, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        SET_TIME);
    return SUCCESS;
}
