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

typedef struct flags_s {
    char *params_name;
    int (*func)(params_t *params, char **av, size_t *av_idx);
} flags_t;

int help_flag(void);
int check_port(params_t *params, char **av, size_t *av_idx);
int check_width(params_t *params, char **av, size_t *av_idx);
int check_height(params_t *params, char **av, size_t *av_idx);
int check_teams_names(params_t *params, char **av, size_t *av_idx);
int check_clients_nb(params_t *params, char **av, size_t *av_idx);
int check_freq(params_t *params, char **av, size_t *av_idx);

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
