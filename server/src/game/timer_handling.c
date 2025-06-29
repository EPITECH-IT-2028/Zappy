/*
** EPITECH PROJECT, 2025
** server
** File description:
** timer_handling.c
*/


#include "macro.h"
#include "server.h"
#include <time.h>

/**
 * @brief Calculates the end time for an action based on its duration
 *
 * This function determines when an action should complete by adding
 * the action's duration (converted to real time) to the current time.
 * It uses the server frequency to convert game time to real time.
 *
 * @param server Pointer to the server structure for frequency info
 * @param action_duration Duration of the action in game time units
 * @return timespec structure representing when the action will end
 */
struct timespec get_action_end_time(server_t *server, int action_duration)
{
    struct timespec current_time;
    struct timespec duration;

    duration = (calculate_action_duration(action_duration,
        server->params.frequence));
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return timespec_add(&current_time, &duration);
}

/**
 * @brief Converts elapsed real time to game timer units
 *
 * This function calculates how many game time units have passed since
 * a reference timer was set. It uses the server frequency to convert
 * real elapsed time into game time units.
 *
 * @param server Pointer to the server structure for frequency info
 * @param timer Pointer to the reference timespec timer
 * @return Number of game time units elapsed since the timer was set
 */
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

/**
 * @brief Checks if a specified duration has elapsed since a timer started
 *
 * This function determines whether enough game time units have passed
 * since a timer was started. It compares the elapsed time against a
 * required duration to determine if an event should trigger.
 *
 * @param server Pointer to the server structure for timing calculations
 * @param start_timer_units Timer units when the timer was started
 * @param duration Required duration in game time units
 * @param timer Pointer to the reference timespec timer
 * @return true if the duration has elapsed, false otherwise
 */
bool has_time_passed(server_t *server, long long start_timer_units,
    int duration, struct timespec *timer)
{
    long long current_units = get_current_timer_units(server, timer);

    return (current_units - start_timer_units) >= duration;
}
