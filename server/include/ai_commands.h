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
int handle_broadcast(server_t *server, response_t *response,
    request_t *request);
int move_forward(server_t *server, response_t *response, request_t *request);
int rotate_right(server_t *server, response_t *response, request_t *request);
int rotate_left(server_t *server, response_t *response, request_t *request);
int handle_connect_nbr(server_t *server, response_t *response,
    request_t *request);
int fork_player(server_t *server, response_t *response, request_t *request);
int handle_set(server_t *server, response_t *response, request_t *request);
int handle_take(server_t *server, response_t *response, request_t *request);
int handle_oncoming_incantation(server_t *server, response_t *response,
    request_t *request);
int handle_eject(server_t *server, response_t *response, request_t *request);

const ai_commands_t tab_ai_commands[] = {
    {"Inventory", &handle_inventory},
    {"Look", &handle_look},
    {"Broadcast", &handle_broadcast},
    {"Forward", &move_forward},
    {"Right", &rotate_right},
    {"Left", &rotate_left},
    {"Connect_nbr", &handle_connect_nbr},
    {"Fork", &fork_player},
    {"Incantation", &handle_oncoming_incantation},
    {"Take", &handle_take},
    {"Set", &handle_set},
    {"Eject", &handle_eject},
    {NULL, NULL}
};

typedef struct action_duration_s {
    const char *command;
    int duration;
} action_duration_t;

const action_duration_t tab_action_durations[] = {
    {"Forward", FORWARD_TIME},
    {"Right", RIGHT_TIME},
    {"Left", LEFT_TIME},
    {"Look", LOOK_TIME},
    {"Inventory", INVENTORY_TIME},
    {"Connect_nbr", CONNECT_NBR_TIME},
    {"Broadcast", BROADCAST_TIME},
    {"Fork", FORK_TIME},
    {"Eject", EJECT_TIME},
    {"Take", TAKE_TIME},
    {"Set", SET_TIME},
    {"Incantation", INCANTATION_TIME},
    {NULL, 0}
};

#endif /* AI_COMMANDS_H_ */
