/*
** EPITECH PROJECT, 2025
** server
** File description:
** time.c
*/

#include "server.h"

struct timespec timespec_add(struct timespec *start, struct timespec *duration)
{
    struct timespec result;

    result.tv_sec = start->tv_sec + duration->tv_sec;
    result.tv_nsec = start->tv_nsec + duration->tv_nsec;
    if (result.tv_nsec >= NANOSECONDS_PER_SECOND) {
        result.tv_sec++;
        result.tv_nsec -= NANOSECONDS_PER_SECOND;
    }
    return result;
}

struct timespec calculate_action_duration(int action_units, int frequency)
{
    long total_ns = (long)action_units * NANOSECONDS_PER_SECOND / frequency;
    struct timespec duration = {
        .tv_sec = total_ns / NANOSECONDS_PER_SECOND,
        .tv_nsec = total_ns % NANOSECONDS_PER_SECOND
    };

    return duration;
}

void check_time_events(server_t *server)
{
    if (has_time_passed(server, server->server_timer_count,
        FOOD_DURATION, &server->server_timer)) {
        remove_food(server);
        server->server_timer_count =
            get_current_timer_units(server, &server->server_timer);
    }
    if (has_time_passed(server, server->density_timer_count,
        DENSITY_DURATION, &server->density_timer)) {
        respawn_resources(server);
        server->density_timer_count =
            get_current_timer_units(server, &server->density_timer);
    }
}
