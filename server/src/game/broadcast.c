/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** broadcast.c
*/

#include "macro.h"
#include "server.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
int calculate_shortest_distance_component(int coord1, int coord2, int map_size)
{
    int direct = ABS(coord2 - coord1);
    int wrapped = map_size - direct;

    if (direct < wrapped) {
        return (coord2 - coord1);
    } else if (coord2 > coord1) {
        return -(wrapped);
    } else {
        return wrapped;
    }
}

static
int calcalute_direction_tile(server_t *server, client_data_t *emitter,
    client_data_t *client)
{
    int dx = 0;
    int dy = 0;
    double angle_deg = 0;
    int tile = 0;

    if (client->x == emitter->x && client->y == emitter->y) {
        return 0;
    }
    dx = calculate_shortest_distance_component(client->x, emitter->x,
        server->params.width);
    dy = calculate_shortest_distance_component(client->y, emitter->y,
        server->params.height);
    angle_deg = atan2(dy, dx);
    if (angle_deg < 0)
        angle_deg += 360.;
    angle_deg = fmod(90. - angle_deg + 360., 360.);
    tile = ((int)(angle_deg + 22.5) / 45) + 1;
    return tile;
}

static
void transmit_sound(server_t *server, client_data_t *emitter,
    sound_result_t *results)
{
    client_data_t *client = NULL;

    if (!server || !emitter || !results)
        return;
    for (int i = 1; i < server->nfds; i++) {
        client = &server->clients[i]->data;
        if (!client || client->is_graphic) {
            results[i].received = false;
            continue;
        }
        results[i].direction_tile = calcalute_direction_tile(server, emitter,
            client);
        results[i].received = true;
    }
}

static
void send_broadcast_to_player(server_t *server, request_t *request,
    sound_result_t *results, const char *message)
{
    client_t *receiver = NULL;
    char sound_message[BUFFER_SIZE] = {0};

    for (int i = 1; i < server->nfds; i++) {
        if (!results[i].received ||
            server->clients[i]->fd == request->client->fd)
            continue;
        memset(sound_message, 0, BUFFER_SIZE);
        receiver = server->clients[i];
        if (!receiver || receiver->data.is_graphic)
            continue;
        snprintf(sound_message, sizeof(sound_message),
                "message %d, %s", results[i].direction_tile, message);
        send_code(receiver->fd, sound_message);
    }
}

static
void client_broadcast_sound(server_t *server, request_t *request,
    const char *message)
{
    sound_result_t *results = NULL;

    if (!server || !server->clients || !request || !request->client ||
        request->client->data.is_graphic)
        return;
    results = calloc(server->nfds, sizeof(sound_result_t));
    if (!results)
        return;
    transmit_sound(server, &request->client->data, results);
    send_broadcast_to_player(server, request, results, message);
    free(results);
}

int handle_broadcast(server_t *server, response_t *response,
    request_t *request)
{
    client_t *client = request->client;
    char *broadcast_text = NULL;

    if (!server || !response || !request)
        return ERROR;
    if (!client || client->data.is_graphic || client->data.is_busy)
        return ERROR;
    broadcast_text = get_broadcast_text(request->request);
    client_broadcast_sound(server, request, broadcast_text);
    free(broadcast_text);
    sprintf(response->response, "ok");
    return SUCCESS;
}
