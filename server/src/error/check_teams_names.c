/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_teams_names.c
*/

#include <stddef.h>
#include <stdlib.h>
#include "macro.h"
#include <string.h>
#include "server.h"

/**
 * @brief Parse and store team names from command line arguments
 *
 * This function reads team names from command line arguments until
 * it encounters another flag, storing them in the parameters structure.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if team names parsed successfully, ERROR otherwise
 */
int check_teams_names(params_t *params, char **av, size_t *av_idx)
{
    if (!av || !av_idx)
        return ERROR;
    for (; av[*av_idx] != NULL; *av_idx += 1) {
        if (av[*av_idx][0] == '-')
            break;
        params->teams_names = realloc(
            params->teams_names,
            sizeof(char *) * (params->teams_count + 2));
        if (params->teams_names == NULL)
            return ERROR;
        params->teams_names[params->teams_count] = strdup(av[*av_idx]);
        if (params->teams_names[params->teams_count] == NULL)
            return ERROR;
        params->teams_names[params->teams_count + 1] = NULL;
        params->teams_count += 1;
    }
    return SUCCESS;
}
