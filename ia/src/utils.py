"""
@file utils.py
@brief Utility constants and configuration for Zappy AI
@author Epitech Project 2025
@date 2025

This module contains all game constants, configuration parameters, and lookup tables
used throughout the Zappy AI system. It defines elevation requirements, game limits,
and various protocol-related constants.
"""

import sys

## @brief Elevation requirements table for level progression
## @details Contains the resource and player requirements for each level advancement.
## Each entry specifies the resources needed and number of players required for incantation.
elevation_table = [
    {
        "level_from": 1,    ##< Starting level
        "level_to": 2,      ##< Target level
        "nb_players": 1,    ##< Number of players required for incantation
        "linemate": 1,      ##< Linemate stones required
        "deraumere": 0,     ##< Deraumere stones required
        "sibur": 0,         ##< Sibur stones required
        "mendiane": 0,      ##< Mendiane stones required
        "phiras": 0,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 2,    ##< Starting level
        "level_to": 3,      ##< Target level
        "nb_players": 2,    ##< Number of players required for incantation
        "linemate": 1,      ##< Linemate stones required
        "deraumere": 1,     ##< Deraumere stones required
        "sibur": 1,         ##< Sibur stones required
        "mendiane": 0,      ##< Mendiane stones required
        "phiras": 0,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 3,    ##< Starting level
        "level_to": 4,      ##< Target level
        "nb_players": 2,    ##< Number of players required for incantation
        "linemate": 2,      ##< Linemate stones required
        "deraumere": 0,     ##< Deraumere stones required
        "sibur": 1,         ##< Sibur stones required
        "mendiane": 0,      ##< Mendiane stones required
        "phiras": 2,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 4,    ##< Starting level
        "level_to": 5,      ##< Target level
        "nb_players": 4,    ##< Number of players required for incantation
        "linemate": 1,      ##< Linemate stones required
        "deraumere": 1,     ##< Deraumere stones required
        "sibur": 2,         ##< Sibur stones required
        "mendiane": 0,      ##< Mendiane stones required
        "phiras": 1,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 5,    ##< Starting level
        "level_to": 6,      ##< Target level
        "nb_players": 4,    ##< Number of players required for incantation
        "linemate": 1,      ##< Linemate stones required
        "deraumere": 2,     ##< Deraumere stones required
        "sibur": 1,         ##< Sibur stones required
        "mendiane": 3,      ##< Mendiane stones required
        "phiras": 0,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 6,    ##< Starting level
        "level_to": 7,      ##< Target level
        "nb_players": 6,    ##< Number of players required for incantation
        "linemate": 1,      ##< Linemate stones required
        "deraumere": 2,     ##< Deraumere stones required
        "sibur": 3,         ##< Sibur stones required
        "mendiane": 0,      ##< Mendiane stones required
        "phiras": 1,        ##< Phiras stones required
        "thystame": 0       ##< Thystame stones required
    },
    {
        "level_from": 7,    ##< Starting level
        "level_to": 8,      ##< Target level
        "nb_players": 6,    ##< Number of players required for incantation
        "linemate": 2,      ##< Linemate stones required
        "deraumere": 2,     ##< Deraumere stones required
        "sibur": 2,         ##< Sibur stones required
        "mendiane": 2,      ##< Mendiane stones required
        "phiras": 2,        ##< Phiras stones required
        "thystame": 1       ##< Thystame stones required
    }
]

## @brief Constants for level transitions
LEVEL_1_TO_2 = elevation_table[0]
LEVEL_2_TO_3 = elevation_table[1]
LEVEL_3_TO_4 = elevation_table[2]
LEVEL_4_TO_5 = elevation_table[3]
LEVEL_5_TO_6 = elevation_table[4]
LEVEL_6_TO_7 = elevation_table[5]
LEVEL_7_TO_8 = elevation_table[6]

## @brief Game command constants
FORWARD = "Forward"
RIGHT = "Right"
LEFT = "Left"
LOOK = "Look"
INVENTORY = "Inventory"
BROADCAST = "Broadcast"
CONNECT_NBR = "Connect_nbr"
FORK = "Fork"
EJECT = "Eject"
TAKE = "Take"
SET = "Set"
INCANTATION = "Incantation"

## @brief Game state constants
NO_TARGET = -1
BUFFER_SIZE = 2048
MAX_COMMANDS = 10
MAX_FOOD = 120
CRITICAL_FOOD = 10
GOOD_FOOD = 50
INVENTORY_ITEMS_COUNT = 7
MIN_REQUIRED_PLAYERS = 1
MAX_REQUIRED_PLAYERS = 6
NBR_PLAYERS_TO_START = 6
PLAYER_CELL = 0
CANT_MOVE = 3
FIRST_COMMAND = 0
MAX_CONSECUTIVE_TURNS = 3
START_MESSAGE = "I_am_starting_to_play"

class ZappyClient:
    """
    @brief Represents a Zappy AI client connected to the server
    @details This class encapsulates the state and behavior of a client in the Zappy AI game.
    @attributes:
        socket (socket.socket): The client socket for communication with the server.
        team_name (str): The name of the team this client belongs to.
        unused_slot (int): Number of unused slots available for new clients.
        map_size (list[int]): Size of the game map as [width, height].
        commands (list[str]): List of commands to be sent to the server.
        inventory (dict): Current inventory of resources for this client.
        last_look (list): Last look response from the server.
        level (int): Current level of the client.
        needed_resources (dict): Resources needed for the next level.
        help_status (bool): Whether the client is currently requesting help.
        help_direction (str or None): Direction in which the client is requesting help.
        is_alive (bool): Whether the client is currently alive in the game.
        move (dict): Current movement state including target and distance.
        incantation (bool): Whether the client is currently performing an incantation.
        waiting_incantation_response (bool): Whether the client is waiting for an incantation response.
        start_playing (bool): Whether the client has started playing.
        status (str): Current status of the client (e.g., "good", "bad").
        player_in_game (int): Number of players currently in the game.
        at_max_food (bool): Whether the client has reached the maximum food limit.
        waiting_for_help (bool): Whether the client is waiting for help from other players.
        look_redirection (bool): Whether the client is redirecting look commands.
        inventory_redirection (bool): Whether the client is redirecting inventory commands.
    """
    
    def __init__(self, client_socket, team_name):
        self.socket = client_socket
        self.team_name = team_name
        self.unused_slot = 0
        self.map_size = [0, 0]
        self.commands = []
        self.inventory = {}
        self.last_look = []
        self.level = 1
        self.needed_resources = LEVEL_1_TO_2
        self.help_status = False
        self.help_direction = None
        self.is_alive = True
        self.move = {
            "consecutive_turns": 0,
            "forward": False,
            "distance_to_target": NO_TARGET,
            "last_target": None,
        }
        self.incantation = False
        self.waiting_incantation_response = False
        self.start_playing = False
        self.status = "good"
        self.player_in_game = 1
        self.at_max_food = False
        self.waiting_for_help = False
        self.look_redirection = False
        self.inventory_redirection = False

def exit_error(message) -> None:
    sys.stderr.write(message + "\n")
    sys.exit(84)
