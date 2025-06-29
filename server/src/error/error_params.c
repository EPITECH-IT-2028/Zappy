/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** error_params.c
*/

#include <stddef.h>
#include "macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "params.h"
#include "server.h"

/**
 * @brief Verify and process a single parameter from command line
 *
 * This function matches command line arguments with known parameters
 * and calls the appropriate validation function.
 *
 * @param params Pointer to the parameters structure to update
 * @param av Array of command line arguments
 * @param av_idx Pointer to current argument index
 * @return SUCCESS if parameter processed successfully, ERROR otherwise
 */
static
int verif_params(params_t *params, char **av, size_t *av_idx)
{
    int return_value = -1;

    for (size_t j = 0; tab_check_func[j].params_name != NULL; j++) {
        if (strcmp(av[*av_idx], tab_check_func[j].params_name) == 0) {
            *av_idx += 1;
            return_value = tab_check_func[j].func(params, av, av_idx);
        }
        if (return_value == ERROR) {
            fprintf(stderr, "Error: Invalid parameter '%s'\n", av[*av_idx]);
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
 * @brief Check if help flag is present in command line arguments
 *
 * This function searches for help flags (-h or -help) in the command
 * line arguments and displays usage information if found.
 *
 * @param ac Argument count
 * @param av Array of command line arguments
 * @return SUCCESS if help flag found, ERROR otherwise
 */
static
int check_help_flag(int ac, char **av)
{
    for (size_t i = 1; i < (size_t)ac; i += 1) {
        if (strcmp("-h", av[i]) == 0 || strcmp("-help", av[i]) == 0) {
            help_flag();
            return SUCCESS;
        }
    }
    return ERROR;
}

/**
 * @brief Check if all required server parameters are set
 *
 * This function verifies that all mandatory parameters have been
 * provided and are not set to their default (-1) values.
 *
 * @param params Pointer to the parameters structure to validate
 * @return SUCCESS if all parameters are set, ERROR if any missing
 */
static
int check_server_all_set(params_t *params)
{
    if (params->port == -1)
        return ERROR;
    if (params->width == -1)
        return ERROR;
    if (params->height == -1)
        return ERROR;
    if (params->frequence == -1)
        return ERROR;
    if (params->client_per_team == -1)
        return ERROR;
    if (params->teams_names == NULL)
        return ERROR;
    if (params->teams_count == 0)
        return ERROR;
    return SUCCESS;
}

/**
 * @brief Print error message for invalid parameters
 *
 * This function displays a detailed error message explaining which
 * parameters are required and how to get help.
 */
static
void print_error_message(void)
{
    fprintf(stderr, "Error: Invalid number of parameters.\n"
            "You need to have: -p, " "-x, -y, -n, -c, -f flags.\n"
            "Check with ./zappy_server -h to "
            "understand how to use it.\n");
}

/**
 * @brief Main parameter validation function
 *
 * This function orchestrates the complete parameter validation process,
 * checking help flags, minimum arguments, and all required parameters.
 *
 * @param params Pointer to the parameters structure to populate
 * @param ac Argument count
 * @param av Array of command line arguments
 * @return SUCCESS if all parameters are valid, ERROR otherwise
 */
int check_params(params_t *params, int ac, char **av)
{
    init_params(params);
    if (check_help_flag(ac, av) == SUCCESS)
        return ERROR;
    if (ac < MIN_PARAMS) {
        print_error_message();
        return ERROR;
    }
    for (size_t i = 1; i < (size_t)ac; i++) {
        if (verif_params(params, av, &i)) {
            return ERROR;
        }
    }
    if (check_server_all_set(params)) {
        print_error_message();
        return ERROR;
    }
    return SUCCESS;
}
