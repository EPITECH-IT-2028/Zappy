/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_commands.c
*/

#include "server.h"
#include "utils.h"
#include <string.h>

static
void send_pic(int fd, client_t **incantators, char *response)
{
    char id_incantator[INCANTATION_ID_LEN] = {0};

    for (int i = 0; incantators[i]; i++) {
        if (incantators[i] && incantators[i]->connected) {
            sprintf(id_incantator, "#%d ", incantators[i]->data.id);
            strcat(response, id_incantator);
        }
    }
    send_code(fd, response);
}

void send_pic_all(server_t *server, client_t **incantators)
{
    char response[BUFFER_SIZE];

    if (!incantators || !incantators[INDEX_INCANTATOR])
        return;
    snprintf(response, BUFFER_SIZE, "pic %d %d ",
        incantators[INDEX_INCANTATOR]->data.x,
        incantators[INDEX_INCANTATOR]->data.y);
    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] && server->clients[i]->fd > 0 &&
            server->clients[i]->data.is_graphic &&
            server->clients[i]->connected) {
            send_pic(server->clients[i]->fd, incantators, response);
        }
    }
}
