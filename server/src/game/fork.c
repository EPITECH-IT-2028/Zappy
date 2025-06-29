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
    int x = request->client->data.x;
    int y = request->client->data.y;
    map_t *tile = &server->map[x][y];
    egg_t *egg = create_egg(server->egg_ids,
        (egg_args_t){x, y, request->client->data.id,
        request->client->data.team_id});

    if (!egg)
        return ERROR;
    if (place_egg(tile, egg) == ERROR)
        return ERROR;
    send_pfk(server, request->client);
    send_enw(server, &tile->eggs[tile->eggs_count - 1]);
    server->egg_ids++;
    add_buffer_to_response("ok", &response->response, &response->size);
    return SUCCESS;
}
