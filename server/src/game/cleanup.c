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

void cleanup_old_actions(client_t *client, server_t *server)
{
    if (!client) return;
    
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    pthread_mutex_lock(&client->data.pending_mutex);
    int queue_size = 0;
    pending_action_t *current = client->data.queue_head;
    while (current) {
        queue_size++;
        current = current->next;
    }
    if (queue_size >= MAX_REQUEST_PER_CLIENT) {
        if (client->data.queue_head) {
            pending_action_t *old_action = client->data.queue_head;
            client->data.queue_head = old_action->next;
            if (!client->data.queue_head) {
                client->data.queue_tail = NULL;
            }
            free(old_action);
        }
    }
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
