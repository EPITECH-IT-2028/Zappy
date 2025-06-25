/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

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
