"""
@file ml_agent.py
@brief Machine Learning Agent for Zappy AI
@author Epitech Project 2025
@date 2025

This module implements a machine learning agent for the Zappy AI game.
"""

import protocole
import game
import math
import random
import utils

def create_help_message(level, food_count):
    """
    @brief Creates a help message for incantation assistance
    @param level: The current level of the player
    @param food_count: The amount of food the player has
    @return: A formatted help message string

    @details This function generates a message that can be broadcasted to other players
    to request assistance for an incantation. The message includes the player's current level
    and the amount of food they have. The message format is standardized for easy recognition.
    """

    return f"I_need_help_to_level_up_to_{level + 1}_with_{food_count}"


def get_position(index, total_cells) -> tuple[int, int]:
    """
    @brief Calculates the (x, y) position in a spiral grid based on the index
    @param index: The index of the cell in the spiral grid
    @param total_cells: The total number of cells in the spiral grid
    @return: A tuple (x, y) representing the position in the grid

    @details This function computes the (x, y) coordinates of a cell in a spiral grid
    based on its index. The grid is structured such that the center cell is (0, 0),
    and the cells are arranged in concentric squares. The function iterates through
    the spiral layers until it finds the correct position for the given index.
    If the index is 0, it returns (0, 0) as the center of the grid.
    """
    if index == 0:
        return (0, 0)

    current_index = 1
    distance = 1

    while current_index < total_cells:
        line = distance * 2 + 1
        if current_index + line > index:
            x = index - current_index - distance
            y = distance
            return (x, y)
        current_index += line
        distance += 1
    return (0, 0)


def search_closest_resource(client, needed_resources):
    """
    @brief Searches for the closest resource in the client's vision data
    @param client: The ZappyClient object containing the client's state
    @param needed_resources: A dictionary of resources needed by the client
    @return: A dictionary with the closest resource's position and name, or None if no resources are found

    @details This function scans the client's vision data for resources that match
    the names in the `needed_resources` dictionary. It calculates the distance of each
    resource from the center of the vision grid (0, 0) and keeps track of the closest
    resource found. The distance is calculated using the Euclidean formula.
    If a resource is found, it returns a dictionary containing the x and y coordinates
    of the resource and its name. If no resources are found, it returns None.
    """

    vision_data = client.last_look
    closest_resource = None
    min_distance = float('inf')

    for index, cell in enumerate(vision_data):
        for resource_name in needed_resources.keys():
            if resource_name in cell.lower():
                x, y = get_position(index, len(vision_data))
                distance = math.sqrt(x * x + y * y)

                if distance < min_distance:
                    min_distance = distance
                    closest_resource = {
                        'x': x,
                        'y': y,
                        'resource_name': resource_name,
                    }
                break

    return closest_resource


def get_action(client, needed_resources):
    """
    @brief Determines the next action for the client based on needed resources
    @param client: The ZappyClient object containing the client's state
    @param needed_resources: A dictionary of resources needed by the client
    @return: A tuple containing the action to take and the resource name if applicable

    @details This function decides the next action for the client based on the resources
    it needs. It first searches for the closest resource that matches the needed resources.
    If no resources are found, it checks if the client has reached the maximum number of
    consecutive turns without finding a resource. If so, it resets the turn count and
    sets the action to move forward. If the client has a last target direction, it checks
    if the distance to that target is zero and decides to turn left or right accordingly.
    If a resource is found, it checks the coordinates of the resource and decides whether
    to move forward, turn left, or turn right based on the resource's position relative to the client.
    If the resource is at the center (0, 0), it sets the action to take the resource.
    If the resource is directly in front (0, y > 0), it moves forward. If the resource is to the right or left,
    it updates the last target direction and sets the action to move forward.
    If the resource is to the right (x > 0) or left (x < 0), it updates the last target direction and sets
    the action to move forward, while also updating the distance to the target.
    If the client has not reached the maximum consecutive turns, it increments the turn count and decides
    to turn left or right randomly.
    """

    closest_resources = search_closest_resource(client, needed_resources)

    if closest_resources is None:
        if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
            client.move["consecutive_turns"] = 0
            client.move["forward"] = True
            return utils.FORWARD, None
        else:
            if random.random() < 0.5:
                client.move["consecutive_turns"] += 1
                return utils.RIGHT, None
            else:
                client.move["consecutive_turns"] += 1
                return utils.LEFT, None

    x, y = closest_resources["x"], closest_resources["y"]
    resources_name = closest_resources["resource_name"]

    if x == 0 and y == 0:
        client.move["consecutive_turns"] = 0
        client.move["forward"] = False
        client.move["distance_to_target"] = utils.NO_TARGET
        client.move["last_target"] = None
        return utils.TAKE, resources_name

    if x == 0 and y > 0:
        client.move["consecutive_turns"] = 0
        client.move["forward"] = True
        return utils.FORWARD, None

    if client.move["last_target"] == "right" and client.move["distance_to_target"] == 0:
        client.move["consecutive_turns"] += 1
        client.move["distance_to_target"] = utils.NO_TARGET
        return utils.RIGHT, None

    if client.move["last_target"] == "left" and client.move["distance_to_target"] == 0:
        client.move["consecutive_turns"] += 1
        client.move["distance_to_target"] = utils.NO_TARGET
        return utils.LEFT, None

    if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
        client.move["consecutive_turns"] = 0
        client.move["forward"] = True
        return utils.FORWARD, None

    if x > 0:
        client.move["last_target"] = "right"
        client.move["forward"] = True
        client.move["distance_to_target"] = y - 1
        return utils.FORWARD, None

    if x < 0:
        client.move["last_target"] = "left"
        client.move["forward"] = True
        client.move["distance_to_target"] = y - 1
        return utils.FORWARD, None

    client.move["consecutive_turns"] = 0
    client.move["forward"] = True
    return None, None


def get_resources(client, needed_resources):
    """
    @brief Executes the action to get resources based on the client's state
    @param client: The ZappyClient object containing the client's state
    @param needed_resources: A dictionary of resources needed by the client
    @return: None

    @details This function handle the action to take resources based on the client's state.
    If the action is to take a resource, it executes the command to take that resource.
    If the action is to move forward, it executes the command to move forward.
    If the action is to turn left or right, it executes the corresponding command.
    """

    action, arg = get_action(client, needed_resources)

    if action == utils.TAKE:
        protocole.execute_command(client, action, arg)
    else:
        protocole.execute_command(client, action, None)


def analyze_vision(client):
    """
    @brief Analyzes the client's vision data to categorize cells
    @param client: The ZappyClient object containing the client's state
    @return: A dictionary with the total number of cells, food cells, and resource cells

    @details This function processes the client's last look at the game world,
    categorizing each cell into food or resources. It creates a dictionary that
    contains the total number of cells, a list of food cells, and a list of resource cells.
    Each cell is represented by its index, x and y coordinates, and its content.
    The function iterates through the vision data, calculates the (x, y) position of each cell
    using the `get_position` function, and checks if the cell contains food or resources.
    The results are stored in a dictionary that is returned at the end.
    """

    vision_data = client.last_look
    analyzed_vision = {
        'total_cells': len(vision_data),
        'food': [],
        'resources': []
    }

    for index, cell in enumerate(vision_data):
        x, y = get_position(index, len(vision_data))

        cell_info = {
            'index': index,
            'x': x,
            'y': y,
            'content': cell
        }

        if 'food' in cell_info['content']:
            analyzed_vision['food'].append(cell_info)
        else:
            analyzed_vision['resources'].append(cell_info)

    return analyzed_vision


def define_needed_resources(client):
    """
    @brief Defines the resources needed for the client's current level
    @param client: The ZappyClient object containing the client's state
    @return: A dictionary of resources needed by the client

    @details This function checks the client's current level and returns a dictionary
    containing the resources needed for that level. The resources are defined in a
    predefined dictionary that maps levels to their required resources.
    """

    needed = client.needed_resources
    inventory = client.inventory
    missing_resources = {}

    valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

    for resource, quantity in needed.items():
        if inventory.get(resource, 0) < quantity and resource in valid_resources:
            missing_resources[resource] = quantity - inventory.get(resource, 0)

    return missing_resources


def verify_incantation(client, current_cell, needed_resources):
    """
    @brief Verifies if the client can perform an incantation
    @param client: The ZappyClient object containing the client's state
    @param current_cell: The current cell the client is in
    @param needed_resources: A dictionary of resources needed by the client
    @return: None

    @details This function checks if the client can perform an incantation based on
    the resources available in the current cell and the number of players present.
    It first checks if the client has the required resources for incantation.
    If the client is at level 1, it sets the required number of players to the minimum.
    If the current cell does not have the needed resources or if the number of players
    is less than the required number, it sets the client's incantation status to False
    and waiting_for_help status to True. If the conditions are met, it sets the
    client's incantation status to True and waiting_for_help status to False.
    """

    required_players = utils.MAX_REQUIRED_PLAYERS
    count_players = current_cell.count("player")

    if client.level == 1:
        required_players = utils.MIN_REQUIRED_PLAYERS

    if needed_resources:
        client.incantation = False
        return

    if count_players < required_players:
        client.incantation = False
        client.waiting_for_help = True
        return

    client.waiting_for_help = False
    client.incantation = True


def setting_up_incantation(client, current_cell):
    """
    @brief Sets up the incantation for the client based on current cell resources
    @param client: The ZappyClient object containing the client's state
    @param current_cell: The current cell the client is in
    @return: True if the incantation can be performed, False otherwise

    @details This function checks if the current cell has the required resources for
    incantation and if the number of players is sufficient. It iterates through
    the valid resources and checks if the quantity in the current cell meets the
    client's needed resources. If any resource is present, it sets the resource
    in the client's inventory. If the current cell does not have enough resources
    or if the number of players is less than the required number, it returns False.
    If all conditions are met, it returns True, indicating that the incantation can be performed.
    """

    valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
    count_players = current_cell.count("player")
    required_players = utils.MAX_REQUIRED_PLAYERS

    if client.level == 1:
        required_players = utils.MIN_REQUIRED_PLAYERS

    for resource in valid_resources:
        quantity = client.inventory.get(resource, 0)

        if quantity > 0:
            protocole.execute_command(client, utils.SET, resource)

        if current_cell.count(resource) < client.needed_resources.get(resource, 0):
            return False

    if count_players < required_players:
        protocole.execute_command(client, utils.BROADCAST,
                                  create_help_message(client.level, client.inventory.get("food", 0)))
        return False

    return True


def get_action_from_broadcast(client, direction):
    """
    @brief Executes the action based on the direction received from a broadcast
    @param client: The ZappyClient object containing the client's state
    @param direction: The direction to move based on the broadcast
    @return: None

    @details This function interprets the direction received from a broadcast message
    and executes the corresponding actions. It uses a predefined mapping of directions
    to actions, where each direction corresponds to a list of actions to be executed.
    The function checks the direction and executes the actions in the order specified
    in the mapping. If the direction is not recognized, it defaults to an empty action list.
    The actions include moving forward, turning left, or turning right.
    """

    direction_actions = {
        0: [],
        1: [utils.FORWARD],
        2: [utils.FORWARD, utils.LEFT],
        3: [utils.LEFT, utils.FORWARD],
        4: [utils.LEFT, utils.FORWARD, utils.LEFT],
        5: [utils.LEFT, utils.LEFT, utils.FORWARD],
        6: [utils.RIGHT, utils.FORWARD, utils.RIGHT],
        7: [utils.RIGHT, utils.FORWARD],
        8: [utils.FORWARD, utils.RIGHT],
    }

    actions = direction_actions.get(direction, [])

    for action in actions:
        protocole.execute_command(client, action, None)

    client.help_direction = 0

def strategy(client):
    """
    @brief Main strategy function for the Zappy AI client
    @param client: The ZappyClient object containing the client's state
    @return: None

    @details This function implements the main strategy for the Zappy AI client.
    It analyzes the client's vision data, defines the needed resources based on the
    client's level, and manages the client's actions based on its current state.
    The function first checks if the client is ready to start playing. If not, it waits
    for the required number of players to start the game. If the client has enough food,
    it attempts to gather resources or move around the game world. If the client is in a
    critical state (low food), it prioritizes gathering food and resources. If the client
    is ready to incant, it verifies if the conditions are met for performing an incantation.
    If the client is waiting for help, it broadcasts a message to request assistance.
    If the client is not in a critical state, it checks if it needs to gather resources
    or food, and executes the appropriate actions based on the current cell's content.
    """
    
    vision_data = analyze_vision(client)
    needed = define_needed_resources(client)
    food_count = client.inventory.get("food", 0)
    current_cell = client.last_look[utils.PLAYER_CELL] if client.last_look else None

    if not client.start_playing:
        if client.player_in_game >= utils.NBR_PLAYERS_TO_START:
            client.start_playing = True
            protocole.execute_command(client, utils.INVENTORY, None)
            return

        if not client.at_max_food and food_count >= utils.MAX_FOOD:
            client.at_max_food = True
            client.player_in_game += 1
            protocole.execute_command(client, utils.BROADCAST, utils.START_MESSAGE)

        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if food_count < utils.CRITICAL_FOOD:
        client.status = "critique"

    if client.status == "critique":
        if food_count > utils.GOOD_FOOD:
            client.status = "good"

    if client.status == "critique":
        client.help_status = False
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if client.help_status:
        get_action_from_broadcast(client, client.help_direction)
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if not client.incantation:
        verify_incantation(client, current_cell, needed)

    if client.incantation:
        if setting_up_incantation(client, current_cell):
            protocole.execute_command(client, utils.INCANTATION, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if client.waiting_for_help:
        if len(client.commands) > utils.MAX_COMMANDS - 1:
            protocole.execute_command(client, utils.INVENTORY, None)
            return
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.BROADCAST,
                                  create_help_message(client.level, client.inventory.get("food")))
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if needed and current_cell:
        action, arg = get_action(client, needed)
        if action == utils.TAKE and arg in current_cell.lower():
            protocole.execute_command(client, action, arg)
            protocole.execute_command(client, utils.INVENTORY, None)
            return

    if food_count < 20 and client.status == "good":
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
            protocole.execute_command(client, utils.INVENTORY, None)
            return

        for resource, quantity in needed.items():
            if resource in current_cell.lower():
                get_resources(client, {resource: quantity})
                protocole.execute_command(client, utils.INVENTORY, None)
                return

        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)

    else:
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
            protocole.execute_command(client, utils.INVENTORY, None)
            return

        if needed:
            get_resources(client, needed)
        else:
            if vision_data["food"]:
                get_resources(client, {"food": 1})
            else:
                if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                    client.move["consecutive_turns"] = 0
                    client.move["forward"] = True
                    protocole.execute_command(client, utils.FORWARD, None)
                else:
                    if random.random() < 0.5:
                        client.move["consecutive_turns"] += 1
                        protocole.execute_command(client, utils.RIGHT, None)
                    else:
                        protocole.execute_command(client, utils.FORWARD, None)

    protocole.execute_command(client, utils.INVENTORY, None)