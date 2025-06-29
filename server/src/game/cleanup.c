/*
** EPITECH PROJECT, 2025
** server
** File description:
** cleanup.c
*/

#include "server.h"
#include <stdlib.h>

/**
 * @brief Clean up a client's action queue and free all pending actions
 *
 * This function safely clears all pending actions in a client's queue
 * using mutex locks to ensure thread safety.
 *
 * @param client Pointer to the client whose action queue to clean
 */
static
void cleanup_client_action_queue(client_t *client)
{
    pending_action_t *current = NULL;
    pending_action_t *next = NULL;

    if (!client)
        return;
    if (pthread_mutex_lock(&client->data.pending_mutex) != 0)
        return;
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

/**
 * @brief Count the number of actions in a client's queue
 *
 * This function counts all pending actions in a client's action queue
 * in a thread-safe manner using mutex locks.
 *
 * @param client Pointer to the client whose queue to count
 * @param queue_size Pointer to store the resulting queue size
 * @return SUCCESS on success, ERROR on failure
 */
static
int count_queue(client_t *client, int *queue_size)
{
    pending_action_t *current = NULL;

    if (!client || !queue_size)
        return ERROR;
    if (pthread_mutex_lock(&client->data.pending_mutex) != 0)
        return ERROR;
    current = client->data.queue_head;
    *queue_size = 0;
    while (current) {
        (*queue_size)++;
        current = current->next;
    }
    pthread_mutex_unlock(&client->data.pending_mutex);
    return SUCCESS;
}

/**
 * @brief Remove the oldest action from a client's queue
 *
 * This function removes and frees the oldest pending action from
 * a client's queue, updating the queue pointers accordingly.
 *
 * @param client Pointer to the client whose oldest action to remove
 */
static
void remove_oldest_action(client_t *client)
{
    pending_action_t *old_action = NULL;

    pthread_mutex_lock(&client->data.pending_mutex);
    old_action = client->data.queue_head;
    client->data.queue_head = old_action->next;
    if (!client->data.queue_head)
        client->data.queue_tail = NULL;
    free(old_action);
    pthread_mutex_unlock(&client->data.pending_mutex);
}

/**
 * @brief Clean up old actions when queue exceeds maximum size
 *
 * This function removes oldest actions from a client's queue when
 * it exceeds the maximum allowed size plus a buffer of 5 actions.
 *
 * @param client Pointer to the client whose queue to clean up
 */
void cleanup_old_actions(client_t *client)
{
    int queue_size = 0;

    if (count_queue(client, &queue_size) == ERROR)
        return;
    while (queue_size > MAX_REQUEST_PER_CLIENT + 5) {
        if (client->data.queue_head) {
            remove_oldest_action(client);
            queue_size--;
        } else {
            break;
        }
    }
}

/**
 * @brief Clean up all game resources including client action queues
 *
 * This function performs cleanup of all game-related resources,
 * particularly cleaning up action queues for all connected clients.
 *
 * @param server Pointer to the server structure containing game data
 */
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
