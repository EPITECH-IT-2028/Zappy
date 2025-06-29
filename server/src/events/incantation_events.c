/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Send pie (incantation end) message to all graphic clients
 *
 * This function formats a message indicating the completion of an
 * incantation with position and success/failure status.
 *
 * @param server Pointer to the server instance containing client list
 * @param incantators Array of clients participating in the incantation
 */
void send_pie(server_t *server, client_t **incantators)
{
    char response[BUFFER_SIZE];

    if (!incantators || !incantators[INDEX_INCANTATOR])
        return;
    snprintf(response, BUFFER_SIZE, "pie %d %d %d\n",
        incantators[INDEX_INCANTATOR]->data.x,
        incantators[INDEX_INCANTATOR]->data.y,
        !incantators[INDEX_INCANTATOR]->data.incantation.incantation_success);
    for (int i = MIN_CLIENT; i < server->nfds; i++) {
        if (server->clients[i] && server->clients[i]->fd > 0 &&
            server->clients[i]->data.is_graphic &&
            server->clients[i]->connected) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

/**
 * @brief Send pic (incantation start) message to all graphic clients
 *
 * This function formats a message indicating the start of an
 * incantation with position and list of participating players.
 *
 * @param server Pointer to the server instance containing client list
 * @param incantators Array of clients participating in the incantation
 */
void send_pic(server_t *server, client_t **incantators)
{
    char response[BUFFER_SIZE];
    char id_incantator[INCANTATION_ID_LEN] = {0};

    if (!incantators || !incantators[INDEX_INCANTATOR])
        return;
    snprintf(response, BUFFER_SIZE, "pic %d %d ",
        incantators[INDEX_INCANTATOR]->data.x,
        incantators[INDEX_INCANTATOR]->data.y);
    for (int i = 0; incantators[i]; i++)
        if (incantators[i] && incantators[i]->connected) {
            sprintf(id_incantator, "#%d ", incantators[i]->data.id);
            strcat(response, id_incantator);
        }
    strcat(response, "\n");
    for (int i = MIN_CLIENT; i < server->nfds; i++)
        if (server->clients[i] &&
            server->clients[i]->fd > 0 &&
            server->clients[i]->data.is_graphic &&
            server->clients[i]->connected)
            send_code(server->clients[i]->fd, response);
}

static
void send_level_to_gui(server_t *server, char *buffer)
{
    char response[BUFFER_SIZE] = {0};
    int id = 0;
    int level = 0;

    if (sscanf(buffer, "plv #%d %d", &id, &level) != 2)
        return;
    snprintf(response, BUFFER_SIZE, "plv #%d %d\n", id, level);
    for (int i = MIN_CLIENT; i < server->nfds; i++) {
        if (server->clients[i] && server->clients[i]->fd > 0 &&
            server->clients[i]->data.is_graphic &&
            server->clients[i]->connected) {
            send_code(server->clients[i]->fd, response);
        }
    }
}

void send_plv(server_t *server, client_t **incantators)
{
    char **response = NULL;
    char level[BUFFER_SIZE] = {0};
    int size = 1;

    for (int i = 0; incantators[i] != NULL; i++) {
        response = realloc(response, sizeof(char *) * (size + 1));
        sprintf(level, "plv #%d %d\n", incantators[i]->data.id,
                incantators[i]->data.level);
        response[i] = strdup(level);
        response[i + 1] = NULL;
        size++;
    }
    for (int i = MIN_CLIENT; i < server->nfds; i++)
        for (int j = 0; response && response[j] != NULL; j++) {
            send_level_to_gui(server, response[j]);
        }
}
