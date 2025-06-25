/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils_buffer_management.c
*/

#include "macro.h"
#include <stdlib.h>
#include <string.h>

int add_buffer_to_response(char *buffer, char ***response, int *index)
{
    char **new_response = NULL;
    
    if (!buffer || !response || !index)
        return ERROR;
    new_response = realloc(*response, sizeof(char *) * (*index + 1));
    if (!new_response)
        return ERROR;
    *response = new_response;
    (*response)[*index - 1] = strdup(buffer);
    if (!(*response)[*index - 1])
        return ERROR;
    (*response)[*index] = NULL;
    *index += 1;
    return SUCCESS;
}
