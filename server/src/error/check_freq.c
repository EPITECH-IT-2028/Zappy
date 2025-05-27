/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** check_freq.c
*/

#include <stdlib.h>
#include "server.h"
#include "macro.h"

int check_freq(params_t *params, char **av, size_t *av_idx)
{
    int freq = atoi(av[*av_idx]);

    if (freq <= 0) {
        return ERROR;
    }
    *av_idx += 1;
    params->frequence = freq;
    return SUCCESS;
}
