/*
** EPITECH PROJECT, 2025
** server
** File description:
** entry.c
*/

#include "server.h"
#include "ai_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
int get_action_duration(const char *command)
{
    size_t cmd_len = 0;

    if (!command)
        return 0;
    for (int i = 0; tab_action_durations[i].command != NULL; i++) {
        cmd_len = strlen(tab_action_durations[i].command);
        if (strncmp(command, tab_action_durations[i].command, cmd_len) == 0 &&
            (command[cmd_len] == '\0' || command[cmd_len] == ' ')) {
            return tab_action_durations[i].duration;
        }
    }
    return 0;
}

int check_request(server_t *server, response_t *response, request_t *request)
{
    int result = ERROR;
    size_t len = 0;

    for (int i = 0; tab_ai_commands[i].request != NULL; i++) {
        len = strlen(tab_ai_commands[i].request);
        if (strncmp(tab_ai_commands[i].request, request->request, len) == 0 &&
            (request->request[len] == '\0' || request->request[len] == ' ')) {
            result = tab_ai_commands[i].func(server, response, request);
            break;
        }
    }
    return result;
}

static
void set_queue(client_t *client, pending_action_t *new_action,
    struct timespec *duration_spec)
{
    struct timespec current_time;

    if (client->data.queue_tail)
        new_action->execute_time = timespec_add(&client->data.queue_tail
            ->execute_time, duration_spec);
    else {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        new_action->execute_time = timespec_add(&current_time, duration_spec);
    }
    pthread_mutex_lock(&client->data.pending_mutex);
    if (!client->data.queue_head) {
        client->data.queue_head = new_action;
        client->data.queue_tail = new_action;
    } else {
        client->data.queue_tail->next = new_action;
        client->data.queue_tail = new_action;
    }
    pthread_mutex_unlock(&client->data.pending_mutex);
}

int add_action_to_client_queue(client_t *client, const char *command,
    server_t *server)
{
    pending_action_t *new_action = malloc(sizeof(pending_action_t));
    int duration = 0;
    struct timespec duration_spec;

    if (!client || !command || !server || !new_action)
        return ERROR;
    strncpy(new_action->command, command, BUFFER_SIZE - 1);
    new_action->command[BUFFER_SIZE - 1] = '\0';
    new_action->next = NULL;
    duration = get_action_duration(command);
    duration_spec = calculate_action_duration(duration,
        server->params.frequence);
    set_queue(client, new_action, &duration_spec);
    return SUCCESS;
}
