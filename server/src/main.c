/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** main.c
*/

#include "server.h"

/**
 * @brief Main entry point of the Zappy server program
 *
 * This function serves as the entry point for the Zappy server,
 * forwarding command line arguments to the server function.
 *
 * @param ac Argument count from command line
 * @param av Argument vector containing command line arguments
 * @return Exit status from the server function
 */
int main(int ac, char **av)
{
    return server(ac, av);
}
