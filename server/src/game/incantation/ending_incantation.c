/*
** EPITECH PROJECT, 2025
** server
** File description:
** end_incantation.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdlib.h>

/**
 * @brief Notify all incantators about the end of incantation
 *
 * This function sends a message to all connected incantators
 * informing them of their current level after the incantation.
 *
 * @param incantators Array of client pointers participating in incantation
 */
static
void notify_incantators_end(client_t **incantators)
{
    char current_level[BUFFER_SIZE] = {0};

    if (!incantators)
        return;
    for (int i = 0; incantators[i] != NULL; i++) {
        if (incantators[i]->connected && incantators[i]->fd > 0) {
            snprintf(current_level, BUFFER_SIZE, "Current level: %d\n",
                incantators[i]->data.level);
            send_code(incantators[i]->fd, current_level);
        }
    }
}

/**
 * @brief Level up all clients in the incantation group
 *
 * This function increments the level of all clients participating
 * in a successful incantation and maintains incantation state.
 *
 * @param incantator Pointer to the main incantator client
 */
static
void level_up_all_client(client_t *incantator)
{
    for (int i = 0; incantator->data.incantation.client_group[i] != NULL;
        i++) {
        incantator->data.incantation.client_group[i]->data.level++;
    }
    incantator->data.incantation.is_incantating = true;
}

/**
 * @brief Clear and reset incantation data for all participants
 *
 * This function resets the incantation state for all group members
 * and frees the allocated memory for the client group.
 *
 * @param client_incantation_data Pointer to incantation data to clear
 */
static
void clear_incantation_data(incantation_t *client_incantation_data)
{
    for (int i = 0; client_incantation_data->client_group[i] != NULL; i++) {
        client_incantation_data->client_group[i]->data.incantation.
            incantation_success = false;
        client_incantation_data->client_group[i]->data.incantation.
            is_incantating = false;
        client_incantation_data->client_group[i]->data.incantation.x = 0;
        client_incantation_data->client_group[i]->data.incantation.y = 0;
    }
    free(client_incantation_data->client_group);
    client_incantation_data->client_group = NULL;
}

/**
 * @brief Handle the completion of an incantation process
 *
 * This function processes the end of an incantation by checking
 * success conditions, leveling up participants, and cleaning up.
 *
 * @param server Pointer to the server structure
 * @param response Pointer to the response structure to fill
 * @param request Pointer to the client request
 * @return SUCCESS if incantation succeeded, ERROR if it failed
 */
int handle_ending_incantation(server_t *server, response_t *response,
    request_t *request)
{
    client_data_t *client_data = &request->client->data;

    if (!client_data->incantation.client_group)
        return ERROR;
    if (check_if_incantation_failed(&request->client->data,
        server->clients, &server->map[client_data->x][client_data->y])
        == ERROR) {
        send_pie(server, client_data->incantation.client_group);
        clear_incantation_data(&client_data->incantation);
        return ERROR;
    }
    remove_needed_ressources(&server->map[client_data->incantation.x]
        [client_data->incantation.y], client_data->level - 1);
    level_up_all_client(response->client);
    notify_incantators_end(client_data->incantation.client_group);
    send_pie(server, client_data->incantation.client_group);
    clear_incantation_data(&client_data->incantation);
    return SUCCESS;
}
