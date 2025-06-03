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
    int (*func)(response_t *response, request_t *request);
} ai_commands_t;

int handle_inventory(response_t *response, request_t *request);

const ai_commands_t tab_ai_commands[] = {
    {"Inventory", &handle_inventory},
    {NULL, NULL}
};

#endif /* AI_COMMANDS_H_ */
