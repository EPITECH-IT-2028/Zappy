/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #define ERROR 1
    #define SUCCESS 0

    #define MIN_PARAMS 14
    // 14 because :
    // BIN + -p + PORT + -x + WIDTH + -y + HEIGHT + -n
    // + (MIN 2 TEAMS) + -c + CLIENTNB + -f + FREQ = 14

    #define MIN_X 10
    #define MIN_Y 10
    #define MIN_CLIENT 1

typedef struct {
    int port;
    int width;
    int height;
    char **teams_names;
    int client_per_team;
    int frequence;
    int teams_count;
} server_t;

int server(int ac, char **av);

int check_params(server_t *server, int ac, char **av);

#endif /* SERVER_H_ */
