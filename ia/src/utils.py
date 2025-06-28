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
CONNECT_NBR = "Connect_nbr"

NO_TARGET = -1
BUFFER_SIZE = 1024
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
