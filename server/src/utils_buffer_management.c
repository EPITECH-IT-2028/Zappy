/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_buffer_management.c
*/

#include "macro.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Adds a string buffer to a response array
 * @param buffer String to be added to the response array
 * @param response Pointer to array of strings (will be reallocated)
 * @param index Pointer to the current index in the response array
 *
 * This function reallocates the response array to accommodate one more string,
 * duplicates the provided buffer string at the current index position,
 * adds a NULL terminator at the end of the array, and increments the index.
 *
 * @return SUCCESS (0) if the buffer was added successfully, ERROR otherwise
 */
int add_buffer_to_response(char *buffer, char ***response, int *index)
{
    char **new_response = NULL;

    if (!buffer || !response || !index)
        return ERROR;
    new_response = realloc(*response, sizeof(char *) * (*index + 2));
    if (!new_response)
        return ERROR;
    *response = new_response;
    (*response)[*index] = strndup(buffer, strlen(buffer));
    if (!(*response)[*index]) {
        free(*response);
        return ERROR;
    }
    (*response)[*index + 1] = NULL;
    *index += 1;
    return SUCCESS;
}
