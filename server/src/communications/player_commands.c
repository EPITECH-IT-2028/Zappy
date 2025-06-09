/*
** EPITECH PROJECT, 2025
** server
** File description:
** player_commands.c
*/

#include "server.h"
#include "utils.h"
#include <string.h>
#include <pthread.h>

static
bool check_commands(const char *buffer)
{
    static const char *commands[] = {
        "Forward", "Right", "Left", "Look", "Inventory",
        "Broadcast", "Connect_nbr", "Fork", "Eject", "Look",
        "Take", "Set", "Incantation", NULL
    };

    for (int i = 0; commands[i] != NULL; i++) {
        if (strcmp(buffer, commands[i]) == 0)
            return true;
    }
    return false;
}

void player_command(server_t *server, int index, const char *buffer)
{
    client_t *client = server->clients[index];
    request_t request;
    int pending = 0;

    pthread_mutex_lock(&client->data.pending_mutex);
    pending = client->data.pending_requests;
    pthread_mutex_unlock(&client->data.pending_mutex);
    if (pending >= 10)
        return;
    if (!check_commands(buffer)) {
        send_code(client->fd, "ko");
    } else {
        request.client = client;
        snprintf(request.request, BUFFER_SIZE, "%s", buffer);
        queue_add_request(server, &request);
    }
}
