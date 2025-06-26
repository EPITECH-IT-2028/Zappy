/*
** EPITECH PROJECT, 2025
** server
** File description:
** init_density.c
*/

#include "server.h"

int init_density(server_t *server, inventory_t *density)
{
    if (!server || !density)
        return ERROR;
    density->food = server->params.width * server->params.height * FOOD;
    density->linemate = server->params.width * server->params.height
        * LINEMATE;
    density->deraumere = server->params.width * server->params.height
        * DERAUMERE;
    density->sibur = server->params.width * server->params.height * SIBUR;
    density->mendiane = server->params.width * server->params.height
        * MENDIANE;
    density->phiras = server->params.width * server->params.height * PHIRAS;
    density->thystame = server->params.width * server->params.height
        * THYSTAME;
    return SUCCESS;
}
