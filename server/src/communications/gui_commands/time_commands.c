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

/**
 * @brief Send current server time frequency to a GUI client
 *
 * This function sends the current server frequency setting to
 * the specified GUI client using the sgt command format.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 */
static
void send_time(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    snprintf(response, BUFFER_SIZE, "sgt %d\n",
        server->params.frequence);
    send_code(server->clients[index]->fd, response);
}

/**
 * @brief Modify server time frequency based on GUI command
 *
 * This function validates and sets a new server frequency value,
 * then sends confirmation or error response to the GUI client.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client sending the command
 * @param freq New frequency value to set
 */
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

/**
 * @brief Handle time-related commands from GUI clients
 *
 * This function processes time commands (sgt, sst) from GUI clients
 * and sends appropriate responses or modifies server settings.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client sending the command
 * @param buffer Command string to process
 */
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
