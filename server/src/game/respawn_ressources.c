/*
** EPITECH PROJECT, 2025
** server
** File description:
** respawn_ressources.c
*/

#include "macro.h"
#include "server.h"

void respawn_ressources(server_t *server)
{
    const int map_size = server->params.width * server->params.height;
    const float table[TOTAL_RESOURCES] = {
        FOOD, LINEMATE, DERAUMERE, SIBUR, MENDIANE, PHIRAS, THYSTAME
    };

    if (server->density_ressources.food > map_size * table[0])
        add_resources(server, map_size * table[0], 0, server->density_ressources.food);
    if (server->density_ressources.linemate > map_size * table[1])
        add_resources(server, map_size * table[1], 1, server->density_ressources.linemate);
    if (server->density_ressources.deraumere > map_size * table[2])
        add_resources(server, map_size * table[2], 2, server->density_ressources.deraumere);
    if (server->density_ressources.sibur > map_size * table[3])
        add_resources(server, map_size * table[3], 3, server->density_ressources.sibur);
    if (server->density_ressources.mendiane > map_size * table[4])
        add_resources(server, map_size * table[4], 4, server->density_ressources.mendiane);
    if (server->density_ressources.phiras > map_size * table[5])
        add_resources(server, map_size * table[5], 5, server->density_ressources.phiras);
    if (server->density_ressources.thystame > map_size * table[6])
        add_resources(server, map_size * table[6], 6, server->density_ressources.thystame);
}
