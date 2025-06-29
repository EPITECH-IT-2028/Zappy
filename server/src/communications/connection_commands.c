/*
** EPITECH PROJECT, 2025
** server
** File description:
** connection_commands.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <string.h>

/**
 * @brief Check if a team name exists in the server
 * @param server Pointer to the server structure
 * @param buffer The team name to check
 * @return true if team exists, false otherwise
 */
static
bool has_team_name(server_t *server, const char *buffer)
{
    return find_team_index(server, buffer) != ERROR;
}

/**
 * @brief Set client data based on connection type
 *
 * This function initializes client data differently based on whether
 * the client is a GUI client or an AI player.
 *
 * @param server Pointer to the server structure
 * @param index Index of the client in the server's client array
 * @param name Team name or client identifier
 * @param is_graphic Boolean indicating if client is a GUI client
 * @return SUCCESS on successful data setting, ERROR on failure
 */
static
int set_data(server_t *server, int index, const char *name, bool is_graphic)
{
    server->clients[index]->data.is_graphic = is_graphic;
    server->clients[index]->data.team_name = strdup(name);
    if (is_graphic) {
        server->clients[index]->data.id = -1;
        server->clients[index]->data.team_id = UNASSIGNED_PLAYER_ID;
    } else {
        server->clients[index]->data.id = server->ids;
        server->clients[index]->data.team_id = find_team_index(server, name);
        server->ids++;
    }
    if (server->clients[index]->data.team_name == NULL)
        perror("strdup failed");
    return SUCCESS;
}

/**
 * @brief Send new player notification to all GUI clients
 *
 * This function safely sends a player new (pnw) message to all
 * connected GUI clients using mutex protection.
 *
 * @param server Pointer to the server structure
 * @param index Index of the new player in the client array
 */
static
void send_new_player_to_gui(server_t *server, int index)
{
    pthread_mutex_lock(&server->clients_mutex);
    send_pnw(server, index);
    pthread_mutex_unlock(&server->clients_mutex);
}

/**
 * @brief Handle AI client connection and setup
 *
 * This function processes AI client connections by validating team
 * capacity, setting up client data, and assigning spawn position.
 *
 * @param server Pointer to the server structure
 * @param index Index of the connecting client
 * @param buffer Team name from the client
 * @param response Buffer to store the response message
 * @return SUCCESS on successful AI setup, ERROR on failure
 */
static
int send_ai(server_t *server, int index, char *buffer, char *response)
{
    int team_index = find_team_index(server, buffer);
    int remaining_slots = 0;

    if (server->teams[team_index].clients_count >=
        server->params.client_per_team || team_index == ERROR) {
        send_code(server->clients[index]->fd, "ko\n");
        return ERROR;
    }
    server->teams[team_index].clients_count++;
    if (set_data(server, index, buffer, false) == ERROR)
        return ERROR;
    if (assign_random_egg_position(server, server->clients[index]) == ERROR)
        return ERROR;
    remaining_slots = server->params.client_per_team -
        server->teams[team_index].clients_count;
    snprintf(response, BUFFER_SIZE, "%d\n%d %d\n", remaining_slots,
        server->params.width, server->params.height);
    send_new_player_to_gui(server, index);
    return SUCCESS;
}

/**
 * @brief Handle GUI client connection and initial data sending
 *
 * This function sets up a GUI client connection and sends all
 * initial game state information including map, time, and players.
 *
 * @param server Pointer to the server structure
 * @param index Index of the connecting GUI client
 * @param buffer Client identifier from the connection
 * @return SUCCESS on successful GUI setup, ERROR on failure
 */
static
int send_gui(server_t *server, int index, char *buffer)
{
    if (set_data(server, index, buffer, true) == ERROR)
        return ERROR;
    map_commands(server, index, "msz");
    time_commands(server, index, "sgt");
    map_commands(server, index, "mct");
    player_commands(server, index, "tna");
    send_all_eggs_to_gui(server);
    return SUCCESS;
}

/**
 * @brief Handle initial client connections based on client type
 *
 * This function processes new client connections and routes them
 * to appropriate handlers based on whether they are GUI or AI clients.
 *
 * @param server Pointer to the server structure
 * @param index Index of the connecting client
 * @param buffer Connection identifier or team name from client
 */
void connection_command(server_t *server, int index, char *buffer)
{
    char response[BUFFER_SIZE];

    if (strcmp(buffer, GRAPHIC_NAME) == 0) {
        send_gui(server, index, buffer);
        return;
    } else if (has_team_name(server, buffer)) {
        return send_ai(server, index, buffer, response) == ERROR
            ? send_code(server->clients[index]->fd, "ko\n")
            : send_code(server->clients[index]->fd, response);
    } else {
        return send_code(server->clients[index]->fd, "ko\n");
    }
}
