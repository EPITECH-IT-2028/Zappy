/*
** EPITECH PROJECT, 2025
** server
** File description:
** player_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

/**
 * @brief Send team names to a GUI client
 *
 * This function sends all available team names to the specified
 * GUI client using the tna command format.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 */
static
void send_teams_name(server_t *server, int index)
{
    char response[BUFFER_SIZE];

    for (int i = 0; i < server->params.teams_count; i++) {
        if (server->teams[i].name != NULL) {
            snprintf(response, BUFFER_SIZE, "tna %s\n", server->teams[i].name);
            send_code(server->clients[index]->fd, response);
        }
    }
}

/**
 * @brief Send specific player position to a GUI client
 *
 * This function sends a player's position and direction to the GUI
 * client, or an error message if the player is not found.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client to send to
 * @param id ID of the player to query
 */
static
void player_position(server_t *server, int index, int id)
{
    char response[BUFFER_SIZE];
    client_t *client = find_ai_by_id(server, id);

    if (client == NULL) {
        send_code(server->clients[index]->fd, "sbp\n");
        return;
    }
    snprintf(response, BUFFER_SIZE, "ppo #%d %d %d %d\n",
        id,
        client->data.x,
        client->data.y,
        client->data.direction + 1);
    send_code(server->clients[index]->fd, response);
}

/**
 * @brief Handle player-related commands from GUI clients
 *
 * This function processes player commands (tna, ppo) from GUI clients
 * and sends appropriate responses with player information.
 *
 * @param server Pointer to the server structure
 * @param index Index of the GUI client sending the command
 * @param buffer Command string to process
 */
void player_commands(server_t *server, int index, char *buffer)
{
    int player_id;

    if (strcmp(buffer, "tna") == 0) {
        send_teams_name(server, index);
    } else if (sscanf(buffer, "ppo #%d", &player_id) == 1) {
        player_position(server, index, player_id);
    } else {
        send_code(server->clients[index]->fd, "suc\n");
    }
}
