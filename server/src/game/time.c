/*
** EPITECH PROJECT, 2025
** server
** File description:
** time.c
*/

#include "server.h"

/**
 * @brief Adds two timespec structures together
 *
 * This function performs addition of two timespec structures, handling
 * nanosecond overflow properly by carrying over to the seconds field
 * when nanoseconds exceed one second.
 *
 * @param start Pointer to the base timespec structure
 * @param duration Pointer to the duration timespec to add
 * @return New timespec structure containing the sum
 */
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

/**
 * @brief Calculates the real-time duration for game actions
 *
 * This function converts game action units into real-time duration
 * based on the server frequency. It handles the conversion from
 * action units to nanoseconds and structures the result properly.
 *
 * @param action_units Number of game time units for the action
 * @param frequency Server frequency (actions per second)
 * @return timespec structure representing the real-time duration
 */
struct timespec calculate_action_duration(int action_units, int frequency)
{
    long total_ns = (long)action_units * NANOSECONDS_PER_SECOND / frequency;
    struct timespec duration = {
        .tv_sec = total_ns / NANOSECONDS_PER_SECOND,
        .tv_nsec = total_ns % NANOSECONDS_PER_SECOND
    };

    return duration;
}

/**
 * @brief Manages periodic server events based on elapsed time
 *
 * This function checks if enough time has passed for periodic server
 * events and triggers them accordingly. It handles food consumption
 * and resource respawning events with their respective timers.
 *
 * @param server Pointer to the server structure containing timers
 */
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
