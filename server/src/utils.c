/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils.c
*/

#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <stdio.h>
#include "macro.h"

/**
 * @brief Sends a formatted message to a file descriptor
 * @param fd File descriptor to write to
 * @param msg Message string to be sent
 *
 * Copies the message to a buffer and writes it to the specified
 * file descriptor.
 * Prints an error message if the write operation fails.
 */
void send_code(int fd, const char *msg)
{
    char buffer[2048];

    snprintf(buffer, sizeof(buffer), "%s", msg);
    if (write(fd, buffer, strlen(buffer)) < 0)
        perror("write failed");
}

/**
 * @brief Removes trailing newline character from a string
 * @param buffer String to be modified
 *
 * Checks if the last character of the string is a newline character ('\n')
 * and replaces it with a null terminator if present.
 */
void remove_newline(char *buffer)
{
    int len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

/**
 * @brief Finds the index of a team in the server's team array by its name
 * @param server Pointer to the server structure
 * @param team_name Name of the team to find
 *
 * @return Index of the team if found, ERROR (84) if not found or parameters
 * are invalid
 */
int find_team_index(server_t *server, const char *team_name)
{
    if (!server || !team_name || !server->teams)
        return ERROR;
    for (int i = 0; i < server->params.teams_count; i++) {
        if (strcmp(server->teams[i].name, team_name) == 0)
            return i;
    }
    return ERROR;
}

/**
 * Finds the AI client by its player ID.
 * The player ID is the index of the AI in the list of clients,
 * excluding graphic clients and clients without a team name.
 *
 * @param server The server instance.
 * @param player_id The ID of the AI player to find.
 * @return The client_t pointer if found, NULL otherwise.
 */
client_t *find_ai_by_id(server_t *server, unsigned int player_id)
{
    unsigned int cmpt = 0;

    for (int i = 1; i < server->nfds; i++) {
        if (server->clients[i] == NULL)
            continue;
        if (server->clients[i]->data.is_graphic)
            continue;
        if (server->clients[i]->data.team_name == NULL)
            continue;
        if (cmpt == player_id) {
            return server->clients[i];
        }
        cmpt++;
    }
    return NULL;
}

/**
 * @brief Extracts the actual message content from a raw broadcast command
 *
 * This function takes a raw broadcast command string
 * (format: "Broadcast [message]")
 * and extracts only the message part, removing the command prefix and any
 * leading spaces.
 *
 * @param raw_broadcast The raw broadcast command string to process
 * @return A newly allocated string containing only the message content
 *         - Returns NULL if input is NULL
 *         - Returns empty string if input has no message content
 *         - The caller is responsible for freeing the returned string
 */
char *get_text_in_commands(const char *raw_message, unsigned long length)
{
    char *text = NULL;
    const char *start = NULL;

    if (!raw_message)
        return NULL;
    if (strlen(raw_message) <= length)
        return strdup("");
    start = raw_message + length;
    while (*start == ' ')
        ++start;
    if (*start == '\0')
        return strdup("");
    text = strdup(start);
    return text;
}
