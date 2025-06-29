/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg.c
*/

#include "macro.h"
#include "server.h"
#include <stdlib.h>

/**
 * @brief Count the total number of eggs on the entire map
 *
 * This function iterates through all tiles on the map and sums up
 * the number of eggs present on each tile.
 *
 * @param server Pointer to the server structure containing the map
 * @return Total number of eggs on the map
 */
int count_total_eggs(server_t *server)
{
    int total_eggs = 0;

    for (int x = 0; x < server->params.width; x++) {
        for (int y = 0; y < server->params.height; y++)
            total_eggs += server->map[x][y].eggs_count;
    }
    return total_eggs;
}

/**
 * @brief Remove an egg from a tile at the specified index
 *
 * This function removes an egg from a tile's egg array by swapping
 * it with the last egg and reducing the array size.
 *
 * @param tile Pointer to the map tile containing the eggs
 * @param index Index of the egg to remove
 */
void remove_egg(map_t *tile, int index)
{
    egg_t *new_eggs = NULL;

    if (index < 0 || index >= tile->eggs_count)
        return;
    if (index != tile->eggs_count - 1)
        tile->eggs[index] = tile->eggs[tile->eggs_count - 1];
    tile->eggs_count--;
    if (tile->eggs_count > 0) {
        new_eggs = realloc(tile->eggs, sizeof(egg_t) * tile->eggs_count);
        if (new_eggs)
            tile->eggs = new_eggs;
    } else {
        free(tile->eggs);
        tile->eggs = NULL;
    }
}

/**
 * @brief Create a new egg with the given parameters
 *
 * This function allocates memory for a new egg structure and
 * initializes it with the provided arguments.
 *
 * @param id Unique identifier for the egg
 * @param args Structure containing egg creation parameters
 * @return Pointer to the newly created egg, or NULL on failure
 */
egg_t *create_egg(int id, egg_args_t args)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (!egg) {
        perror("malloc failed");
        return NULL;
    }
    egg->id = id;
    egg->x = args.x;
    egg->y = args.y;
    egg->player_id = args.player_id;
    egg->team_id = args.team_id;
    return egg;
}

/**
 * @brief Place an egg on a tile
 *
 * This function adds an egg to a tile's egg array by reallocating
 * memory and copying the egg data.
 *
 * @param tile Pointer to the map tile where the egg will be placed
 * @param egg Pointer to the egg to be placed
 * @return SUCCESS on successful placement, ERROR on failure
 */
int place_egg(map_t *tile, egg_t *egg)
{
    egg_t *new_eggs = realloc(tile->eggs, sizeof(egg_t) *
                        (tile->eggs_count + 1));

    if (!new_eggs) {
        perror("realloc failed");
        free(new_eggs);
        return ERROR;
    }
    tile->eggs = new_eggs;
    tile->eggs[tile->eggs_count] = *egg;
    tile->eggs_count++;
    free(egg);
    return SUCCESS;
}

/**
 * @brief Assign a client to an egg position and add them to the tile
 *
 * This function assigns a client to a specific egg's position,
 * adds the client to the tile's player list, and removes the egg.
 *
 * @param tile Pointer to the map tile containing the egg
 * @param client Pointer to the client to assign
 * @param target Index of the target egg
 * @return true on success, false on failure
 */
static
int assign_egg_position(map_t *tile, client_t *client, int target)
{
    client_t **new_players = NULL;

    if (target < 0 || target >= tile->eggs_count)
        return false;
    client->data.x = tile->eggs[target].x;
    client->data.y = tile->eggs[target].y;
    new_players = realloc(tile->players, sizeof(client_t *) *
        (tile->nbr_of_players + 1));
    if (!new_players)
        return false;
    tile->players = new_players;
    tile->players[tile->nbr_of_players] = client;
    tile->nbr_of_players++;
    remove_egg(tile, target);
    return true;
}

/**
 * @brief Assign client to egg position and send notification
 *
 * This function assigns a client to an egg position and sends
 * an egg broken (ebo) notification to the server.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to assign
 * @param tile Pointer to the map tile containing the egg
 * @param target Index of the target egg
 * @return SUCCESS on success, ERROR on failure
 */
static
int assign_and_send(server_t *server, client_t *client,
    map_t *tile, int target)
{
    egg_t target_egg;

    target_egg = tile->eggs[target];
    if (!assign_egg_position(tile, client, target))
        return ERROR;
    send_ebo(server, &target_egg);
    return SUCCESS;
}

/**
 * @brief Loop through eggs on a tile to find a matching team egg
 *
 * This function searches through all eggs on a tile to find one
 * belonging to the specified team and assigns it to the client.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to assign
 * @param tile Pointer to the map tile to search
 * @param team_id ID of the team to match
 * @return SUCCESS if egg found and assigned, ERROR otherwise
 */
static
int loop_eggs(server_t *server, client_t *client, map_t *tile, int team_id)
{
    for (int j = 0; j < tile->eggs_count; j++) {
        if (tile->eggs[j].team_id == team_id)
            return assign_and_send(server, client, tile, j);
    }
    return ERROR;
}

/**
 * @brief Find and assign an egg belonging to a specific team
 *
 * This function searches the entire map for an egg belonging to
 * the specified team and assigns it to the client.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to assign
 * @param team_id ID of the team to search for
 * @return SUCCESS if team egg found and assigned, ERROR otherwise
 */
static
int find_and_assign_team_egg(server_t *server, client_t *client, int team_id)
{
    int nb_tiles = server->params.width * server->params.height;
    int x = 0;
    int y = 0;

    for (int i = 0; i < nb_tiles; i++) {
        x = i / server->params.height;
        y = i % server->params.height;
        if (loop_eggs(server, client, &server->map[x][y], team_id) == SUCCESS)
            return SUCCESS;
    }
    return ERROR;
}

/**
 * @brief Assign a random egg position to a client
 *
 * This function attempts to assign an egg to a client, first trying
 * to find an egg from their team, then falling back to unassigned eggs.
 *
 * @param server Pointer to the server structure
 * @param client Pointer to the client to assign an egg position
 * @return SUCCESS if egg assigned, ERROR if no suitable egg found
 */
int assign_random_egg_position(server_t *server, client_t *client)
{
    if (find_and_assign_team_egg(server, client,
        client->data.team_id) == SUCCESS)
        return SUCCESS;
    if (find_and_assign_team_egg(server, client,
        UNASSIGNED_PLAYER_ID) == SUCCESS)
        return SUCCESS;
    return ERROR;
}
