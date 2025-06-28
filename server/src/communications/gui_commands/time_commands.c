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

    snprintf(response, BUFFER_SIZE, "sgt %d\n",
        server->params.frequence);
    send_code(server->clients[index]->fd, response);
}

static
void modify_time(server_t *server, int index, int freq)
{
    if (freq <= 0) {
        send_code(server->clients[index]->fd, "sbp\n");
        return;
    }
    server->params.frequence = freq;
    send_time(server, index);
}

void time_commands(server_t *server, int index, char *buffer)
{
    int freq;

    if (strcmp(buffer, "sgt") == 0) {
        send_time(server, index);
        return;
    }
    if (strncmp(buffer, "sst ", 4) == 0 &&
        sscanf(buffer, "sst %d", &freq) == 1) {
        modify_time(server, index, freq);
        return;
    }
    send_code(server->clients[index]->fd, "suc\n");
}
