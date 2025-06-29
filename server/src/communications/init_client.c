/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_client.c
*/

#include "server.h"

void init_response(response_t *response)
{
    response->client = NULL;
    response->size = 0;
    response->response = NULL;
}

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

static
void init_pending_request(client_t *clients)
{
    clients->data.pending_requests = 0;
    clients->data.pending_response.client = NULL;
    clients->data.pending_response.response = NULL;
    clients->data.pending_response.size = 0;
}

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
    clients->buffer = NULL;
}
