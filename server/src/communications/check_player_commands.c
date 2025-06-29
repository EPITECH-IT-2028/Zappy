/*
** EPITECH PROJECT, 2025
** server
** File description:
** check_player_commands.c
*/

#include "server.h"
#include "utils.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

/**
 * @brief Check if a command string is valid
 *
 * This function validates if the provided command is in the list
 * of accepted game commands.
 *
 * @param buffer Command string to validate
 * @return true if command is valid, false otherwise
 */
static
bool check_commands(const char *buffer)
{
    static const char *commands[] = {
        "Forward", "Right", "Left", "Look", "Inventory",
        "Broadcast", "Connect_nbr", "Fork", "Eject",
        "Take", "Set", "Incantation", NULL
    };

    for (int i = 0; commands[i] != NULL; i++) {
        if (strncmp(buffer, commands[i], strlen(commands[i])) == 0)
            return true;
    }
    return false;
}

/**
 * @brief Count number of pending actions in client's queue
 *
 * This function safely counts the number of actions waiting
 * in a client's action queue using mutex protection.
 *
 * @param client Pointer to the client structure
 * @return Number of pending actions in the queue
 */
static
int count_client_actions(client_t *client)
{
    int count = 0;
    pending_action_t *current = NULL;

    if (!client)
        return 0;
    pthread_mutex_lock(&client->data.pending_mutex);
    current = client->data.queue_head;
    while (current) {
        count++;
        current = current->next;
    }
    pthread_mutex_unlock(&client->data.pending_mutex);
    return count;
}

/**
 * @brief Validate and queue player commands
 *
 * This function validates player commands, checks queue limits,
 * and adds valid commands to the client's action queue.
 *
 * @param server Pointer to the server structure
 * @param index Index of the client in the server's client array
 * @param buffer Command string to process
 */
void check_player_command(server_t *server, int index, const char *buffer)
{
    client_t *client = server->clients[index];
    int action_count = 0;

    if (!client || !buffer || !server || !client->connected
        || !client->data.team_name || client->data.is_graphic)
        return;
    cleanup_old_actions(client);
    action_count = count_client_actions(client);
    if (action_count >= MAX_REQUEST_PER_CLIENT)
        return;
    if (!check_commands(buffer)) {
        send_code(client->fd, "ko\n");
        return;
    }
    if (add_action_to_client_queue(client, buffer, server) == ERROR)
        send_code(client->fd, "ko\n");
}
