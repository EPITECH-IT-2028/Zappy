def parse_look_response(response: str) -> list:
    cleaned = response.strip().lstrip('[').rstrip(']')
    return [tile.strip() for tile in cleaned.split(',')]

def count_players_from_look(look_tiles: list) -> int:
    count = 0
    for tile in look_tiles:
        objects = tile.split()
        count += objects.count("player")
    return count

def build_state(client: dict, inventory: dict, look_response: str) -> dict:
    look_tiles = parse_look_response(look_response)
    players_nearby = count_players_from_look(look_tiles)

    return {
        "food": inventory.get("food", 0),
        "linemate": inventory.get("linemate", 0),
        "sibur": inventory.get("sibur", 0),
        "deraumere": inventory.get("deraumere", 0),
        "mendiane": inventory.get("mendiane", 0),
        "phiras": inventory.get("phiras", 0),
        "thystame": inventory.get("thystame", 0),
        "level": client.get("level", 1),
        "players_nearby": players_nearby
    }
