"""
@file protocole.py
@brief Zappy server communication protocol management module
@author Epitech Project 2025
@date 2025

This module contains all functions for handling Zappy protocol commands and responses.
It enables bidirectional communication between the AI and the game server.
"""

import socket
import game
import ml_agent
import message as msg
import time
import utils
import select
import sys

## @brief List of commands allowed by the Zappy protocol
## @details Contains all commands that the AI can send to the server
allowed_commands = [
    "Forward",      ##< Command to move forward
    "Right",        ##< Command to turn right
    "Left",         ##< Command to turn left
    "Look",         ##< Command to look around
    "Inventory",    ##< Command to check inventory
    "Broadcast",    ##< Command to broadcast a message
    "Connect_nbr",  ##< Command to get number of connections
    "Fork",         ##< Command to create a new client
    "Eject",        ##< Command to eject a player
    "Take",         ##< Command to take an object
    "Set",          ##< Command to drop an object
    "Incantation",  ##< Command to start an incantation
]

def send_message(client, message) -> None:
    """
    @brief Sends a formatted message to the server
    @param client Zappy client sending the message
    @param message Message to send (without newline)
    @return None
    
    @details Automatically adds a newline to the message and encodes it
    in UTF-8 before sending through the socket.
    """
        
    formatted_message = message + "\n"
    client.socket.send((formatted_message).encode())

def handle_Forward(client, response) -> None:
    """
    @brief Response handler for the Forward command
    @param client Client that executed the command
    @param response Server response ("ok" if successful)
    @return None
    
    @details Processes the response to a forward movement command.
    """

    return

def handle_Right(client, response) -> None:
    """
    @brief Response handler for the Right command
    @param client Client that executed the command
    @param response Server response ("ok" if successful)
    @return None
    
    @details Processes the response to a right turn command.
    """

    return

def handle_Left(client, response) -> None:
    """
    @brief Response handler for the Left command
    @param client Client that executed the command
    @param response Server response ("ok" if successful)
    @return None

    @details Processes the response to a left turn command.
    """

    return

def handle_Take(client, response) -> None:
    """
    @brief Response handler for the Take command
    @param client Client that executed the command
    @param response Server response ("ok" if successful)
    @return None

    @details Processes the response to a take command.
    """

    return

def handle_Set(client, response) -> None:
    """
    @brief Response handler for the Set command
    @param client Client that executed the command
    @param response Server response ("ok" if successful)
    @return None

    @details Processes the response to a set command.
    """

    return

def handle_Inventory(client, response) -> None:
    """
    @brief Response handler for the Inventory command
    @param client Client that executed the command
    @param response Server response containing inventory data
    @return None

    @details Parses the inventory response and updates the client's inventory.
    """

    if not response.startswith("["):
        if response.startswith("message "):
            handle_Broadcast(client, response)
        else:
            return
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    for item in cleaned_response.split(', '):
        if item:
            try:
                resource, quantity = item.split()
                client.inventory[resource] = int(quantity)
            except ValueError:
                client.look_redirection = True

def handle_Dead(client, response) -> None:
    """
    @brief Handles the death of a client
    @param client Client that has died
    @param response Server response indicating death
    @return None

    @details Closes the client's socket and removes it from the game.
    """

    client.socket.close()
    client.is_alive = False
    game.remove_client(client)

def handle_Fork(client, response) -> None:
    """
    @brief Handles the Fork command response
    @param client Client that executed the Fork command
    @param response Server response to the Fork command
    @return None

    @details If the response is "ok", it indicates a successful fork.
    If the response is "ko", it indicates a failure to fork.
    """
    server_address = client.socket.getpeername()
    team_name = client.team_name
    connect_client(server_address, team_name)

def handle_Look(client, response) -> None:
    """
    @brief Handles the Look command response
    @param client Client that executed the Look command
    @param response Server response to the Look command
    @return None

    @details Parses the response to update the client's last look.
    """

    cleaned_response = response.strip().lstrip('[').rstrip(']')
    client.last_look = [item.strip() for item in cleaned_response.split(',')]

def is_look_response(message):
    """
    @brief Checks if the response is a look response
    @param message Response message to check

    @return bool True if it is a look response, False otherwise
    """
    content = message.strip().lstrip('[').rstrip(']')
    items = [item.strip() for item in content.split(',')]
    
    return len(items) != utils.INVENTORY_ITEMS_COUNT

def handle_Broadcast(client, response) -> None:
    """
    @brief Handles the Broadcast command response
    @param client Client that executed the Broadcast command
    @param response Server response to the Broadcast command
    @return None

    @details Parses the response to determine if it contains a message or a direction.
    If the response is "ok", it indicates a successful broadcast.
    If the response starts with "message ", it indicates a message broadcast.
    If the response is a help request, it updates the client's help status.
    If the response is a start message, it increments the player's in-game count.
    """

    if (response == "ok"):
        return

    if not response.startswith("message "):
        if not (response.startswith("[") and response.endswith("]")):
            return
        if is_look_response(response):
            client.look_redirection = True
        else:
            client.inventory_redirection = True
        return

    direction, message = response.split(", ")
    decrypted_message = msg.decrypt(message)
    direction = int(direction.split("message ")[1])

    if decrypted_message.startswith("I_need_help_to_level_up_to_"):
        parts = decrypted_message.replace("I_need_help_to_level_up_to_", "").split("_with_")

        if len(parts) == 2:
            target_level = int(parts[0])
            food = int(parts[1])
        else:
            return
        if client.waiting_for_help and food > int(client.inventory.get("food", 0)):
            client.waiting_for_help = False
            execute_command(client, utils.LOOK, None)
        if target_level == client.level + 1 and not client.waiting_for_help:
            client.help_status = True
            client.help_direction = direction
            return
    if decrypted_message == "I_am_starting_to_play":
        client.player_in_game += 1
        return

def get_requirements_for_level(level):
    """
    @brief Returns the resource requirements for a given level
    @param level Level for which to get the requirements
    @return dict Resource requirements for the level

    @details Uses a mapping of levels to their resource requirements.
    """

    requirements_map = {
        1: utils.LEVEL_1_TO_2,
        2: utils.LEVEL_2_TO_3,
        3: utils.LEVEL_3_TO_4,
        4: utils.LEVEL_4_TO_5,
        5: utils.LEVEL_5_TO_6,
        6: utils.LEVEL_6_TO_7,
        7: utils.LEVEL_7_TO_8
    }
    return requirements_map.get(level, {})

def handle_Incantation(client, response) -> None:
    """
    @brief Handles the Incantation command response
    @param client Client that executed the Incantation command
    @param response Server response to the Incantation command
    @return None

    @details If the response is "ok", it indicates a successful incantation.
    If the response is "ko", it indicates a failure to incant.
    """

    if response == "ko":
        client.incantation = False
        return
    
    client.waiting_incantation_response = True
    
def handle_Incantation_Response(client, response) -> None:
    """
    @brief Handles the response to an incantation command
    @param client Client that executed the incantation command
    @param response Server response to the incantation command
    @return None

    @details If the response is "ko", it indicates a failure to incant.
    If the response starts with "Current level:", it updates the client's level.
    """
    if response == "ko":
        client.incantation = False
        return

    new_level = int(response.split("Current level:")[1])
    client.level = new_level
    client.needed_resources = get_requirements_for_level(new_level)
    client.incantation = False
    if client.help_status:
        client.help_status = False
        client.help_direction = None
    client.waiting_for_help = False
    execute_command(client, utils.LOOK, None)

def handle_Connect_nbr(client, response) -> None:
    """
    @brief Handles the Connect_nbr command response
    @param client Client that executed the Connect_nbr command
    @param response Server response to the Connect_nbr command
    @return None

    @details Parses the response to determine the number of unused slots.
    If the response is "0", it indicates no unused slots.
    If the response is not "0", it updates the client's unused slot count.
    If there are unused slots, it executes the Connect_nbr command again.
    """

    client.unused_slot = int(response.strip())
    if client.unused_slot == 0:
        time.sleep(0.5)
        execute_command(client, utils.BROADCAST, utils.START_MESSAGE)
    else:
        execute_command(client, utils.CONNECT_NBR, None)

def handle_command(client, command, response):
    """
    @brief Handles a command response based on the command type
    @param client Client that executed the command
    @param command Command that was executed
    @param response Server response to the command
    @return None

    @details Dynamically calls the appropriate handler function based on the command type.
    If the command is not recognized, it raises a ValueError.
    """

    func_name = f"handle_{command}"

    if func_name in globals():
        handler = globals()[func_name]
        handler(client, response)

def execute_command(client, command, args) -> None:
    """
    @brief Executes a command by sending it to the server
    @param client Client that will execute the command
    @param command Command to execute
    @param args Arguments for the command (if any)
    @return None

    @details Checks if the command is allowed, appends it to the client's command list,
    and sends it to the server. If the command is BROADCAST, it encrypts the arguments.
    If the command is SET or TAKE, it sends the arguments directly.
    If the command is LOOK, it checks if the last command was also LOOK to avoid redundancy.
    If the command list exceeds the maximum allowed commands, it does not append the command.
    """

    global allowed_commands

    if command not in allowed_commands:
        raise ValueError(f"Command '{command}' non autorisée")
    if (command == utils.LOOK and len(client.commands) > 0 and client.commands[-1] == utils.LOOK):
        return
    if len(client.commands) < utils.MAX_COMMANDS:
        client.commands.append(command)
    else:
        return
    if command == utils.BROADCAST:
        send_message(client, f"{command} {msg.encrypt(args)}")
    elif command == utils.SET or command == utils.TAKE:
        send_message(client, f"{command} {args}")
    else:
        send_message(client, command)

def initialize_clients(client) -> None:
    """
    @brief Initializes clients and manages the connection process
    @param client Client to initialize
    @return None

    @details Connects the client to the server and manages the number of players in the game.
    If the number of players is less than the required number to start, it forks a new client.
    If the client is the only player, it connects to the server and waits for other players.
    If the client is not alive, it exits the process.
    If the client is alive, it waits for responses from the server and processes them.
    If the client receives a "dead" message, it handles the death of the client.
    If the client receives a "message" command, it handles the broadcast message.
    If the client receives a command, it handles the command response.
    """

    buffer = ""
    last_fork_check = time.time()
    fork_check_interval = 2
    
    execute_command(client, utils.CONNECT_NBR, None)
    
    while client.is_alive and client.player_in_game < utils.NBR_PLAYERS_TO_START:
        current_time = time.time()
        if current_time - last_fork_check >= fork_check_interval:
            if client.player_in_game < utils.NBR_PLAYERS_TO_START and client.unused_slot == 0:
                client.player_in_game = 1
                execute_command(client, utils.FORK, None)
                execute_command(client, utils.CONNECT_NBR, None)
            
            last_fork_check = current_time
        
        ready, _, _ = select.select([client.socket], [], [], 0.05)
        
        if ready:
            response = client.socket.recv(utils.BUFFER_SIZE).decode()
            buffer += response


            if buffer:
                while "\n" in buffer:
                    message, buffer = buffer.split("\n", 1)

                    if message:
                        if (message == "dead"):
                            handle_Dead(client, message)
                            sys.exit(0)
                        if message.startswith("message "):
                            handle_Broadcast(client, message)
                            continue

                        command = client.commands.pop(utils.FIRST_COMMAND)
                        handle_command(client, command, message)

    client.player_in_game = 0
    handle_client(client)

def handle_client(client) -> None:
    """
    @brief Handles the main client communication loop
    @param client Client to handle
    @return None

    @details Waits for responses from the server and processes them.
    If the client is not alive, it exits the process.
    If the client is alive, it waits for responses from the server and processes them.
    If the client receives a "dead" message, it handles the death of the client.
    If the client receives a "message" command, it handles the broadcast message.
    If the client receives a command, it handles the command response.
    """

    buffer = ""

    execute_command(client, utils.LOOK, None)
    while client.is_alive: 
        response = client.socket.recv(utils.BUFFER_SIZE).decode()

        if not response:
            client.socket.close()
            client.is_alive = False
            break

        buffer += response

        if buffer:
            while "\n" in buffer:
                message, buffer = buffer.split("\n", 1)

                if message:
                    if client.waiting_incantation_response:
                        if message.startswith("Current level:") or message == "ko":
                            handle_Incantation_Response(client, message)
                            client.waiting_incantation_response = False
                        continue

                    if (message == "dead"):
                        handle_Dead(client, message)
                        break

                    if (message == "Elevation underway" and client.commands[utils.FIRST_COMMAND] != utils.INCANTATION):
                        handle_Incantation(client, "ok")
                        continue

                    if (message.startswith("message ")):
                        handle_Broadcast(client, message)
                        continue
                    
                    if (client.commands):
                        command = client.commands.pop(utils.FIRST_COMMAND)

                        handle_command(client, command, message)

                        if client.inventory_redirection:
                            client.inventory_redirection = False
                            handle_Inventory(client, message)
                            command = utils.INVENTORY

                        if client.look_redirection:
                            client.look_redirection = False
                            handle_Look(client, message)
                            command = utils.LOOK

                        if command == utils.INVENTORY:
                            execute_command(client, utils.LOOK, None)

                        if command == utils.LOOK:
                            ml_agent.strategy(client)
                            
        else:
            execute_command(client, utils.LOOK, None)

def connect_client(server_address, team_name) -> int:
    """
    @brief Connects to the Zappy server and initializes a client
    @param server_address Tuple containing the server address and port
    @param team_name Name of the team to connect
    @return int Unused slot number if successful, 0 if connection failed
    
    @details Creates a socket connection to the server, sends the team name,
    and receives the initial game data. If the connection is successful,
    it initializes the client and returns the unused slot number.
    If the connection fails, it raises a ConnectionError.
    """

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)

    client = utils.ZappyClient(client_socket, team_name)

    welcome_msg = client.socket.recv(utils.BUFFER_SIZE).decode()
    if (welcome_msg != "WELCOME\n"):
        raise ConnectionError("Erreur de connexion au serveur")

    send_message(client, team_name)

    game_data = client.socket.recv(utils.BUFFER_SIZE).decode()
    if game_data == "ko\n":
        return 0
  
    unused_slot = game_data.split()[0]
    
    width, height = game_data.split()[1:3]
    client.map_size = [int(width), int(height)]

    game.add_client(client)

    return int(unused_slot)
