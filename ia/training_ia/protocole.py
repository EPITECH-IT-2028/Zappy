import socket
import game
from ia_logic import *
from data import *
import time
import sys


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


def parse_inventory(response: str) -> dict:
    inventory = {}
    response = response.strip().strip("[]")
    for item in response.split(","):
        parts = item.strip().split()
        if len(parts) == 2:
            key, val = parts
            inventory[key] = int(val)
    return inventory

def handle_ia_client(client) -> None:
    while client["is_alive"]:
        try:
            send_message(client, "Inventory")
            inventory_raw = client["socket"].recv(1024).decode()
            inventory = parse_inventory(inventory_raw)
            send_message(client, "Look")
            look_raw = client["socket"].recv(1024).decode()

            state = build_state(client, inventory, look_raw)

            food = inventory.get("food", 0)

            if food <= 1:
                action = "Take food"
            else:
                action = predict_action(state)

            print(f"[IA] food={food} | action={action} | state={state}")

            if action.startswith("Take"):
                res = action.replace("Take ", "")
                execute_command(client, "Take", res)
            else:
                execute_command(client, action, None)

            record_state(state, action)

            response = client["socket"].recv(1024).decode()
            if "dead" in response:
                print("[IA] Mort détectée.")
                client["is_alive"] = False

        except Exception as e:
            print(f"Error message: {e}", file=sys.stderr)
            break


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
    if game_data == "ko\n":
        print("Unknown team name or team is full")
        return 0

    unused_slot = game_data.split()[0]
    print(f"Game data received: {game_data}")

    width, height = game_data.split()[1:3]
    client["mape_size"] = [int(width), int(height)]
    print(f"Unused slot: {unused_slot}")
    print(f"Map size: {client['mape_size']}")

    game.add_ia_client(client)

    return int(unused_slot)
