/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_clients_nb.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "server.h"
#include "macro.h"

/**
 * @brief Validate and set the number of clients per team parameter
 *
 * This function checks if the provided client number is valid
 * and sets it in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if valid client number, ERROR otherwise
 */
int check_clients_nb(params_t *params, char **av, size_t *av_idx)
{
    int client_nb = atoi(av[*av_idx]);

    if (client_nb == 0) {
        return ERROR;
    }
    if (client_nb < MIN_CLIENT) {
        return ERROR;
    }
    *av_idx += 1;
    params->client_per_team = client_nb;
    return SUCCESS;
}
