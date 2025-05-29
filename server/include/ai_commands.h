/*
** EPITECH PROJECT, 2025
** server
** File description:
** game.h
*/

#ifndef GAME_H_
    #define GAME_H_

    #include "server.h"

typedef struct ai_commands_s {
    char *request;
    int (*func)(response_t *response, request_t *request);
} ai_commands_t;

int handle_inventory(response_t *response, request_t *request);

const ai_commands_t tab_ai_commands[] = {
    {"Inventory", &handle_inventory},
};

#endif /* GAME_H_ */
