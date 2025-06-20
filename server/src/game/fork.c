/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** fork.c
*/

#include "macro.h"
#include "server.h"

int fork_player(server_t *server, response_t *response, request_t *request)
{
    int player_id = request->client->data.id;
    int x = request->client->data.x;
    int y = request->client->data.y;
    map_t *tile = &server->map[x][y];
    egg_t *egg = create_egg(server->egg_ids, x, y, player_id);

    if (!egg)
        return ERROR;
    if (place_egg(tile, egg) == ERROR)
        return ERROR;
    send_pfk(server, request->client);
    send_enw(server, &tile->eggs[tile->eggs_count - 1]);
    server->egg_ids++;
    sprintf(response->response, "ok");
    response->client->data.is_busy = true;
    response->client->data.action_end_time =
        get_action_end_time(server, FORK_TIME);
    return SUCCESS;
}
