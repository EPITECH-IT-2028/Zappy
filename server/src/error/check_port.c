/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_port.c
*/

#include "server.h"
#include <stddef.h>
#include <stdlib.h>
#include "macro.h"

/**
 * @brief Validate and set the server port parameter
 *
 * This function checks if the provided port number is within valid
 * range (1-65535) and sets it in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if valid port number, ERROR otherwise
 */
int check_port(params_t *params, char **av, size_t *av_idx)
{
    int port = atoi(av[*av_idx]);

    if (port <= 0 || port > 65535) {
        return ERROR;
    }
    *av_idx += 1;
    params->port = port;
    return SUCCESS;
}
