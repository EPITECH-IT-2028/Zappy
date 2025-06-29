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

/**
 * @brief Updates the server's resource density counters when a resource
 * is added to the map
 *
 * This function increments the density counter for a specific resource
 * type when that resource is placed on the game map.
 *
 * @param server Pointer to the server structure
 * @param type Resource type index (0-6)
 */
void increment_resource_density(server_t *server, int type)
{
    int *density_fields[] = {
        &server->density.food, &server->density.linemate,
        &server->density.deraumere, &server->density.sibur,
        &server->density.mendiane, &server->density.phiras,
        &server->density.thystame
    };

    if (!server)
        return;
    (*density_fields[type])++;
}

/**
 * @brief Updates the server's resource density counters when a resource
 * is removed from the map
 *
 * This function decrements the density counter for a specific resource
 * type when that resource is taken from the game map.
 *
 * @param server Pointer to the server structure
 * @param type Resource type index (0-6)
 */
void decrement_resource_density(server_t *server, int type)
{
    int *density_fields[] = {
        &server->density.food, &server->density.linemate,
        &server->density.deraumere, &server->density.sibur,
        &server->density.mendiane, &server->density.phiras,
        &server->density.thystame
    };

    if (!server)
        return;
    (*density_fields[type])--;
}

/**
 * @brief Handles resource transfer between inventory and map
 *
 * This function manages the transfer of resources either from a client's
 * inventory to the map (set operation) or from the map to inventory
 * (take operation). It updates both locations and density counters.
 *
 * @param server Pointer to the server structure
 * @param resources Array of resource structures containing pointers
 * @param from_inv_to_map True for set operation, false for take operation
 * @param idx_resource Index of the resource type to transfer
 * @return SUCCESS if transfer completed, ERROR otherwise
 */
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

/**
 * @brief Validates if a resource exists and can be transferred
 *
 * This function checks if a named resource exists in the game and whether
 * the requested transfer operation (set/take) can be performed based on
 * availability in the source location.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client data
 * @param resource String name of the resource to check
 * @param from_inv_to_map Transfer direction flag
 * @return Resource index if valid transfer, ERROR otherwise
 */
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

/**
 * @brief Processes and validates a resource transfer request
 *
 * This function parses a client's command to extract the resource name,
 * validates the request, and performs the actual resource transfer
 * operation between inventory and map.
 *
 * @param server Pointer to the server structure
 * @param request Pointer to the client's request
 * @param client Pointer to the client data
 * @param from_inv_to_map True for set command, false for take command
 * @return Resource ID if successful, ERROR otherwise
 */
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
        return ERROR;
    id = check_if_ressources_exists(server, client, resource,
        from_inv_to_map);
    if (id == ERROR) {
        free(resource);
        return ERROR;
    }
    free(resource);
    return id;
}

/**
 * @brief Handles the "Set" command for placing resources on the map
 *
 * This function processes a client's request to place a resource from
 * their inventory onto the current map tile. It validates the operation,
 * updates the game state, and sends appropriate notifications.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to response structure to fill with result
 * @param request Pointer to the client's set request
 * @return SUCCESS if resource was placed successfully, ERROR otherwise
 */
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
    if (add_buffer_to_response("ok", &response->response, &response->size)
        == ERROR)
        return ERROR;
    send_bct(server, client->x, client->y);
    return SUCCESS;
}
