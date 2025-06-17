/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_condition.h
*/

#ifndef INCANTATION_CONDITION_H_
    #define INCANTATION_CONDITION_H_

    #include <stdint.h>

typedef struct incantation_condition_s {
    uint8_t player;
    uint8_t linemate;
    uint8_t deraumere;
    uint8_t sibur;
    uint8_t mendiane;
    uint8_t phiras;
    uint8_t thystame;
} incantation_condition_t;

const incantation_condition_t tab_incantation[] = {
    {1, 1, 0, 0, 0, 0, 0},  // Level 1->2
    {2, 1, 1, 1, 0, 0, 0},  // Level 2->3
    {2, 2, 0, 1, 0, 2, 0},  // Level 3->4
    {4, 1, 1, 2, 0, 1, 0},  // Level 4->5
    {4, 1, 2, 1, 3, 0, 0},  // Level 5->6
    {6, 1, 2, 3, 0, 1, 0},  // Level 6->7
    {6, 2, 2, 2, 2, 2, 1},  // Level 7->8
};

#endif /* INCANTATION_CONDITION_H_ */
