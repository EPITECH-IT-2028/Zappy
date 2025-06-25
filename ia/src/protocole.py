import socket
import threading
import game
import ml_agent
import random
import utils

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
    client["socket"].send((formatted_message).encode())

def handle_Forward(client, response) -> None:
    # print("Avance réussie")
    return

def handle_Right(client, response) -> None:
    # print("Rotation à droite réussie")
    return

def handle_Left(client, response) -> None:
    # print("Rotation à gauche réussie")
    return

def handle_Take(client, response) -> None:
    # print(f"Taking resources")
    return

def handle_Set(client, response) -> None:
    # print(f"Seting resources")
    return

def handle_Inventory(client, response) -> None:
    if not response.startswith("["):
        if response.startswith("message "):
            handle_Broadcast(client, response)
        else:
            return
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    for item in cleaned_response.split(', '):
        # print(f"Item in inventory: {item}")
        if item:
            try:
                resource, quantity = item.split()
                client["inventory"][resource] = int(quantity)
            except:
                print(f"response: {response}")
                client["look_redirection"] = True
    # print(f"Inventory updated: {client['inventory']}")
    # print(f"food: {client["inventory"].get("food", 0)}")

def handle_Dead(client, response) -> None:
    client["socket"].close()
    client["is_alive"] = False
    game.remove_client(client)

def handle_fork(client, response) -> None:
    server_address = client["socket"].getpeername()
    team_name = client["team_name"]
    connect_client(server_address, team_name)

def handle_Look(client, response) -> None:
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    client["last_look"] = [item.strip() for item in cleaned_response.split(',')]

def is_look_response(message):
    content = message.strip().lstrip('[').rstrip(']')
    items = [item.strip() for item in content.split(',')]
    
    if len(items) != 7:
        return True
    
    return False

def handle_Broadcast(client, response) -> None:
    if (response == "ok"):
        return

    if not response.startswith("message "):
        if not (response.startswith("[") and response.endswith("]")):
            return
        if is_look_response(response):
            client["look_redirection"] = True
        else:
            client["inventory_redirection"] = True
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
        print(f"my food: {client['inventory'].get('food', 0)}")
        if client["waiting_for_help"] and food > int(client["inventory"].get("food", 0)):
            client["waiting_for_help"] = False
        if target_level == client["level"] + 1:
            client["help_status"] = True
            client["help_direction"] = direction
            client["total_actions"] = 0
            client["count_actions"] = 0
            return
    
    if message == "I_am_starting_to_play":
        client["player_in_game"] += 1
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
        print("Incantation échouée")
        client["incantation"] = False
        return
    
    client["waiting_incantation_response"] = True
    print("Incantation en cours")
    
def handle_Incantation_Response(client, response) -> None:
    if response == "ko":
        print("Incantation échouée")
        client["incantation"] = False
        return

    new_level = int(response.split("Current level:")[1])
    client["level"] = new_level
    client["needed_resources"] = get_requirements_for_level(new_level)
    client["incantation"] = False
    if client["help_status"]:
        client["help_status"] = False
        client["help_direction"] = None
    client["waiting_for_help"] = False
    print(f"Incantation réussie, nouveau niveau: {new_level}")
    execute_command(client, utils.LOOK, None)

def handle_commande(client, commande, response):
    func_name = f"handle_{commande}"

    if func_name in globals():
        handler = globals()[func_name]
        handler(client, response)

def execute_command(client, commande, args) -> None:
    global allowed_commands

    if commande not in allowed_commands:
        raise ValueError(f"Commande '{commande}' non autorisée")
    if commande == utils.BROADCAST or commande == utils.SET or commande == utils.TAKE:
        send_message(client, f"{commande} {args}")
    elif commande == utils.FORWARD:
        if client["last_look"][0].count("player") > 3 and client["help_status"]:
            return
        send_message(client, commande)
    else:
        send_message(client, commande)
    if len(client["commandes"]) < 10:
        client["commandes"].append(commande)

def handle_client(client) -> None:
    buffer = ""

    execute_command(client, utils.LOOK, None)
    while client["is_alive"]:

        response = client["socket"].recv(1024).decode()

        buffer += response

        if buffer:
            while "\n" in buffer:
                message, buffer = buffer.split("\n", 1)

                if message:
                    if client["waiting_incantation_response"]:
                        if message.startswith("Current level:") or message == "ko":
                            handle_Incantation_Response(client, message)
                            client["waiting_incantation_response"] = False
                        continue

                    if (message == "dead"):
                        print("Client dead, closing connection")
                        handle_Dead(client, message)
                        break

                    if (message == "Elevation underway" and client["commandes"][0] != utils.INCANTATION):
                        handle_Incantation(client, "ok")
                        continue

                    if (message.startswith("message ")):
                        handle_Broadcast(client, message)
                        continue
                    
                    if (client["commandes"]):
                            command = client["commandes"].pop(0)

                            if client["inventory_redirection"]:
                                client["inventory_redirection"] = False
                                execute_command(client, utils.INVENTORY, message)

                            elif client["look_redirection"]:
                                client["look_redirection"] = False
                                execute_command(client, utils.LOOK, None)
                            else:
                                handle_commande(client, command, message)

                            if command == utils.INVENTORY:
                                execute_command(client, utils.LOOK, None)

                            if command == utils.LOOK:
                                ml_agent.strategy(client)

            if client["waiting_for_help"]:
                print(f"Client commandes: {client['commandes']}")
                            
        else:
            execute_command(client, utils.LOOK, None)

def connect_client(server_address, team_name) -> int:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)

    client = {
        "socket": client_socket,
        "team_name": team_name,
        "mape_size": [0, 0],
        "commandes": [],
        "broadcast_commandes": [],
        "inventory": {},
        "last_look": [],
        "level": 1,
        "needed_resources": utils.LEVEL_1_TO_2,
        "help_status": False,
        "help_direction": None,
        "is_alive": True,
        "move": {
            "consecutive_turns": 0,
            "forward": False,
            "distance_to_target": -1,
            "last_target": None,
        },
        "incantation": False,
        "waiting_incantation_response": False,
        "start_playing": False,
        "status": "good",
        "player_in_game": 0,
        "at_120_food": False,
        "waiting_for_help": False,
        "look_redirection": False,
        "inventory_redirection": False,
    }

    welcome_msg = client["socket"].recv(1024).decode()
    if (welcome_msg != "WELCOME\n"):
        raise ConnectionError("Erreur de connexion au serveur")

    send_message(client, team_name)

    game_data = client["socket"].recv(1024).decode()
    if game_data == "ko\n":
        print("Unknown team name or team is full")
        return 0
  
    unused_slot = game_data.split()[0]
    
    width, height = game_data.split()[1:3]
    client["mape_size"] = [int(width), int(height)]

    game.add_client(client)

    return int(unused_slot)
