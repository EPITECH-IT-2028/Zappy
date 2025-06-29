/*
** EPITECH PROJECT, 2025
** server
** File description:
** incantation_setups.c
*/

#include "server.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Add a player to the incantation group
 *
 * This function reallocates memory to expand the incantation group
 * and adds a new player to the group array.
 *
 * @param client Pointer to the main client's data
 * @param player Pointer to the player to add to the group
 * @param nbr_of_incantators Pointer to the current group size counter
 * @return SUCCESS on successful addition, ERROR on failure
 */
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

/**
 * @brief Check if a player can be added to the incantation group
 *
 * This function verifies if a player has the same level as the main
 * client and adds them to the group if conditions are met.
 *
 * @param main_client Pointer to the main client initiating incantation
 * @param unit_space Pointer to the map tile containing players
 * @param nbr_of_incantators Pointer to the current group size counter
 * @param i Index of the player to check in the tile's player array
 * @return SUCCESS on successful check/addition, ERROR on failure
 */
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

/**
 * @brief Initialize the incantation group with eligible players
 *
 * This function iterates through all players on a tile and adds
 * those with matching levels to the incantation group.
 *
 * @param main_client Pointer to the main client initiating incantation
 * @param unit_space Pointer to the map tile containing players
 * @param nbr_of_incantators Pointer to the current group size counter
 * @return SUCCESS on successful group initialization, ERROR on failure
 */
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
 * @brief Build complete incantation group with matching level players
 *
 * This function creates and populates an incantation group by finding
 * all players on the same tile with the same level as the initiator.
 *
 * @param main_client Pointer to the client initiating the incantation
 * @param unit_space Pointer to the map tile to search for players
 * @return Number of players in the group, or ERROR on failure
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
 * @brief Set up incantation data for the main incantator
 *
 * This function initializes the incantation state for the client who
 * initiated the incantation, setting position, timing, and flags.
 *
 * @param server Pointer to the server structure for timing calculations
 * @param client Pointer to the main incantator's client data
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
 * @brief Set up incantation data for all group members
 *
 * This function initializes the incantation state for all members
 * of the incantation group, synchronizing their incantation data.
 *
 * @param client Pointer to the main incantator's client data
 * @param nbr_of_incantators Number of players in the incantation group
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

/**
 * @brief Initialize incantation structure to default values
 *
 * This function resets all fields of an incantation structure to
 * their default/initial state for clean initialization.
 *
 * @param inc Pointer to the incantation structure to initialize
 */
void init_incantation_state(incantation_t *inc)
{
    inc->is_incantating = false;
    inc->x = 0;
    inc->y = 0;
    inc->id_incantator = 0;
    inc->client_group = NULL;
    inc->incantation_success = false;
}
