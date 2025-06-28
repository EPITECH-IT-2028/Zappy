import socket
import threading
import game
import ml_agent
import random
import time
import utils
import select

allowed_commands = [
    "Forward",
    "Right",
    "Left",
    "Look",
    "Inventory",
    "Broadcast",
    "Connect_nbr",
    "Fork",
    "Eject",
    "Take",
    "Set",
    "Incantation",
]

def send_message(client, message) -> None:
    formatted_message = message + "\n"
    client.socket.send((formatted_message).encode())

def handle_Forward(client, response) -> None:
    return

def handle_Right(client, response) -> None:
    return

def handle_Left(client, response) -> None:
    return

def handle_Take(client, response) -> None:
    return

def handle_Set(client, response) -> None:
    return

def handle_Inventory(client, response) -> None:
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
                print(f"response: {response}")
                client.look_redirection = True

def handle_Dead(client, response) -> None:
    client.socket.close()
    client.is_alive = False
    game.remove_client(client)

def handle_Fork(client, response) -> None:
    print("Fork command executed, waiting for connection")
    server_address = client.socket.getpeername()
    team_name = client.team_name
    connect_client(server_address, team_name)

def handle_Look(client, response) -> None:
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    client.last_look = [item.strip() for item in cleaned_response.split(',')]

def is_look_response(message):
    content = message.strip().lstrip('[').rstrip(']')
    items = [item.strip() for item in content.split(',')]
    
    return len(items) != utils.INVENTORY_ITEMS_COUNT

def handle_Broadcast(client, response) -> None:
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
    direction = int(direction.split("message ")[1])
    
    if message.startswith("I_need_help_to_level_up_to_"):
        parts = message.replace("I_need_help_to_level_up_to_", "").split("_with_")

        if len(parts) == 2:
            target_level = int(parts[0])
            food = int(parts[1])
        else:
            return
        if client.waiting_for_help and food > int(client.inventory.get("food", 0)):
            client.waiting_for_help = False
        if target_level == client.level + 1:
            client.help_status = True
            client.help_direction = direction
            return
    
    if message == "I_am_starting_to_play":
        client.player_in_game += 1
        return

def get_requirements_for_level(level):
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
    if response == "ko":
        client.incantation = False
        return
    
    client.waiting_incantation_response = True
    
def handle_Incantation_Response(client, response) -> None:
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
    client.unused_slot = int(response.strip())
    if client.unused_slot == 0:
        time.sleep(0.5)
        execute_command(client, utils.BROADCAST, utils.START_MESSAGE)
    else:
        execute_command(client, utils.CONNECT_NBR, None)

def handle_command(client, command, response):
    func_name = f"handle_{command}"

    if func_name in globals():
        handler = globals()[func_name]
        handler(client, response)

def execute_command(client, command, args) -> None:
    global allowed_commands

    if command not in allowed_commands:
        raise ValueError(f"Command '{command}' non autorisée")
    if len(client.commands) < utils.MAX_COMMANDS:
        client.commands.append(command)
    else:
        print("Max commands reached, command not added to queue")
        return
    if command == utils.BROADCAST or command == utils.SET or command == utils.TAKE:
        send_message(client, f"{command} {args}")
    elif command == utils.FORWARD:
        if client.last_look[utils.PLAYER_CELL].count("player") > utils.CANT_MOVE and client.help_status:
            return
        send_message(client, command)
    else:
        send_message(client, command)

def initialize_clients(client) -> None:
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

            print(f"Player in game: {client.player_in_game}")
            print(f"Unused slot: {client.unused_slot}")
            
            if buffer:
                while "\n" in buffer:
                    message, buffer = buffer.split("\n", 1)

                    if message:
                        if (message == "dead"):
                            print("Client dead, closing connection")
                            handle_Dead(client, message)
                            exit(0)
                        if message.startswith("message "):
                            handle_Broadcast(client, message)
                            continue

                        else:
                            command = client.commands.pop(utils.FIRST_COMMAND)
                            handle_command(client, command, message)

    client.player_in_game = 0
    handle_client(client)

def handle_client(client) -> None:
    buffer = ""

    execute_command(client, utils.LOOK, None)
    while client.is_alive:

        response = client.socket.recv(utils.BUFFER_SIZE).decode()

        if not response:
            print("No response from server, closing connection")
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
                        print("Client dead, closing connection")
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
                            print("Inventory redirection detected")
                            client.inventory_redirection = False
                            handle_Inventory(client, message)
                            command = utils.INVENTORY

                        if client.look_redirection:
                            print("Look redirection detected")
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
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)

    client = utils.ZappyClient(client_socket, team_name)

    welcome_msg = client.socket.recv(utils.BUFFER_SIZE).decode()
    if (welcome_msg != "WELCOME\n"):
        raise ConnectionError("Erreur de connexion au serveur")

    send_message(client, team_name)

    game_data = client.socket.recv(utils.BUFFER_SIZE).decode()
    if game_data == "ko\n":
        print("Unknown team name or team is full")
        return 0
  
    unused_slot = game_data.split()[0]
    
    width, height = game_data.split()[1:3]
    client.map_size = [int(width), int(height)]

    game.add_client(client)

    return int(unused_slot)
