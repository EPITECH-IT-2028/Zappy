/*
** EPITECH PROJECT, 2025
** server
** File description:
** game.h
*/

#ifndef AI_COMMANDS_H_
    #define AI_COMMANDS_H_

    #include "server.h"

/**
 * @brief Structure for AI command mapping
 *
 * Maps command strings to their corresponding handler functions
 * for processing AI player requests.
 */
typedef struct ai_commands_s {
    char *request;
    int (*func)(server_t *server, response_t *response, request_t *request);
} ai_commands_t;

/**
 * @brief Handle inventory command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_inventory(server_t *, response_t *, request_t *);

/**
 * @brief Handle look command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_look(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle broadcast command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_broadcast(server_t *server, response_t *response,
    request_t *request);

/**
 * @brief Handle forward movement command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int move_forward(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle right rotation command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int rotate_right(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle left rotation command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int rotate_left(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle connect_nbr command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_connect_nbr(server_t *server, response_t *response,
    request_t *request);

/**
 * @brief Handle fork command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int fork_player(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle set command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_set(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle take command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_take(server_t *server, response_t *response, request_t *request);

/**
 * @brief Handle incantation command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_oncoming_incantation(server_t *server, response_t *response,
    request_t *request);

/**
 * @brief Handle eject command from AI player
 * @param server Pointer to server structure
 * @param response Pointer to response structure
 * @param request Pointer to request structure
 * @return SUCCESS or ERROR code
 */
int handle_eject(server_t *server, response_t *response, request_t *request);

/**
 * @brief Array mapping AI command strings to handler functions
 *
 * This constant array provides the mapping between command names
 * and their corresponding handler functions for AI players.
 */
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

/**
 * @brief Structure for action duration mapping
 *
 * Maps command names to their execution durations in game time units
 * for proper timing of AI player actions.
 */

typedef struct action_duration_s {
    const char *command;
    int duration;
} action_duration_t;

/**
 * @brief Array mapping command names to their execution durations
 *
 * This constant array defines how long each AI command takes to
 * execute in game time units.
 */
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
