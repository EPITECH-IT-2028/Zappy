/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils.h
*/

#ifndef UTILS_H_
    #define UTILS_H_

    #include "server.h"

/**
 * @brief Send a message to a client via file descriptor
 * @param fd File descriptor of the client connection
 * @param msg Message string to send to the client
 */
void send_code(int fd, const char *msg);

/**
 * @brief Remove newline characters from a buffer
 * @param buffer String buffer to process for newline removal
 */
void remove_newline(char *buffer);

/**
 * @brief Find the index of a team by name
 * @param server Pointer to the server structure
 * @param team_name Name of the team to search for
 * @return Team index if found, ERROR otherwise
 */
int find_team_index(server_t *server, const char *team_name);

/**
 * @brief Find an AI client by player ID
 * @param server Pointer to the server structure
 * @param player_id Unique identifier of the player to find
 * @return Pointer to client structure if found, NULL otherwise
 */
client_t *find_ai_by_id(server_t *server, unsigned int player_id);

/**
 * @brief Extract text content from broadcast command
 * @param raw_broadcast Raw broadcast command string
 * @param length Length of the command string
 * @return Extracted text content or NULL on failure
 */
char *get_text_in_commands(const char *raw_broadcast, unsigned long length);

#endif /* UTILS_H_ */
