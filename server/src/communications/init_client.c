/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_client.c
*/

#include "server.h"

/**
 * @brief Initialize a response structure to default values
 * @param response Pointer to the response structure to initialize
 */
void init_response(response_t *response)
{
    response->client = NULL;
    response->size = 0;
    response->response = NULL;
}

/**
 * @brief Initialize client inventory with default starting values
 *
 * Sets up the initial inventory for a new client with food and
 * zero resources.
 *
 * @param cd Pointer to the client data structure
 */
static
void init_client_inventory(client_data_t *cd)
{
    cd->inventory.food = 10;
    cd->inventory.linemate = 0;
    cd->inventory.deraumere = 0;
    cd->inventory.sibur = 0;
    cd->inventory.mendiane = 0;
    cd->inventory.phiras = 0;
    cd->inventory.thystame = 0;
}

/**
 * @brief Initialize pending request data for a client
 *
 * Sets up the pending request structure with default values
 * for tracking client command queuing.
 *
 * @param clients Pointer to the client structure
 */
static
void init_pending_request(client_t *clients)
{
    clients->data.pending_requests = 0;
    clients->data.pending_response.client = NULL;
    clients->data.pending_response.response = NULL;
    clients->data.pending_response.size = 0;
}

/**
 * @brief Initialize a complete client structure
 *
 * Sets up all client data including connection info, game state,
 * inventory, direction, and communication buffers.
 *
 * @param clients Pointer to the client structure to initialize
 * @param fd File descriptor for the client connection
 */
void init_client_struct(client_t *clients, int fd)
{
    clients->fd = fd;
    clients->addr_len = sizeof(clients->addr);
    clients->fd_open = 0;
    clients->connected = true;
    clients->data.team_name = NULL;
    clients->data.x = 0;
    clients->data.y = 0;
    clients->data.id = -1;
    clients->data.level = 1;
    clients->data.is_graphic = false;
    init_incantation_state(&clients->data.incantation);
    clients->data.is_busy = false;
    init_pending_request(clients);
    init_direction(&clients->data.direction);
    pthread_mutex_init(&clients->data.pending_mutex, NULL);
    init_client_inventory(&clients->data);
    clients->data.queue_head = NULL;
    clients->data.queue_tail = NULL;
    clients->buffer = NULL;
}
