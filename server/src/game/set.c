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
 * Updates the server's resource density counters when a resource is added
 * to the map
 * @param server Server structure
 * @param type Resource type (0-6)
 */
void increment_resource_density(server_t *server, int type)
{
    if (!server)
        return;
    if (type == 0)
        server->density.food++;
    if (type == 1)
        server->density.linemate++;
    if (type == 2)
        server->density.deraumere++;
    if (type == 3)
        server->density.sibur++;
    if (type == 4)
        server->density.mendiane++;
    if (type == 5)
        server->density.phiras++;
    if (type == 6)
        server->density.thystame++;
}

/**
 * Updates the server's resource density counters when a resource is removed
 * from the map
 * @param server Server structure
 * @param type Resource type (0-6)
 */
void decrement_resource_density(server_t *server, int type)
{
    if (!server)
        return;
    if (type == 0 && server->density.food > 0)
        server->density.food--;
    if (type == 1 && server->density.linemate > 0)
        server->density.linemate--;
    if (type == 2 && server->density.deraumere > 0)
        server->density.deraumere--;
    if (type == 3 && server->density.sibur > 0)
        server->density.sibur--;
    if (type == 4 && server->density.mendiane > 0)
        server->density.mendiane--;
    if (type == 5 && server->density.phiras > 0)
        server->density.phiras--;
    if (type == 6 && server->density.thystame)
        server->density.thystame--;
}

int remove_or_add_ressource(server_t *server, ressources_t *resources,
    bool from_inv_to_map, uint8_t idx_resource)
{
    if (idx_resource == ERROR)
        return ERROR;
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
int check_if_ressources_exists(
    server_t *server,
    client_data_t *client,
    const char *resource,
    bool from_inv_to_map
)
{
    int idx_resource = ERROR;
    map_t *unit_space = &server->map[client->x][client->y];
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
    if (remove_or_add_ressource(server, resources,
        from_inv_to_map, idx_resource) == ERROR)
        return ERROR;
    return idx_resource;
}

int check_ressource_update(
    server_t *server,
    request_t *request,
    client_data_t *client,
    bool from_inv_to_map
)
{
    char *resource = NULL;
    int id = ERROR;

    resource = get_text_in_commands(request->request,
        from_inv_to_map == true ? WORD_SET_LENGTH : WORD_TAKE_LENGTH);
    if (!resource)
        return -1;
    id = check_if_ressources_exists(server, client, resource,
        from_inv_to_map);
    if (id == ERROR) {
        free(resource);
        return ERROR;
    }
    free(resource);
    return id;
}

int handle_set(server_t *server, response_t *response, request_t *request)
{
    client_data_t *client = &request->client->data;
    int id = ERROR;

    if (!server || !response || !request)
        return ERROR;
    id = check_ressource_update(server, request, client, true);
    if (id == ERROR)
        return ERROR;
    send_pdr(server, request->client, id);
    snprintf(response->response, BUFFER_SIZE, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time = get_action_end_time(server,
        SET_TIME);
    return SUCCESS;
}
