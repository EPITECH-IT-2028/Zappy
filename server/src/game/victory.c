/*
** EPITECH PROJECT, 2025
** server
** File description:
** victory.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Counts the number of level 8 players in each team
 * 
 * This function iterates through all connected clients and counts
 * how many players of each team have reached level 8.
 * 
 * @param server Server instance containing client and team data
 * @param counts Array to store the count for each team
 */
static
void count_levels_team(server_t *server, char *counts)
{
    client_t *client = NULL;
    int team_index = 0;

    for (int i = 1; i < server->nfds; i++) {
        if (!server->clients[i] || !server->clients[i]->connected)
            continue;
        client = server->clients[i];
        if (client->data.is_graphic || !client->data.team_name)
            continue;
        if (client->data.level == 8) {
            team_index = find_team_index(server, client->data.team_name);
            counts[team_index]++;
        }
    }
}

/**
 * @brief Checks if any team has won the game
 * 
 * Victory condition: A team wins when it has at least 6 players at level 8.
 * This function checks all teams and returns the winning team ID if found.
 * 
 * @param server Server instance containing game state
 * @return Team ID of winning team, or ERROR if no team has won yet
 */
int check_victory(server_t *server)
{
    char *counts = NULL;
    int winning_team = ERROR;
    
    if (!server || server->params.teams_count <= 0)
        return ERROR;
    counts = calloc(server->params.teams_count, sizeof(int));
    if (!counts)
        return ERROR;
    count_levels_team(server, counts);
    for (int i = 0; i < server->params.teams_count; i++) {
        if (counts[i] >= 6) {
            winning_team = i;
            printf("Team '%s' has won the game\n",
                   server->teams[i].name);
            server->running = false;
            break;
        }
    }
    free(counts);
    return winning_team;
}
