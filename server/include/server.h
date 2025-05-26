/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <stdbool.h>

typedef struct params_s {
    int port;
    int width;
    int height;
    char **teams_names;
    int client_per_team;
    int frequence;
    int teams_count;
} params_t;

typedef struct client_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int fd_open;
    bool connected;
} client_t;

typedef struct server_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    client_t **clients;
    char *path;
    struct pollfd *fds;
    int nfds;
} server_t;

typedef struct command_s {
    char *name;
    void (*f)(server_t *server, int i, char *str);
} command_t;

int server(int ac, char **av);
int check_params(params_t *params, int ac, char **av);

/* Struct initialization functions */
int init_client_struct(client_t *clients);
int init_server_struct(server_t *server, params_t *params);

/* Event handling functions */
int get_new_connection(server_t *server);

#endif /* SERVER_H_ */
