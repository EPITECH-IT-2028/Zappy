/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdint.h>
    #include <stdio.h>
    #include <stdatomic.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <pthread.h>
    #include <stdbool.h>
    #include <time.h>
    #include "macro.h"
    #include "inventory.h"

typedef enum direction_s {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction_t;


typedef struct incantation_s {
    bool is_incantating;
    uint8_t x;
    uint8_t y;
    uint8_t id_incantator;
} incantation_t;

typedef struct {
    int x;
    int y;
} direction_offset_t;

typedef struct params_s {
    int port;
    int width;
    int height;
    char **teams_names;
    int client_per_team;
    int frequence;
    int teams_count;
} params_t;

typedef struct egg_s {
    int id;
    int x;
    int y;
    int player_id;
} egg_t;


typedef struct teams_s {
    char *name;
    int clients_count;
} teams_t;

struct client_s;

typedef struct response_s {
    struct client_s *client;
    char response[BUFFER_SIZE];
} response_t;

typedef struct client_data_s {
    char *team_name;
    bool is_graphic;
    int id;
    int x;
    int y;
    int orientation;
    int level;
    int pending_requests;
    pthread_mutex_t pending_mutex;
    inventory_t inventory;
    direction_t direction;
    bool has_egg;
    bool is_busy;
    incantation_t incantation;
    struct timespec action_end_time;
    struct response_s pending_response;
} client_data_t;

typedef struct client_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int fd_open;
    bool connected;
    client_data_t data;
} client_t;

typedef struct request_s {
    client_t *client;
    char request[BUFFER_SIZE];
} request_t;

typedef struct threads_s {
    pthread_t game_thread;
} threads_t;

typedef struct queue_response_s {
    response_t queue[QUEUE_MAX_SIZE];
    int head;
    int tail;
    int len;
    pthread_mutex_t mutex;
} queue_response_t;

typedef struct queue_request_s {
    request_t queue[QUEUE_MAX_SIZE];
    int head;
    int tail;
    int len;
    pthread_mutex_t mutex;
} queue_request_t;

typedef struct map_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
    int nbr_of_players;
    client_t **players;
    egg_t *eggs;
    int eggs_count;
} map_t;

typedef struct server_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    client_t **clients;
    char *path;
    struct pollfd *fds;
    int nfds;
    params_t params;
    teams_t *teams;
    atomic_bool running;
    queue_response_t queue_response;
    queue_request_t queue_request;
    struct timespec server_timer;
    long long server_timer_count;
    threads_t threads;
    map_t **map;
    int ids;
    int egg_ids;
} server_t;

typedef struct sound_result_s {
    uint8_t direction_tile;
    bool received;
} sound_result_t;

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
void free_server(server_t *server);

/* Function for multi-thread */
int game_loop(server_t *server);

/* Connection commands */
void connection_command(server_t *server, int index, char *buffer);

/* Player commands */
void check_player_command(server_t *server, int index, const char *buffer);

/* Gui commands */
void map_commands(server_t *server, int index, char *buffer);
void time_commands(server_t *server, int index, char *buffer);
void player_commands(server_t *server, int index, char *buffer);

/* Game Events */
void remove_food(server_t *server);
void send_enw(server_t *server, int index, egg_t *egg);
void send_all_eggs_to_gui(server_t *server, int index);

/* Parameters checks */
int help_flag(void);
int check_port(params_t *params, char **av, size_t *av_idx);
int check_width(params_t *params, char **av, size_t *av_idx);
int check_height(params_t *params, char **av, size_t *av_idx);
int check_teams_names(params_t *params, char **av, size_t *av_idx);
int check_clients_nb(params_t *params, char **av, size_t *av_idx);
int check_freq(params_t *params, char **av, size_t *av_idx);

/* Queues functions */
int queue_add_request(server_t *server, request_t *request);
int queue_add_response(server_t *server, response_t *response);
int queue_pop_request(server_t *server, request_t *request);
int queue_pop_response(server_t *server, response_t *response);

/* Map functions */
int place_resources(server_t *server);

/* Egg functions */
int assign_random_egg_position(server_t *server, client_t *client);
void remove_egg(map_t *tile, int index);
int count_total_eggs(server_t *server);
egg_t *create_egg(int id, int x, int y, int player_id);
int place_egg(map_t *tile, egg_t *egg);

/* Timer functions */
struct timespec get_action_end_time(server_t *server, int action_duration);
long long get_current_timer_units(server_t *server);
bool has_time_passed(server_t *server, long long, int duration);

/* Direction function */
void init_direction(direction_t *direction);

/* Incantation functions */
int check_if_incantation_failed(
    client_data_t *incantator,
    client_t **clients,
    map_t *unit_space
);

#endif /* SERVER_H_ */
