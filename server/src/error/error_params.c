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

static
void print_error_message(void)
{
    fprintf(stderr, "Error: Invalid number of parameters.\n"
            "You need to have: -p, " "-x, -y, -n, -c, -f flags.\n"
            "Check with ./zappy_server -h to "
            "understand how to use it.\n");
}

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
