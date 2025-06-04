/*
** EPITECH PROJECT, 2025
** server
** File description:
** time_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

static
void send_time(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "sgt %d",
        server->params.frequence);
    send_code(server->clients[index]->fd, response);
}

static
void modify_time(server_t *server, int index, char *buffer)
{
    int new_freq = atoi(buffer + 3);

    if (new_freq <= 0) {
        send_code(server->clients[index]->fd, "sbp");
        return;
    }
    server->params.frequence = new_freq;
    send_time(server, index);
}

void time_commands(server_t *server, int index, char *buffer)
{
    if (strcmp(buffer, "sgt") == 0) {
        send_time(server, index);
        return;
    }
    if (strncmp(buffer, "sst ", 4) == 0) {
        modify_time(server, index, buffer);
        return;
    }
    send_code(server->clients[index]->fd, "suc");
}
