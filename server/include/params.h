/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** params.h
*/

#ifndef PARAMS_H_
    #define PARAMS_H_

    #include "server.h"
    #include <stddef.h>

/**
 * @brief Structure for command-line flag mapping
 *
 * Maps command-line parameter names to their corresponding validation
 * functions for parsing server configuration options.
 */
typedef struct flags_s {
    char *params_name;
    int (*func)(params_t *params, char **av, size_t *av_idx);
} flags_t;

/**
 * @brief Array mapping command-line flags to validation functions
 *
 * This constant array defines the mapping between command-line flags
 * and their corresponding parameter validation functions.
 */
const flags_t tab_check_func[] = {
    {"-p", &check_port},
    {"-x", &check_width},
    {"-y", &check_height},
    {"-n", &check_teams_names},
    {"-c", &check_clients_nb},
    {"-f", &check_freq},
    {NULL, NULL}
};

#endif /* PARAMS_H_ */
