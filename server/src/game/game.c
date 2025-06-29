/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game.c
*/

#include "macro.h"
#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Execute a pending action for a client
 *
 * This function removes an action from the client's queue and
 * executes it, sending the response back to the client.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client executing the action
 * @param action Pointer to the pending action to execute
 */
static
void execute_pending_action(server_t *server, client_t *client,
    pending_action_t *action)
{
    request_t request = {0};
    response_t response = {0};

    client->data.queue_head = action->next;
    if (!client->data.queue_head)
        client->data.queue_tail = NULL;
    pthread_mutex_unlock(&client->data.pending_mutex);
    request.client = client;
    strncpy(request.request, action->command, BUFFER_SIZE);
    init_response(&response);
    response.client = client;
    if (check_request(server, &response, &request) == ERROR)
        add_buffer_to_response("ko", &response.response, &response.size);
    if (response.response && response.size > 0)
        check_if_queue_is_full(server, &response);
    free(action);
}

/**
 * @brief Process pending actions for a client based on timing
 *
 * This function checks if a client has pending actions ready to
 * execute based on their scheduled execution time.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to process
 */
static
void process_client_actions(server_t *server, client_t *client)
{
    struct timespec current_time;
    pending_action_t *action = NULL;

    if (!client || !client->connected)
        return;
    pthread_mutex_lock(&client->data.pending_mutex);
    if (!client->data.queue_head) {
        pthread_mutex_unlock(&client->data.pending_mutex);
        return;
    }
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    action = client->data.queue_head;
    if (current_time.tv_sec > action->execute_time.tv_sec ||
        (current_time.tv_sec == action->execute_time.tv_sec &&
        current_time.tv_nsec >= action->execute_time.tv_nsec)) {
        execute_pending_action(server, client, action);
    } else {
        pthread_mutex_unlock(&client->data.pending_mutex);
    }
}

/**
 * @brief Process pending responses for a client
 *
 * This function checks if a client has pending responses and
 * adds them to the response queue for transmission.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to process responses for
 */
static
void process_client_response(server_t *server, client_t *client)
{
    response_t tmp;

    if (!client || !client->connected)
        return;
    pthread_mutex_lock(&client->data.pending_mutex);
    if (client->data.pending_response.response) {
        tmp = client->data.pending_response;
        client->data.pending_response.response = NULL;
        client->data.pending_response.size = 0;
        pthread_mutex_unlock(&client->data.pending_mutex);
        check_if_queue_is_full(server, &tmp);
    } else {
        pthread_mutex_unlock(&client->data.pending_mutex);
    }
}

/**
 * @brief Handle incantation completion for a client
 *
 * This function processes incantation completion when a client
 * finishes their incantation cooldown period.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client performing incantation
 */
static
void handle_incantation(server_t *server, client_t *client)
{
    response_t completion_response;
    request_t completion_request;

    if (!client->data.incantation.is_incantating)
        return;
    if (is_client_on_cd(&client->data))
        return;
    init_response(&completion_response);
    completion_response.client = client;
    completion_request.client = client;
    strcpy(completion_request.request, "Incantation");
    if (handle_ending_incantation(server, &completion_response,
            &completion_request) == ERROR)
        add_buffer_to_response("ko", &completion_response.response,
            &completion_response.size);
    if (completion_response.response && completion_response.size > 0)
        check_if_queue_is_full(server, &completion_response);
}

/**
 * @brief Process all connected clients for game logic
 *
 * This function iterates through all connected non-graphic clients
 * and processes their actions, responses, and incantations.
 *
 * @param server Pointer to the server structure
 */
static
void process_all_clients(server_t *server)
{
    client_t *client = NULL;

    if (server->nfds <= 1)
        return;
    pthread_mutex_lock(&server->clients_mutex);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL || server->fds[i].fd == -1)
            continue;
        if (!server->clients[i]->connected)
            continue;
        if (server->clients[i]->data.is_graphic)
            continue;
        client = server->clients[i];
        cleanup_old_actions(client);
        process_client_actions(server, client);
        process_client_response(server, client);
        handle_incantation(server, client);
    }
    pthread_mutex_unlock(&server->clients_mutex);
}

/**
 * @brief Main game loop thread function
 *
 * This function runs the main game loop, processing clients and
 * handling time-based events until the server stops running.
 *
 * @param arg Pointer to the server structure (cast from void*)
 * @return NULL when the game loop exits
 */
static
void *game(void *arg)
{
    server_t *server = (server_t *)arg;

    clock_gettime(CLOCK_MONOTONIC, &server->server_timer);
    clock_gettime(CLOCK_MONOTONIC, &server->density_timer);
    server->server_timer_count =
        get_current_timer_units(server, &server->server_timer);
    server->density_timer_count =
        get_current_timer_units(server, &server->density_timer);
    while (server->running) {
        process_all_clients(server);
        check_time_events(server);
        check_victory(server);
        sleep_time(server);
    }
    return NULL;
}

int game_loop(server_t *server)
{
    if (pthread_create(&server->threads.game_thread, NULL, game, server)) {
        perror("pthread_create game_thread failed");
        return ERROR;
    }
    return SUCCESS;
}
