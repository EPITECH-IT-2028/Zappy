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
    print("Avance réussie")

def handle_Right(client, response) -> None:
    print("Rotation à droite réussie")

def handle_Left(client, response) -> None:
    print("Rotation à gauche réussie")

def handle_Take(client, response) -> None:
    print(f"Taking resources")

def handle_Set(client, response) -> None:
    print(f"Seting resources")

def handle_Inventory(client, response) -> None:
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    for item in cleaned_response.split(', '):
        if item:
            resource, quantity = item.split()
            client["inventory"][resource] = int(quantity)
    print(f"Inventory updated: {client['inventory']}")

def handle_Dead(client, response) -> None:
    client["socket"].close()
    client["is_alive"] = False
    game.remove_client(client)

def handle_fork(client, response) -> None:
    server_address = client["socket"].getpeername()
    team_name = client["team_name"]
    connect_client(server_address, team_name)

def handle_Broadcast(client, response) -> None:
    if (response == "ok"):
        return
    print(f"Message broadcast reçu: {response}")

def handle_Look(client, response) -> None:
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    client["last_look"] = [item.strip() for item in cleaned_response.split(',')]

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
        return
    
    new_response = client["socket"].recv(1024).decode().strip()

    if new_response == "ko":
        print("Incantation échouée")
        return
    
    new_level = int(new_response.split("Current level:")[1])
    client["level"] = new_level
    client["needed_resources"] = get_requirements_for_level(new_level)
    client["incantation"] = False
    print(f"Incantation réussie, nouveau niveau: {new_level}")

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

        while "\n" in buffer:
            message, buffer = buffer.split("\n", 1)

            if message:
                if (message == "dead"):
                    print("Client dead, closing connection")
                    handle_Dead(client, message)

                if (client["commandes"]):
                    command = client["commandes"].pop(0)
                    handle_commande(client, command, message)

                    if command == utils.LOOK:
                        ml_agent.strategy(client)
                    
                    if command == utils.INVENTORY:
                        execute_command(client, utils.LOOK, None)

def connect_client(server_address, team_name) -> int:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_address)

    client = {
        "socket": client_socket,
        "team_name": team_name,
        "mape_size": [0, 0],
        "commandes": [],
        "inventory": {},
        "last_look": [],
        "level": 1,
        "needed_resources": utils.LEVEL_1_TO_2,
        "is_alive": True,
        "move": {
            "consecutive_turns": 0,
            "forward": False,
            "distance_to_target": -1,
            "last_target": None,
        },
        "incantation": False,
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
    print(f"Game data received: {game_data}")
    
    width, height = game_data.split()[1:3]
    client["mape_size"] = [int(width), int(height)]
    print(f"Unused slot: {unused_slot}")
    print(f"Map size: {client['mape_size']}")

    game.add_client(client)

    return int(unused_slot)
