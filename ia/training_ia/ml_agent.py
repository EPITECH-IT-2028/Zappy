import protocole
import game
import math
import random


def get_position(index, total_cells) -> tuple[int, int]:
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


def search_closest_food(client):
    vision_data = client["last_look"]
    closest_food = None
    min_distance = float('inf')

    for index, cell in enumerate(vision_data):
        if 'food' in cell:
            x, y = get_position(index, len(vision_data))
            distance = math.sqrt(x ** 2 + y ** 2)
            if distance < min_distance:
                min_distance = distance
                closest_food = {
                    'x': x,
                    'y': y,
                }

    return closest_food


def get_action(client):
    closest_food = search_closest_food(client)

    if closest_food is None:
        return "Right", None

    x, y = closest_food["x"], closest_food["y"]

    if x == 0 and y == 0:
        client["move"]["consecutive_turns"] = 0
        client["move"]["forward"] = False
        client["move"]["distance_to_food"] = -1
        client["move"]["last_food"] = None
        return "Take", "food"

    if x == 0 and y > 0:
        client["move"]["consecutive_turns"] = 0
        client["move"]["forward"] = True
        return "Forward", None

    if client["move"]["last_food"] == "right" and client["move"]["distance_to_food"] == 0:
        client["move"]["consecutive_turns"] += 1
        client["move"]["distance_to_food"] = -1
        return "Right", None

    if client["move"]["last_food"] == "left" and client["move"]["distance_to_food"] == 0:
        client["move"]["consecutive_turns"] += 1
        client["move"]["distance_to_food"] = -1
        return "Left", None

    if client["move"]["consecutive_turns"] >= 2:
        client["move"]["consecutive_turns"] = 0
        client["move"]["forward"] = True
        return "Forward", None

    if x > 0:
        client["move"]["last_food"] = "right"
        client["move"]["forward"] = True
        client["move"]["distance_to_food"] = y - 1
        return "Forward", None

    if x < 0:
        client["move"]["last_food"] = "left"
        client["move"]["forward"] = True
        client["move"]["distance_to_food"] = y - 1
        return "Forward", None

    client["move"]["consecutive_turns"] = 0
    client["move"]["forward"] = True
    return None, None


def get_food(client):
    action, arg = get_action(client)

    if action == "Take":
        protocole.execute_command(client, action, arg)
    else:
        protocole.execute_command(client, action, None)


def analyze_vision(client):
    vision_data = client["last_look"]
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


def strategy(client):
    vision_data = analyze_vision(client)

    if vision_data["food"]:
        get_food(client)
    else:
        client["move"]["consecutive_turns"] += 1
        if client["move"]["consecutive_turns"] >= 3:
            client["move"]["consecutive_turns"] = 0
            client["move"]["forward"] = True
            protocole.execute_command(client, "Forward", None)
        else:
            if random.random() < 0.7:
                protocole.execute_command(client, "Right", None)
            else:
                protocole.execute_command(client, "Forward", None)

    protocole.execute_command(client, "Inventory", None)

# TO DO: Implement strategy

# if (client["inventory"].get("food", 0) < 5):
#   focus on getting food until the food count is >= 10
# if (client["inventory"].get("food", 0) < 10):
#   focus on getting food jusqu'a ce que le nombre de nourriture soit >= 20 and resources if encountered
# else:
#   focus on other resources and food if encountered

# def get_cell_symbol(cell_content):
#         if 'food' in cell_content.lower():
#             return 'F'
#         elif cell_content.strip() == '':
#             return '.'
#         else:
#             return 'X'

# def print_vision_cone(vision_data):
#     if not vision_data:
#         return

#     current_cell = get_cell_symbol(vision_data[0])
#     print(f"[{current_cell}]")

#     index = 1
#     distance = 1

#     while index < len(vision_data):
#         line_size = distance * 2 + 1
#         line_end = min(index + line_size, len(vision_data))

#         line_cells = vision_data[index:line_end]
#         line_symbols = [get_cell_symbol(cell) for cell in line_cells]

#         spaces = " " * (4 - distance) if distance <= 4 else ""

#         line_display = " ".join(line_symbols)
#         print(f" {spaces}[{line_display}]")

#         index = line_end
#         distance += 1
