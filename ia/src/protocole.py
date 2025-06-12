import socket
import threading
import game
import ml_agent
import random

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
    print(f"Taking food")

def handle_Set(client, response) -> None:
    send_message(client, "Inventory")

def handle_Inventory(client, response) -> None:
    cleaned_response = response.strip().lstrip('[').rstrip(']')
    for item in cleaned_response.split(', '):
        if item:
            resource, quantity = item.split()
            client["inventory"][resource] = int(quantity)

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

def handle_commande(client, commande, response):
    func_name = f"handle_{commande}"

    if func_name in globals():
        handler = globals()[func_name]
        handler(client, response)

def execute_command(client, commande, args) -> None:
    global allowed_commands

    if commande not in allowed_commands:
        raise ValueError(f"Commande '{commande}' non autorisée")
    if commande == "Broadcast" or commande == "Set" or commande == "Take":
        send_message(client, f"{commande} {args}")
    else:
        send_message(client, commande)
    if len(client["commandes"]) < 10:
        client["commandes"].append(commande)

def handle_client(client) -> None:
    buffer = ""

    execute_command(client, "Look", None)
    while client["is_alive"]:
        response = client["socket"].recv(1024).decode()

        buffer += response

        while "\n" in buffer:
            message, buffer = buffer.split("\n", 1)

            if message:
                if (message == "dead\n"):
                    print("Client dead, closing connection")
                    handle_Dead(client, message)

                if (client["commandes"]):
                    command = client["commandes"].pop(0)
                    print(f"Traitement réponse pour: {command} -> {message}")
                    handle_commande(client, command, message)

                    if command == "Look":
                        ml_agent.strategy(client)
                    
                    if command == "Inventory":
                        execute_command(client, "Look", None)

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
        "is_alive": True,
        "move": {
            "consecutive_turns": 0,
            "forward": False,
            "distance_to_food": -1,
            "last_food": None,
        }
    }

    welcome_msg = client["socket"].recv(1024).decode()
    if (welcome_msg != "WELCOME\n"):
        raise ConnectionError("Erreur de connexion au serveur")

    send_message(client, team_name)

    game_data = client["socket"].recv(1024).decode()
    unused_slot = game_data.split()[0]

    if unused_slot == "ko\n":
        raise ConnectionError("Unknown team name or team is full")
    
    width, height = game_data.split()[1:3]
    client["mape_size"] = [int(width), int(height)]
    print(f"Unused slot: {unused_slot}")
    print(f"Map size: {client['mape_size']}")

    game.add_client(client)

    return int(unused_slot)
