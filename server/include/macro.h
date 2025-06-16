/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** macro.h
*/

#ifndef MACRO_H_
    #define MACRO_H_

    #define NFDS_REALLOC_NUMBER 1
    #define ERROR 84
    #define SUCCESS 0
    #define MIN_PARAMS 14
    #define SERVER_INDEX 0
    #define REMOVE_USELESS_COMMA 1
    #define MIN_X 10
    #define MIN_Y 10
    #define MIN_CLIENT 1
    #define GRAPHIC_NAME "GRAPHIC"
    #define BUFFER_SIZE 2048
    #define QUEUE_MAX_SIZE 100
    #define MAX_DIRECTION 4
    #define DEFAULT_VISION_DEPTH 2
    #define ADDITIONAL_VISION 1
    #define UNASSIGNED_PLAYER_ID -1
    #define NANOSECONDS_PER_SECOND 1000000000L
    #define WORD_BROADCAST_LENGTH (sizeof("Broadcast ") - 1)
    #define ABS(x) (((x) < 0) ? - (x) : (x))
    #define FULL_CIRCLE_DEG 360.0
    #define HALF_CIRCLE_DEG 180.0
    #define QUARTER_CIRCLE_DEG 90.0
    #define NUM_DIRECTIONS 8
    #define DEGREES_PER_DIRECTION (FULL_CIRCLE_DEG / NUM_DIRECTIONS)
    #define DIRECTION_TOLERANCE (DEGREES_PER_DIRECTION / 2.0)

    #define FOOD 0.5f
    #define LINEMATE 0.3f
    #define DERAUMERE 0.15f
    #define SIBUR 0.10f
    #define MENDIANE 0.10f
    #define PHIRAS 0.08f
    #define THYSTAME 0.05f
    #define TOTAL_RESOURCES 7

    #define FORWARD_TIME 7
    #define RIGHT_TIME 7
    #define LEFT_TIME 7
    #define LOOK_TIME 7
    #define INVENTORY_TIME 1
    #define CONNECT_NBR_TIME 0
    #define BROADCAST_TIME 0
    #define FORK_TIME 42
    #define EJECT_TIME 7
    #define DEATH_TIME 7
    #define TAKE_TIME 7
    #define SET_TIME 7
    #define INCANTATION_TIME 300

    #define FOOD_DURATION 126

#endif /* MACRO_H_ */
