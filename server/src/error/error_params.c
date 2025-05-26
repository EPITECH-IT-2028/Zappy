/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** error_params.c
*/

#include "server.h"
#include <stddef.h>
#include "macro.h"

static int check_port(server_t *server, int ac, char **av)
{
    (void)server;
    (void)ac;
    (void)av;
  return SUCCESS;
}

int check_params(server_t *server, int ac, char **av)
{
    (void)server;
    check_port(server, ac, av);
  if (ac <= MIN_PARAMS)
    return ERROR;
  for (size_t i = 0; av[i] != NULL; i ++) {

  }
  return SUCCESS;
}
