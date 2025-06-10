/*
** EPITECH PROJECT, 2025
** server
** File description:
** timer_handling.c
*/


#include "macro.h"
#include "server.h"
#include <time.h>

static
struct timespec calculate_action_duration(int action_units, int frequency)
{
    long total_ns = (long)action_units * NANOSECONDS_PER_SECOND / frequency;
    struct timespec duration = {
        .tv_sec = total_ns / NANOSECONDS_PER_SECOND,
        .tv_nsec = total_ns % NANOSECONDS_PER_SECOND
    };

    return duration;
}

static
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

struct timespec get_action_end_time(server_t *server, int action_duration)
{
    struct timespec current_time;
    struct timespec duration;

    duration = (calculate_action_duration(action_duration,
        server->params.frequence));
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return timespec_add(&current_time, &duration);
}

long long get_current_timer_units(server_t *server)
{
    struct timespec current_time;
    double elapsed_seconds = 0.;

    clock_gettime(CLOCK_MONOTONIC, &current_time);
    elapsed_seconds = (current_time.tv_sec - server->server_timer.tv_sec) +
        (current_time.tv_nsec - server->server_timer.tv_nsec)
        / (double)NANOSECONDS_PER_SECOND;
    return (long long)(elapsed_seconds * server->params.frequence);
}

bool has_time_passed(server_t *server, long long start_timer_units,
    int duration)
{
    long long current_units = get_current_timer_units(server);

    return (current_units - start_timer_units) >= duration;
}
