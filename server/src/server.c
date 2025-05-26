/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.c
*/

#include "server.h"

int server(int ac, char **av) {
  server_t server;
  if (check_params(&server, ac, av)) {
    return ERROR;
  }
  return SUCCESS;
}
