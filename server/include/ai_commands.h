/*
** EPITECH PROJECT, 2025
** server
** File description:
** game.h
*/

#ifndef AI_COMMANDS_H_
    #define AI_COMMANDS_H_

    #include "server.h"

typedef struct ai_commands_s {
    char *request;
    int (*func)(server_t *server, response_t *response, request_t *request);
} ai_commands_t;

int handle_inventory(server_t *, response_t *, request_t *);
int handle_look(server_t *server, response_t *response, request_t *request);
int move_forward(server_t *server, response_t *response, request_t *request);
int rotate_right(server_t *server, response_t *response, request_t *request);
int rotate_left(server_t *server, response_t *response, request_t *request);

const ai_commands_t tab_ai_commands[] = {
    {"Inventory", &handle_inventory},
    {"Look", &handle_look},
    {"Forward", &move_forward},
    {"Right", &rotate_right},
    {"Left", &rotate_left},
    {NULL, NULL}
};

#endif /* AI_COMMANDS_H_ */
