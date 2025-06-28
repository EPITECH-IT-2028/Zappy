/*
** EPITECH PROJECT, 2025
** server
** File description:
** cleanup.c
*/

#include "server.h"
#include <stdlib.h>

static
void cleanup_client_action_queue(client_t *client)
{
    pending_action_t *current = NULL;
    pending_action_t *next = NULL;

    if (!client)
        return;
    pthread_mutex_lock(&client->data.pending_mutex);
    current = client->data.queue_head;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    client->data.queue_head = NULL;
    client->data.queue_tail = NULL;
    pthread_mutex_unlock(&client->data.pending_mutex);
}

void cleanup_game_resources(server_t *server)
{
    if (!server)
        return;
    pthread_mutex_lock(&server->clients_mutex);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i])
            cleanup_client_action_queue(server->clients[i]);
    }
    pthread_mutex_unlock(&server->clients_mutex);
}
