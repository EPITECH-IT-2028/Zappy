import protocole
import game
import math
import random
import utils

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

def search_closest_resource(client, needed_resources):
    vision_data = client["last_look"]
    closest_resource = None
    min_distance = float('inf')

    for index, cell in enumerate(vision_data):
        for resource_name in needed_resources.keys():
            if resource_name in cell.lower():
                x, y = get_position(index, len(vision_data))
                real_distance = math.sqrt(x*x + y*y)
                
                if real_distance < min_distance:
                    min_distance = real_distance
                    closest_resource = {
                        'x': x,
                        'y': y,
                        'real_distance': real_distance,
                        'resource_name': resource_name,
                        'content': cell
                    }
                break

    return closest_resource
      
def get_action(client, needed_resources):
  closest_resources = search_closest_resource(client, needed_resources)

  if closest_resources is None:
    return utils.RIGHT, None

  x, y = closest_resources["x"], closest_resources["y"]
  resources_name = closest_resources["resource_name"]

  if x == 0 and y == 0:
    client["move"]["consecutive_turns"] = 0
    client["move"]["forward"] = False
    client["move"]["distance_to_target"] = -1
    client["move"]["last_target"] = None
    return utils.TAKE, resources_name
  
  if x == 0 and y > 0:
    client["move"]["consecutive_turns"] = 0
    client["move"]["forward"] = True
    return utils.FORWARD, None
  
  if client["move"]["last_target"] == "right" and client["move"]["distance_to_target"] == 0:
    client["move"]["consecutive_turns"] += 1
    client["move"]["distance_to_target"] = -1
    return utils.RIGHT, None
  
  if client["move"]["last_target"] == "left" and client["move"]["distance_to_target"] == 0:
    client["move"]["consecutive_turns"] += 1
    client["move"]["distance_to_target"] = -1
    return utils.LEFT, None
  
  if client["move"]["consecutive_turns"] >= 2:
    client["move"]["consecutive_turns"] = 0
    client["move"]["forward"] = True
    return utils.FORWARD, None
  
  if x > 0:
    client["move"]["last_target"] = "right"
    client["move"]["forward"] = True
    client["move"]["distance_to_target"] = y - 1
    return utils.FORWARD, None
  
  if x < 0:
    client["move"]["last_target"] = "left"
    client["move"]["forward"] = True
    client["move"]["distance_to_target"] = y - 1
    return utils.FORWARD, None
  
  client["move"]["consecutive_turns"] = 0
  client["move"]["forward"] = True
  return None, None
 
def get_resources(client, needed_resources):
  action, arg = get_action(client, needed_resources)

  if action == utils.TAKE:
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

def define_needed_resources(client):
  needed = client.get("needed_resources", {})
  inventory = client.get("inventory", {})
  missing_resources = {}

  valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

  for resource, quantity in needed.items():
    if inventory.get(resource, 0) < quantity and resource in valid_resources:
      missing_resources[resource] = quantity - inventory.get(resource, 0)

  return missing_resources

def verify_incantation(client, current_cell, needed_resources):
    required_players = client["needed_resources"].get("nb_players", 0)
    count_players = current_cell.count("player")

    if needed_resources or not current_cell or count_players < required_players:
        client["incantation"] = False
        return
    
    client["incantation"] = True

def setting_up_incantation(client):
    valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
    dropped_resources = []

    for resource in valid_resources:
        quantity = client["inventory"].get(resource, 0)

        if quantity > 0:
            protocole.execute_command(client, utils.SET, resource)

        if quantity == 0:
            dropped_resources.append(resource)

    if len(dropped_resources) == len(valid_resources):
       return True
    else:
       return False  

def strategy(client):
    vision_data = analyze_vision(client)
    needed = define_needed_resources(client)
    food_count = client["inventory"].get("food", 0)
    current_cell = client["last_look"][0] if client["last_look"] else None

    if not client["incantation"]:
        verify_incantation(client, current_cell, needed)

    if client["incantation"]:
        print("Incantation is possible, setting up resources...")
        if setting_up_incantation(client):
            protocole.execute_command(client, utils.INCANTATION, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return

    if needed and current_cell:
        action, arg = get_action(client, needed)
        if action == utils.TAKE and arg in current_cell.lower():
            protocole.execute_command(client, action, arg)
            protocole.execute_command(client, utils.INVENTORY, None)
            return
      
    if food_count < 10:
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client["move"]["consecutive_turns"] += 1
            if client["move"]["consecutive_turns"] >= 3:
                client["move"]["consecutive_turns"] = 0
                client["move"]["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.7:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)

    elif food_count < 20:
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
            protocole.execute_command(client, utils.INVENTORY, None)
            return
        
        if needed:
            get_resources(client, needed)
            protocole.execute_command(client, utils.INVENTORY, None)
            return
        
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client["move"]["consecutive_turns"] += 1
            if client["move"]["consecutive_turns"] >= 3:
                client["move"]["consecutive_turns"] = 0
                client["move"]["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.7:
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
            client["move"]["consecutive_turns"] += 1
            if client["move"]["consecutive_turns"] >= 3:
                client["move"]["consecutive_turns"] = 0
                client["move"]["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.7:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)

    protocole.execute_command(client, utils.INVENTORY, None)
