/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
  #define SERVER_H_

typedef struct {
  int port;
  int width;
  int height;
  char **teams_names;
  int client_per_team;
  int frequence;
} server_t;

int server(int ac, char **av);

#endif /* SERVER_H_ */
