/*
** EPITECH PROJECT, 2025
** server
** File description:
** inventory.h
*/

#ifndef INVENTORY_H_
    #define INVENTORY_H_

/**
 * @brief Structure representing an inventory of resources
 *
 * Contains quantities of all available resources in the Zappy game,
 * including food and the six types of stones needed for incantations.
 */
typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

#endif /* INVENTORY_H_ */
