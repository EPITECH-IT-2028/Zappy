/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdatomic.h>
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

typedef struct client_data_s {
    char *team_name;
    bool is_graphic;
    int x;
    int y;
    int level;
} client_data_t;

typedef struct client_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int fd_open;
    bool connected;
    client_data_t data;
} client_t;

typedef struct server_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    client_t **clients;
    char *path;
    struct pollfd *fds;
    int nfds;
    params_t params;
    atomic_bool running;
    pthread_t game_thread;
} server_t;

typedef struct command_s {
    char *name;
    void (*f)(server_t *server, int i, char *str);
} command_t;

int server(int ac, char **av);
int check_params(params_t *params, int ac, char **av);

/* Struct initialization functions */
void init_client_struct(client_t *clients, int fd);
int init_server_struct(server_t *server, params_t *params);
void init_params(params_t *params);

/* Event handling functions */
int get_new_connection(server_t *server);
void handle_all_client(server_t *server);

/* Resource management functions */
void free_params(params_t *params);

/* Function for multi-thread */
void *game(void *arg);

/* Connection commands */
void connection_command(server_t *server, int index, char *buffer);

/* Parameters checks */
int help_flag(void);
int check_port(params_t *params, char **av, size_t *av_idx);
int check_width(params_t *params, char **av, size_t *av_idx);
int check_height(params_t *params, char **av, size_t *av_idx);
int check_teams_names(params_t *params, char **av, size_t *av_idx);
int check_clients_nb(params_t *params, char **av, size_t *av_idx);
int check_freq(params_t *params, char **av, size_t *av_idx);

#endif /* SERVER_H_ */
