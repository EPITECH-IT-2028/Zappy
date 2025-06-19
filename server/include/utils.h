/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils.h
*/

#ifndef UTILS_H_
    #define UTILS_H_

    #include "server.h"

void send_code(int fd, const char *msg);

void remove_newline(char *buffer);

int find_team_index(server_t *server, const char *team_name);

client_t *find_ai_by_id(server_t *server, unsigned int player_id);

char *get_text_in_commands(const char *raw_broadcast, unsigned long length);

#endif /* UTILS_H_ */
