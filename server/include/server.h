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

/**
 * @struct ressources_s
 * @brief Resource mapping structure
 *
 * Maps resource names to their inventory and space pointers for easy access
 * and management of different resource types in the game.
 */
typedef struct ressources_s {
    const char *name;
    int *inv;
    int *space;
} ressources_t;

/**
 * @enum direction_s
 * @brief Player movement directions
 *
 * Represents the four cardinal directions for player movement and orientation
 * in the game world.
 */
typedef enum direction_s {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction_t;

/**
 * @struct incantation_s
 * @brief Incantation ceremony data
 *
 * Contains all information about an ongoing incantation ritual, including
 * participants, location, and success status.
 */
typedef struct incantation_s {
    bool is_incantating;
    uint8_t x;
    uint8_t y;
    uint8_t id_incantator;
    struct client_s **client_group;
    bool incantation_success;
} incantation_t;

/**
 * @struct direction_offset_t
 * @brief Direction offset for movement calculations
 *
 * Used to calculate new positions based on direction with x and y offsets.
 */
typedef struct {
    int x;
    int y;
} direction_offset_t;

/**
 * @struct params_s
 * @brief Server startup parameters
 *
 * Contains all command-line arguments and configuration for server
 * initialization.
 */
typedef struct params_s {
    int port;
    int width;
    int height;
    char **teams_names;
    int client_per_team;
    int frequence;
    int teams_count;
} params_t;

/**
 * @struct egg_s
 * @brief Egg structure for player spawning
 *
 * Represents an egg that can hatch into a player, containing position and
 * team information.
 */
typedef struct egg_s {
    int id;
    int x;
    int y;
    int player_id;
    int team_id;
} egg_t;

/**
 * @struct teams_s
 * @brief Team information structure
 *
 * Contains team data including ID, name, and current client count.
 */
typedef struct teams_s {
    int id;
    char *name;
    int clients_count;
} teams_t;

/**
 * @brief Forward declaration of client structure
 */
struct client_s;

/**
 * @struct response_s
 * @brief Server response structure
 *
 * Contains response data to be sent to clients, including the target client
 * and response messages.
 */
typedef struct response_s {
    struct client_s *client;
    int size;
    char **response;
} response_t;

/**
 * @struct pending_action_s
 * @brief Pending action in client queue
 *
 * Represents a queued action with execution time, forming a linked list
 * of pending commands for each client.
 */
typedef struct pending_action_s {
    char command[BUFFER_SIZE];
    struct timespec execute_time;
    struct pending_action_s *next;
} pending_action_t;

/**
 * @struct client_data_s
 * @brief Client game data structure
 *
 * Contains all game-related data for a client including position, inventory,
 * team information, and action queues.
 */
typedef struct client_data_s {
    char *team_name;
    int team_id;
    bool is_graphic;
    int id;
    int x;
    int y;
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
    pending_action_t *queue_head;
    pending_action_t *queue_tail;
    bool is_action;
} client_data_t;

/**
 * @struct client_s
 * @brief Client connection structure
 *
 * Represents a connected client with network information and game data.
 */
typedef struct client_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int fd_open;
    bool connected;
    client_data_t data;
    char *buffer;
} client_t;

/**
 * @struct request_s
 * @brief Client request structure
 *
 * Contains a client request with the associated client and request data.
 */
typedef struct request_s {
    client_t *client;
    char request[BUFFER_SIZE];
} request_t;

/**
 * @struct threads_s
 * @brief Thread management structure
 *
 * Contains thread handles for multi-threaded server operations.
 */
typedef struct threads_s {
    pthread_t game_thread;
} threads_t;

/**
 * @struct queue_response_s
 * @brief Response queue structure
 *
 * Thread-safe circular queue for managing server responses to clients.
 */
typedef struct queue_response_s {
    response_t queue[QUEUE_MAX_SIZE];
    int head;
    int tail;
    int len;
    pthread_mutex_t mutex;
} queue_response_t;

/**
 * @struct queue_request_s
 * @brief Request queue structure
 *
 * Thread-safe circular queue for managing client requests to the server.
 */
typedef struct queue_request_s {
    request_t queue[QUEUE_MAX_SIZE];
    int head;
    int tail;
    int len;
    pthread_mutex_t mutex;
} queue_request_t;

/**
 * @struct map_s
 * @brief Game map tile structure
 *
 * Represents a single tile on the game map containing resources,
 * players, and eggs.
 */
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

/**
 * @struct server_s
 * @brief Main server structure
 *
 * Central structure containing all server state, including network
 * configuration, client management, game map, and threading components.
 */
typedef struct server_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    client_t **clients;
    pthread_mutex_t clients_mutex;
    char *path;
    struct pollfd *fds;
    int nfds;
    params_t params;
    teams_t *teams;
    atomic_bool running;
    queue_response_t queue_response;
    queue_request_t queue_request;
    inventory_t density;
    struct timespec server_timer;
    long long server_timer_count;
    struct timespec density_timer;
    long long density_timer_count;
    threads_t threads;
    map_t **map;
    int ids;
    int egg_ids;
} server_t;

/**
 * @struct sound_result_s
 * @brief Sound broadcast result structure
 *
 * Contains the result of a sound broadcast including direction
 * and reception status.
 */
typedef struct sound_result_s {
    uint8_t direction_tile;
    bool received;
} sound_result_t;

/**
 * @struct command_s
 * @brief Command handler structure
 *
 * Maps command names to their corresponding handler functions.
 */
typedef struct command_s {
    char *name;
    void (*f)(server_t *server, int i, char *str);
} command_t;

/**
 * @brief Main server entry point
 * @param ac Argument count
 * @param av Argument vector
 * @return Exit status code
 */
int server(int ac, char **av);

/**
 * @brief Validate server parameters from command line
 * @param params Parameters structure to validate
 * @param ac Argument count
 * @param av Argument vector
 * @return 0 on success, error code otherwise
 */
int check_params(params_t *params, int ac, char **av);

/**
 * @brief Initialize client structure with default values
 * @param clients Client structure to initialize
 * @param fd File descriptor for the client connection
 */
void init_client_struct(client_t *clients, int fd);

/**
 * @brief Initialize server structure with parameters
 * @param server Server structure to initialize
 * @param params Configuration parameters
 * @return 0 on success, error code otherwise
 */
int init_server_struct(server_t *server, params_t *params);

/**
 * @brief Initialize parameters structure with default values
 * @param params Parameters structure to initialize
 */
void init_params(params_t *params);

/**
 * @brief Initialize response structure
 * @param response Response structure to initialize
 */
void init_response(response_t *response);

/**
 * @brief Accept new client connections
 * @param server Server instance
 * @return 0 on success, error code otherwise
 */
int get_new_connection(server_t *server);

/**
 * @brief Handle all connected clients
 * @param server Server instance
 */
void handle_all_client(server_t *server);

/**
 * @brief Free allocated memory in parameters structure
 * @param params Parameters structure to free
 */
void free_params(params_t *params);

/**
 * @brief Free all server resources and cleanup
 * @param server Server structure to free
 */
void free_server(server_t *server);

/**
 * @brief Cleanup client data at specified index
 * @param server Server instance
 * @param index Client index to cleanup
 */
void cleanup_client_data(server_t *server, int index);

/**
 * @brief Main game loop for multi-threaded execution
 * @param server Server instance
 * @return 0 on success, error code otherwise
 */
int game_loop(server_t *server);

/**
 * @brief Handle initial connection commands from clients
 * @param server Server instance
 * @param index Client index
 * @param buffer Command buffer
 */
void connection_command(server_t *server, int index, char *buffer);

/**
 * @brief Process player commands
 * @param server Server instance
 * @param index Client index
 * @param buffer Command buffer
 */
void check_player_command(server_t *server, int index, const char *buffer);

/**
 * @brief Handle GUI map-related commands
 * @param server Server instance
 * @param index Client index
 * @param buffer Command buffer
 */
void map_commands(server_t *server, int index, char *buffer);

/**
 * @brief Handle GUI time-related commands
 * @param server Server instance
 * @param index Client index
 * @param buffer Command buffer
 */
void time_commands(server_t *server, int index, char *buffer);

/**
 * @brief Handle GUI player-related commands
 * @param server Server instance
 * @param index Client index
 * @param buffer Command buffer
 */
void player_commands(server_t *server, int index, char *buffer);

/**
 * @brief Remove food from the game world periodically
 * @param server Server instance
 */
void remove_food(server_t *server);

/**
 * @brief Send egg new (enw) event to GUI clients
 * @param server Server instance
 * @param egg Egg that was created
 */
void send_enw(server_t *server, egg_t *egg);

/**
 * @brief Send player fork (pfk) event to GUI clients
 * @param server Server instance
 * @param client Client that is forking
 */
void send_pfk(server_t *server, client_t *client);

/**
 * @brief Send egg broadcast (ebo) event to GUI clients
 * @param server Server instance
 * @param egg Egg that is broadcasting
 */
void send_ebo(server_t *server, egg_t *egg);

/**
 * @brief Send egg death (edi) event to GUI clients
 * @param server Server instance
 * @param egg Egg that died
 */
void send_edi(server_t *server, egg_t *egg);

/**
 * @brief Send player new (pnw) event to GUI clients
 * @param server Server instance
 * @param index Client index of new player
 */
void send_pnw(server_t *server, int index);

/**
 * @brief Send player expulsion (pex) event to GUI clients
 * @param server Server instance
 * @param client Client that was expelled
 */
void send_pex(server_t *server, client_t *client);

/**
 * @brief Send player drop resource (pdr) event to GUI clients
 * @param server Server instance
 * @param client Client that dropped resource
 * @param resource_id ID of the dropped resource
 */
void send_pdr(server_t *server, client_t *client, int resource_id);

/**
 * @brief Send player get resource (pgt) event to GUI clients
 * @param server Server instance
 * @param client Client that took resource
 * @param resource_id ID of the taken resource
 */
void send_pgt(server_t *server, client_t *client, int resource_id);

/**
 * @brief Send player death (pdi) event to GUI clients
 * @param server Server instance
 * @param index Client index of dead player
 */
void send_pdi(server_t *server, int index);

/**
 * @brief Send player inventory (pin) event to GUI clients
 * @param server Server instance
 * @param index Client index
 */
void send_pin(server_t *server, int index);

/**
 * @brief Send player position (ppo) event to GUI clients
 * @param server Server instance
 * @param client Client whose position to send
 */
void send_ppo(server_t *server, client_t *client);

/**
 * @brief Send player broadcast (pbc) event to GUI clients
 * @param server Server instance
 * @param client Client that is broadcasting
 * @param message Broadcast message
 */
void send_pbc(server_t *server, client_t *client, const char *message);

/**
 * @brief Send all eggs information to GUI clients
 * @param server Server instance
 */
void send_all_eggs_to_gui(server_t *server);

/**
 * @brief Send incantation start (pic) event to GUI clients
 * @param server Server instance
 * @param incantators Array of clients participating in incantation
 */
void send_pic(server_t *server, client_t **incantators);

/**
 * @brief Send incantation end (pie) event to GUI clients
 * @param server Server instance
 * @param incantators Array of clients that participated in incantation
 */
void send_pie(server_t *server, client_t **incantators);

/**
 * @brief Send broadcast content (bct) of tile to GUI clients
 * @param server Server instance
 * @param x X coordinate of tile
 * @param y Y coordinate of tile
 */
void send_bct(server_t *server, int x, int y);

/**
 * @brief Remove player from map tile
 * @param map Map tile
 * @param client Client to remove
 * @return 0 on success, error code otherwise
 */
int remove_player_map(map_t *map, client_t *client);

/**
 * @brief Add player to map tile
 * @param server Server instance
 * @param map Map tile pointer
 * @param client Client to add
 * @return 0 on success, error code otherwise
 */
int add_player_map(server_t *server, map_t **map, client_t *client);

/**
 * @brief Display help information and usage
 * @return Always returns 0
 */
int help_flag(void);

/**
 * @brief Validate and set port parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_port(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Validate and set width parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_width(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Validate and set height parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_height(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Validate and set team names parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_teams_names(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Validate and set clients per team parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_clients_nb(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Validate and set frequency parameter
 * @param params Parameters structure
 * @param av Argument vector
 * @param av_idx Current argument index pointer
 * @return 0 on success, error code otherwise
 */
int check_freq(params_t *params, char **av, size_t *av_idx);

/**
 * @brief Add request to the server request queue
 * @param server Server instance
 * @param request Request to add
 * @return 0 on success, error code otherwise
 */
int queue_add_request(server_t *server, request_t *request);

/**
 * @brief Add response to the server response queue
 * @param server Server instance
 * @param response Response to add
 * @return 0 on success, error code otherwise
 */
int queue_add_response(server_t *server, response_t *response);

/**
 * @brief Pop request from the server request queue
 * @param server Server instance
 * @param request Request structure to fill
 * @return 0 on success, error code otherwise
 */
int queue_pop_request(server_t *server, request_t *request);

/**
 * @brief Pop response from the server response queue
 * @param server Server instance
 * @param response Response structure to fill
 * @return 0 on success, error code otherwise
 */
int queue_pop_response(server_t *server, response_t *response);

/**
 * @brief Place initial resources on the game map
 * @param server Server instance
 * @return 0 on success, error code otherwise
 */
int place_resources(server_t *server);

/**
 * @brief Assign random position for egg placement
 * @param server Server instance
 * @param client Client associated with the egg
 * @return 0 on success, error code otherwise
 */
int assign_random_egg_position(server_t *server, client_t *client);

/**
 * @brief Remove egg from map tile
 * @param tile Map tile
 * @param index Egg index to remove
 */
void remove_egg(map_t *tile, int index);

/**
 * @brief Count total number of eggs in the game
 * @param server Server instance
 * @return Total egg count
 */
int count_total_eggs(server_t *server);

/**
 * @struct egg_args_s
 * @brief Arguments for egg creation
 *
 * Contains the necessary parameters for creating a new egg.
 */
typedef struct egg_args_s {
    int x;
    int y;
    int player_id;
    int team_id;
} egg_args_t;

/**
 * @brief Create a new egg with specified parameters
 * @param id Unique egg ID
 * @param args Egg creation arguments
 * @return Pointer to created egg, NULL on failure
 */
egg_t *create_egg(int id, egg_args_t args);

/**
 * @brief Place egg on map tile
 * @param tile Map tile
 * @param egg Egg to place
 * @return 0 on success, error code otherwise
 */
int place_egg(map_t *tile, egg_t *egg);

/**
 * @brief Calculate action end time based on duration
 * @param server Server instance
 * @param action_duration Duration in time units
 * @return Timespec structure with end time
 */
struct timespec get_action_end_time(server_t *server, int action_duration);

/**
 * @brief Get current timer units for synchronization
 * @param server Server instance
 * @param timer Timer reference
 * @return Current timer units
 */
long long get_current_timer_units(server_t *server, struct timespec *timer);

/**
 * @brief Check if specified time duration has passed
 * @param server Server instance
 * @param start_timer_units Starting timer units
 * @param duration Duration to check
 * @param timer Timer reference
 * @return True if time has passed, false otherwise
 */
bool has_time_passed(server_t *server, long long start_timer_units,
    int duration, struct timespec *timer);

/**
 * @brief Initialize direction to default value
 * @param direction Direction pointer to initialize
 */
void init_direction(direction_t *direction);

/**
 * @brief Check if incantation has failed due to conditions
 * @param incantator Main incantator client data
 * @param clients Array of participating clients
 * @param unit_space Map tile where incantation occurs
 * @return 0 if successful, error code if failed
 */
int check_if_incantation_failed(
    client_data_t *incantator,
    client_t **clients,
    map_t *unit_space
);

/**
 * @brief Free memory allocated for incantator array
 * @param incantators Array of incantator clients
 */
void free_incantators(client_t **incantators);

/**
 * @brief Handle the completion of an incantation ceremony
 * @param server Server instance
 * @param response Response structure
 * @param request Request structure
 * @return 0 on success, error code otherwise
 */
int handle_ending_incantation(server_t *server, response_t *response,
    request_t *request);

/**
 * @brief Build group of clients for incantation
 * @param main_client Main incantator client
 * @param unit_space Map tile for incantation
 * @return Number of participants in incantation
 */
uint8_t build_incantation_group(client_t *main_client, map_t *unit_space);

/**
 * @brief Setup main incantator for ceremony
 * @param server Server instance
 * @param client Client data of main incantator
 */
void setup_main_incantator(server_t *server, client_data_t *client);

/**
 * @brief Setup group members for incantation ceremony
 * @param client Client data of main incantator
 * @param nbr_of_incantators Number of participating incantators
 */
void setup_group_members(client_data_t *client, uint8_t nbr_of_incantators);

/**
 * @brief Initialize incantation state structure
 * @param inc Incantation structure to initialize
 */
void init_incantation_state(incantation_t *inc);

/**
 * @brief Remove required resources from tile for incantation
 * @param tile Map tile
 * @param level Incantation level
 * @return 0 on success, error code otherwise
 */
int remove_needed_ressources(map_t *tile, uint8_t level);

/**
 * @brief Check and update resource between inventory and map
 * @param server Server instance
 * @param request Client request
 * @param client Client data
 * @param from_inv_to_map Direction of resource transfer
 * @return 0 on success, error code otherwise
 */
int check_ressource_update(server_t *server, request_t *request,
    client_data_t *client, bool from_inv_to_map);

/**
 * @brief Increment resources on map tile
 * @param map Map tile
 * @param type Resource type to increment
 */
void increment_resources(map_t *map, int type);

/**
 * @brief Respawn resources across the game map
 * @param server Server instance
 * @return 0 on success, error code otherwise
 */
int respawn_resources(server_t *server);

/**
 * @brief Increment resource density counter
 * @param server Server instance
 * @param type Resource type
 */
void increment_resource_density(server_t *server, int type);

/**
 * @brief Decrement resource density counter
 * @param server Server instance
 * @param type Resource type
 */
void decrement_resource_density(server_t *server, int type);

/**
 * @brief Initialize resource density tracking
 * @param server Server instance
 * @param density Density structure to initialize
 * @return 0 on success, error code otherwise
 */
int init_density(server_t *server, inventory_t *density);

/**
 * @brief Add buffer content to response array
 * @param buffer Buffer to add
 * @param response Response array pointer
 * @param index Current index pointer
 * @return 0 on success, error code otherwise
 */
int add_buffer_to_response(char *buffer, char ***response, int *index);

/**
 * @brief Define available index in server structures
 * @param server Server instance
 * @return Available index, -1 if none found
 */
int define_index(server_t *server);

/**
 * @brief Resize file descriptor array
 * @param server Server instance
 * @param new_size New size for the array
 * @return 0 on success, error code otherwise
 */
int resize_fds(server_t *server, int new_size);

/**
 * @brief Initialize file descriptor structure
 * @param server Server instance
 * @param index Index in fd array
 * @param client_fd Client file descriptor
 */
void init_fds(server_t *server, int index, int client_fd);

/**
 * @brief Copy response data between structures
 * @param dest Destination response
 * @param src Source response
 * @param count Number of responses to copy
 * @return 0 on success, error code otherwise
 */
int copy_response_data(response_t *dest, response_t *src, int count);

/**
 * @brief Cleanup pending response structure
 * @param pending Pending response to cleanup
 * @return 0 on success, error code otherwise
 */
int cleanup_pending_response(response_t *pending);

/**
 * @brief Check if response queue is full and handle overflow
 * @param server Server instance
 * @param response Response to potentially add
 */
void check_if_queue_is_full(server_t *server, response_t *response);

/**
 * @brief Check if client is on cooldown for actions
 * @param client_data Client data to check
 * @return 1 if on cooldown, 0 otherwise
 */
int is_client_on_cd(client_data_t *client_data);

/**
 * @brief Sleep for appropriate time based on server frequency
 * @param server Server instance
 */
void sleep_time(server_t *server);

/**
 * @brief Add action to client's action queue
 * @param client Client to add action to
 * @param command Command string
 * @param server Server instance for timing
 * @return 0 on success, error code otherwise
 */
int add_action_to_client_queue(client_t *client, const char *command,
    server_t *server);

/**
 * @brief Cleanup all game resources during shutdown
 * @param server Server instance
 */
void cleanup_game_resources(server_t *server);

/**
 * @brief Add two timespec structures
 * @param start Starting timespec
 * @param duration Duration to add
 * @return Sum of the two timespec structures
 */
struct timespec timespec_add(struct timespec *start,
    struct timespec *duration);

/**
 * @brief Calculate action duration based on units and frequency
 * @param action_units Action duration in game units
 * @param frequency Server frequency
 * @return Timespec structure with calculated duration
 */
struct timespec calculate_action_duration(int action_units, int frequency);

/**
 * @brief Check and handle time-based events
 * @param server Server instance
 */
void check_time_events(server_t *server);

/**
 * @brief Check and process client request
 * @param server Server instance
 * @param response Response structure
 * @param request Request structure
 * @return 0 on success, error code otherwise
 */
int check_request(server_t *server, response_t *response, request_t *request);

/**
 * @brief Cleanup old actions from client queue
 * @param client Client to cleanup
 */
void cleanup_old_actions(client_t *client);

void send_plv(server_t *server, client_t **incantators);

#endif /* SERVER_H_ */
