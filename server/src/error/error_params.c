/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** error_params.c
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "params.h"
#include "server.h"

static int verif_params(server_t *server, char **av, size_t *av_idx)
{
    int return_value = -1;

    for (size_t j = 0; tab_check_func[j].params_name != NULL; j++) {
        if (strcmp(av[*av_idx], tab_check_func[j].params_name) == 0) {
            *av_idx += 1;
            return_value = tab_check_func[j].func(server, av, av_idx);
        }
        if (return_value == ERROR) {
            fprintf(stderr, "Error: Invalid parameter '%s'\n", av[*av_idx]);
            return ERROR;
        }
    }
    return SUCCESS;
}

static int check_help_flag(int ac, char **av)
{
    for (size_t i = 1; i < (size_t) ac; i += 1) {
        if (strcmp("-h", av[i]) == 0 || strcmp("-help", av[i]) == 0) {
            help_flag();
            return SUCCESS;
        }
    }
    return ERROR;
}

int check_params(server_t *server, int ac, char **av)
{
    if (check_help_flag(ac, av) == SUCCESS)
        return ERROR;
    if (ac < MIN_PARAMS) {
        fprintf(stderr,
                "Error: Invalid number of parameters. You need to have at "
                "least 14 parameters.\nCheck with ./zappy_server -h to "
                "understand how to use it.\n");
        return ERROR;
    }
    for (size_t i = 1; i < (size_t)ac; i++) {
        if (verif_params(server, av, &i)) {
            return ERROR;
        }
    }
    return SUCCESS;
}
