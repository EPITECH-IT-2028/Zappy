/*
** EPITECH PROJECT, 2025
** server
** File description:
** timer_handling.c
*/


#include "macro.h"
#include "server.h"
#include <time.h>

struct timespec get_action_end_time(server_t *server, int action_duration)
{
    struct timespec current_time;
    struct timespec duration;

    duration = (calculate_action_duration(action_duration,
        server->params.frequence));
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return timespec_add(&current_time, &duration);
}

long long get_current_timer_units(server_t *server, struct timespec *timer)
{
    struct timespec current_time;
    double elapsed_seconds = 0.;

    if (clock_gettime(CLOCK_MONOTONIC, &current_time) != 0)
        return 0;
    elapsed_seconds = (current_time.tv_sec - timer->tv_sec) +
        (current_time.tv_nsec - timer->tv_nsec)
        / (double)NANOSECONDS_PER_SECOND;
    return (long long)(elapsed_seconds * server->params.frequence);
}

bool has_time_passed(server_t *server, long long start_timer_units,
    int duration, struct timespec *timer)
{
    long long current_units = get_current_timer_units(server, timer);

    return (current_units - start_timer_units) >= duration;
}
