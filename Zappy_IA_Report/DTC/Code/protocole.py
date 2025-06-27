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
    if response == "ko":
        print("Take failed - no resource available")
        if client["last_look"] and client["last_command"] == "Take":
            resource = client["last_command_args"]
            current_tile = client["last_look"][0].split()
            if resource in current_tile:
                current_tile.remove(resource)
                client["last_look"][0] = " ".join(current_tile)
        execute_command(client, random.choice(["Forward", "Left", "Right"]), None)
    else:
        print("Take succeeded")
        execute_command(client, "Inventory", None)


def handle_Set(client, response) -> None:
    send_message(client, "Inventory")


def handle_Inventory(client, response) -> None:
    try:
        cleaned = response.strip().lstrip('[').rstrip(']')
        items = [item.strip() for item in cleaned.split(',') if item.strip()]

        client["inventory"] = {}

        for item in items:
            parts = item.split()
            if len(parts) >= 2:
                resource = parts[0]
                quantity = int(parts[1])
                client["inventory"][resource] = quantity
    except Exception as e:
        print(f"Inventory parsing error: {e}")


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


def handle_message(client, message):
    if message == "dead":
        handle_Dead(client, message)
        return

    if client["commandes"]:
        command = client["commandes"].pop(0)
        handle_commande(client, command, message)

        if command != "Dead":
            ml_agent.strategy(client)


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
    execute_command(client, "Look", None)  # First look command
    while client["is_alive"]:
        try:
            data = client["socket"].recv(1024).decode()
            if not data:
                print("Server closed connection")
                handle_Dead(client, "dead")
                break
            buffer += data
            while "\n" in buffer:
                message, buffer = buffer.split("\n", 1)
                message = message.strip()
                if not message:
                    continue
                if message == "dead":
                    handle_Dead(client, message)
                    break
                if client["commandes"]:
                    command = client["commandes"].pop(0)
                    handle_commande(client, command, message)
                    if command in ["Look", "Inventory"]:
                        ml_agent.strategy(client)
        except ConnectionResetError:
            print("Connection reset by server")
            handle_Dead(client, "dead")
            break
        except Exception as e:
            print(f"Unexpected error: {e}")
            handle_Dead(client, "dead")
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
        client["socket"].close()
        return 0

    unused_slot = game_data.split()[0]
    print(f"Game data received: {game_data}")

    width, height = game_data.split()[1:3]
    client["mape_size"] = [int(width), int(height)]
    print(f"Unused slot: {unused_slot}")
    print(f"Map size: {client['mape_size']}")

    game.add_client(client)

    return int(unused_slot)