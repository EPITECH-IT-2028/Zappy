import sys

import sys

elevation_table = [
    {
        "level_from": 1,
        "level_to": 2,
        "nb_players": 1,
        "linemate": 1,
        "deraumere": 0,
        "sibur": 0,
        "mendiane": 0,
        "phiras": 0,
        "thystame": 0
    },
    {
        "level_from": 2,
        "level_to": 3,
        "nb_players": 2,
        "linemate": 1,
        "deraumere": 1,
        "sibur": 1,
        "mendiane": 0,
        "phiras": 0,
        "thystame": 0
    },
    {
        "level_from": 3,
        "level_to": 4,
        "nb_players": 2,
        "linemate": 2,
        "deraumere": 0,
        "sibur": 1,
        "mendiane": 0,
        "phiras": 2,
        "thystame": 0
    },
    {
        "level_from": 4,
        "level_to": 5,
        "nb_players": 4,
        "linemate": 1,
        "deraumere": 1,
        "sibur": 2,
        "mendiane": 0,
        "phiras": 1,
        "thystame": 0
    },
    {
        "level_from": 5,
        "level_to": 6,
        "nb_players": 4,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 1,
        "mendiane": 3,
        "phiras": 0,
        "thystame": 0
    },
    {
        "level_from": 6,
        "level_to": 7,
        "nb_players": 6,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 3,
        "mendiane": 0,
        "phiras": 1,
        "thystame": 0
    },
    {
        "level_from": 7,
        "level_to": 8,
        "nb_players": 6,
        "linemate": 2,
        "deraumere": 2,
        "sibur": 2,
        "mendiane": 2,
        "phiras": 2,
        "thystame": 1
    }
]

LEVEL_1_TO_2 = elevation_table[0]
LEVEL_2_TO_3 = elevation_table[1]
LEVEL_3_TO_4 = elevation_table[2]
LEVEL_4_TO_5 = elevation_table[3]
LEVEL_5_TO_6 = elevation_table[4]
LEVEL_6_TO_7 = elevation_table[5]
LEVEL_7_TO_8 = elevation_table[6]

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

def exit_error(message) -> None:
    """
    Displays an error message and exits the program.

    Args:
        message (str): Error message to display.
    """
    sys.stderr.write(message + "\n")
    sys.exit(84)

def exit_error(message) -> None:
    """
    Displays an error message and exits the program.

    Args:
        message (str): Error message to display.
    """
    sys.stderr.write(message + "\n")
    sys.exit(84)