/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_setups.c
*/

#include "server.h"
#include <stdint.h>
#include <stdlib.h>

static
int add_player_to_group(client_data_t *client, client_t *player,
    uint8_t *nbr_of_incantators)
{
    client_t **new_group = NULL;

    new_group = realloc(client->incantation.client_group,
        sizeof(client_t *) * (*nbr_of_incantators + 1));
    if (!new_group)
        return ERROR;
    client->incantation.client_group = new_group;
    client->incantation.client_group[*nbr_of_incantators] = player;
    *nbr_of_incantators += 1;
    return SUCCESS;
}

static
int check_add_player_to_group(client_t *main_client, map_t *unit_space,
    uint8_t *nbr_of_incantators, int i)
{
    client_data_t *client = &main_client->data;

    if (unit_space->players[i] != main_client &&
        unit_space->players[i]->data.level == client->level) {
        if (add_player_to_group(client, unit_space->players[i],
            nbr_of_incantators) == ERROR) {
            return ERROR;
        }
    }
    return SUCCESS;
}

static
int init_client_group(client_t *main_client, map_t *unit_space,
    uint8_t *nbr_of_incantators)
{
    for (int i = 0; i < unit_space->nbr_of_players; i++) {
        if (check_add_player_to_group(main_client, unit_space,
            nbr_of_incantators, i) == ERROR)
            return ERROR;
    }
    return SUCCESS;
}

/**
 * Add players with matching level to incantation group
 */
uint8_t build_incantation_group(client_t *main_client, map_t *unit_space)
{
    uint8_t nbr_of_incantators = 0;
    client_data_t *client = &main_client->data;

    client->incantation.client_group = malloc(sizeof(client_t *));
    if (!client->incantation.client_group)
        return ERROR;
    client->incantation.client_group[nbr_of_incantators] = main_client;
    nbr_of_incantators += 1;
    client->incantation.client_group = realloc(
        client->incantation.client_group,
        sizeof(client_t *) * (nbr_of_incantators + 1));
    if (init_client_group(main_client, unit_space, &nbr_of_incantators)
        == ERROR) {
        return ERROR;
    }
    client->incantation.client_group[nbr_of_incantators] = NULL;
    return nbr_of_incantators;
}

/**
 * Set incantation data for main incantator
 */
void setup_main_incantator(server_t *server, client_data_t *client)
{
    client->incantation.x = client->x;
    client->incantation.y = client->y;
    client->incantation.is_incantating = true;
    client->incantation.id_incantator = client->id;
    client->is_busy = true;
    client->action_end_time = get_action_end_time(server, INCANTATION_TIME);
}

/**
 * Set incantation data for group members
 */
void setup_group_members(client_data_t *client, uint8_t nbr_of_incantators)
{
    for (int i = 0; i < nbr_of_incantators; i++) {
        client->incantation.client_group[i]->data.incantation.x = client->x;
        client->incantation.client_group[i]->data.incantation.y = client->y;
        client->incantation.client_group[i]->data.incantation.is_incantating =
            true;
        client->incantation.client_group[i]->data.incantation.id_incantator =
            client->id;
    }
}

void init_incantation_state(incantation_t *inc)
{
    inc->is_incantating = false;
    inc->x = 0;
    inc->y = 0;
    inc->id_incantator = 0;
    inc->client_group = NULL;
    inc->incantation_success = false;
}
