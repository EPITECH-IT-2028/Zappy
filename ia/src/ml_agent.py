import protocole
import game
import math
import random
import utils

def create_help_message(level, food_count):
    return f"I_need_help_to_level_up_to_{level + 1}_with_{food_count}"

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
    vision_data = client.last_look
    closest_resource = None
    min_distance = float('inf')

    for index, cell in enumerate(vision_data):
        for resource_name in needed_resources.keys():
            if resource_name in cell.lower():
                x, y = get_position(index, len(vision_data))
                distance = math.sqrt(x*x + y*y)
                
                if distance < min_distance:
                    min_distance = distance
                    closest_resource = {
                        'x': x,
                        'y': y,
                        'resource_name': resource_name,
                    }
                break

    return closest_resource
      
def get_action(client, needed_resources):
  closest_resources = search_closest_resource(client, needed_resources)

  if closest_resources is None:
    if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
            client.move["consecutive_turns"] = 0
            client.move["forward"] = True
            return utils.FORWARD, None
    else:
        if random.random() < 0.5:
            client.move["consecutive_turns"] += 1
            return utils.RIGHT, None
        else:
            client.move["consecutive_turns"] = 0
            return utils.FORWARD, None

  x, y = closest_resources["x"], closest_resources["y"]
  resources_name = closest_resources["resource_name"]

  if x == 0 and y == 0:
    client.move["consecutive_turns"] = 0
    client.move["forward"] = False
    client.move["distance_to_target"] = utils.NO_TARGET
    client.move["last_target"] = None
    return utils.TAKE, resources_name
  
  if x == 0 and y > 0:
    client.move["consecutive_turns"] = 0
    client.move["forward"] = True
    return utils.FORWARD, None
  
  if client.move["last_target"] == "right" and client.move["distance_to_target"] == 0:
    client.move["consecutive_turns"] += 1
    client.move["distance_to_target"] = utils.NO_TARGET
    return utils.RIGHT, None
  
  if client.move["last_target"] == "left" and client.move["distance_to_target"] == 0:
    client.move["consecutive_turns"] += 1
    client.move["distance_to_target"] = utils.NO_TARGET
    return utils.LEFT, None
  
  if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
    client.move["consecutive_turns"] = 0
    client.move["forward"] = True
    return utils.FORWARD, None
  
  if x > 0:
    client.move["last_target"] = "right"
    client.move["forward"] = True
    client.move["distance_to_target"] = y - 1
    return utils.FORWARD, None
  
  if x < 0:
    client.move["last_target"] = "left"
    client.move["forward"] = True
    client.move["distance_to_target"] = y - 1
    return utils.FORWARD, None
  
  client.move["consecutive_turns"] = 0
  client.move["forward"] = True
  return None, None
 
def get_resources(client, needed_resources):
  action, arg = get_action(client, needed_resources)

  if action == utils.TAKE:
    protocole.execute_command(client, action, arg)
  else:
    protocole.execute_command(client, action, None)

def analyze_vision(client):
  vision_data = client.last_look
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
  needed = client.needed_resources
  inventory = client.inventory
  missing_resources = {}

  valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

  for resource, quantity in needed.items():
    if inventory.get(resource, 0) < quantity and resource in valid_resources:
      missing_resources[resource] = quantity - inventory.get(resource, 0)

  return missing_resources

def verify_incantation(client, current_cell, needed_resources):
    required_players = utils.MAX_REQUIRED_PLAYERS
    count_players = current_cell.count("player")

    if client.level == 1:
        required_players = utils.MIN_REQUIRED_PLAYERS

    if needed_resources:
        client.incantation = False
        return
    
    if count_players < required_players:
        client.incantation = False
        client.waiting_for_help = True
        protocole.execute_command(client, utils.BROADCAST, create_help_message(client.level, client.inventory.get("food", 0)))
        return
    
    client.waiting_for_help = False
    client.incantation = True

def setting_up_incantation(client, current_cell):
    valid_resources = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
    count_players = current_cell.count("player")
    required_players = utils.MAX_REQUIRED_PLAYERS

    if client.level == 1:
        required_players = utils.MIN_REQUIRED_PLAYERS

    for resource in valid_resources:
        quantity = client.inventory.get(resource, 0)

        if quantity > 0:
            protocole.execute_command(client, utils.SET, resource)

        if current_cell.count(resource) < client.needed_resources.get(resource, 0):
            return False
        
    if count_players < required_players:
        protocole.execute_command(client, utils.BROADCAST, create_help_message(client.level, client.inventory.get("food", 0)))
        return False
    
    return True
    
def get_action_from_broadcast(client, direction):
    direction_actions = {
        0: [],
        1: [utils.FORWARD],
        2: [utils.FORWARD, utils.LEFT],
        3: [utils.LEFT, utils.FORWARD],
        4: [utils.LEFT, utils.FORWARD, utils.LEFT],
        5: [utils.LEFT, utils.LEFT, utils.FORWARD],
        6: [utils.RIGHT, utils.FORWARD, utils.RIGHT],
        7: [utils.RIGHT, utils.FORWARD],
        8: [utils.FORWARD, utils.RIGHT],
    }

    actions = direction_actions.get(direction, [])

    for action in actions:
        protocole.execute_command(client, action, None)
    
    client.help_direction = 0

def strategy(client):
    vision_data = analyze_vision(client)
    needed = define_needed_resources(client)
    food_count = client.inventory.get("food", 0)
    current_cell = client.last_look[utils.PLAYER_CELL] if client.last_look else None

    if not client.start_playing:
        if client.player_in_game == utils.MAX_REQUIRED_PLAYERS:
            client.start_playing = True
            protocole.execute_command(client, utils.INVENTORY, None)
            return

        if not client.at_max_food and food_count >= utils.MAX_FOOD:
            client.at_max_food = True
            client.player_in_game += 1
            protocole.execute_command(client, utils.BROADCAST, utils.START_MESSAGE)
    
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return
    
    if food_count < utils.CRITICAL_FOOD:
       client.status = "critique"

    if client.status == "critique":
       if food_count > utils.GOOD_FOOD:
          client.status = "good"

    if client.status == "critique":
        client.help_status = False
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
                    protocole.execute_command(client, utils.RIGHT, None)
                else:
                    protocole.execute_command(client, utils.FORWARD, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return
    
    if client.help_status == True:
        get_action_from_broadcast(client, client.help_direction)
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
        protocole.execute_command(client, utils.INVENTORY, None)
        return
    
    if not client.incantation:
        verify_incantation(client, current_cell, needed)

    if client.incantation:
        if setting_up_incantation(client, current_cell):
            protocole.execute_command(client, utils.INCANTATION, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        return
    
    if client.waiting_for_help:
        if len(client.commands) > utils.MAX_COMMANDS - 1:
            protocole.execute_command(client, utils.INVENTORY, None)
            return
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.RIGHT, None)
        protocole.execute_command(client, utils.INVENTORY, None)
        protocole.execute_command(client, utils.BROADCAST, create_help_message(client.level, client.inventory.get("food")))
        return
    
    if needed and current_cell:
        action, arg = get_action(client, needed)
        if action == utils.TAKE and arg in current_cell.lower():
            protocole.execute_command(client, action, arg)
            protocole.execute_command(client, utils.INVENTORY, None)
            return

    if food_count < 20 and client.status == "good":
        if "food" in current_cell.lower():
            protocole.execute_command(client, utils.TAKE, "food")
            protocole.execute_command(client, utils.INVENTORY, None)
            return
        
        for resource, quantity in needed.items():
          if resource in current_cell.lower():
              get_resources(client, {resource: quantity})
              protocole.execute_command(client, utils.INVENTORY, None)
              return
        
        if vision_data["food"]:
            get_resources(client, {"food": 1})
        else:
            client.move["consecutive_turns"] += 1
            if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                client.move["consecutive_turns"] = 0
                client.move["forward"] = True
                protocole.execute_command(client, utils.FORWARD, None)
            else:
                if random.random() < 0.5:
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
            if vision_data["food"]:
                get_resources(client, {"food": 1})
            else:
                if client.move["consecutive_turns"] >= utils.MAX_CONSECUTIVE_TURNS:
                    client.move["consecutive_turns"] = 0
                    client.move["forward"] = True
                    protocole.execute_command(client, utils.FORWARD, None)
                else:
                    if random.random() < 0.5:
                        client.move["consecutive_turns"] += 1
                        protocole.execute_command(client, utils.RIGHT, None)
                    else:
                        protocole.execute_command(client, utils.FORWARD, None)

    protocole.execute_command(client, utils.INVENTORY, None)
