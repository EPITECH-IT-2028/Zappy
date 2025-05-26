/*
** EPITECH PROJECT, 2025
** server
** File description:
** utils.c
*/

#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <stdio.h>

void send_code(int fd, const char *msg)
{
    char buffer[2048];

    snprintf(buffer, sizeof(buffer), "%s\n", msg);
    write(fd, buffer, strlen(buffer));
}
