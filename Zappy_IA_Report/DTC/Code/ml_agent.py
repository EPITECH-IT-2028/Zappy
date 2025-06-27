import joblib
import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import os
import random
import numpy as np

stones = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
requirements = {
    2: {"linemate": 1, "deraumere": 1, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
    3: {"linemate": 2, "deraumere": 1, "sibur": 1, "mendiane": 1, "phiras": 0, "thystame": 0},
    4: {"linemate": 2, "deraumere": 2, "sibur": 0, "mendiane": 1, "phiras": 0, "thystame": 2},
    5: {"linemate": 4, "deraumere": 1, "sibur": 1, "mendiane": 2, "phiras": 0, "thystame": 1},
    6: {"linemate": 4, "deraumere": 1, "sibur": 2, "mendiane": 1, "phiras": 3, "thystame": 0},
    7: {"linemate": 6, "deraumere": 1, "sibur": 2, "mendiane": 3, "phiras": 0, "thystame": 1},
    8: {"linemate": 6, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1}
}

FEATURES = [
    'level', 'food_inv', 'missing_food', 'food_here',
    'linemate_here', 'linemate_needed', 'linemate_inv',
    'deraumere_here', 'deraumere_needed', 'deraumere_inv',
    'sibur_here', 'sibur_needed', 'sibur_inv',
    'mendiane_here', 'mendiane_needed', 'mendiane_inv',
    'phiras_here', 'phiras_needed', 'phiras_inv',
    'thystame_here', 'thystame_needed', 'thystame_inv'
]

MODEL_FILE = 'zappy_tree_model.pkl'
model = None
feature_names = FEATURES


def generate_synthetic_data(num_samples=1000):
    data = []

    for _ in range(num_samples):
        level = random.randint(1, 7)
        food_inv = random.randint(0, 10)
        missing_food = 1 if food_inv < 5 else 0
        food_here = random.choice([0, 1])

        stone_features = {}
        next_level = level + 1
        req = requirements.get(next_level, {})

        for stone in stones:
            stone_features[f"{stone}_here"] = random.choice([0, 1])
            stone_features[f"{stone}_needed"] = 1 if req.get(stone, 0) > 0 else 0
            stone_features[f"{stone}_inv"] = random.randint(0, req.get(stone, 0) + 1)

        if missing_food and food_here:
            action = "Take food"
        else:
            stone_taken = False
            for stone in stones:
                if stone_features[f"{stone}_needed"] and stone_features[f"{stone}_here"]:
                    action = f"Take {stone}"
                    stone_taken = True
                    break
            if not stone_taken:
                action = random.choice(["Forward", "Right", "Left", "Inventory"])

        features = [level, food_inv, missing_food, food_here]
        for stone in stones:
            features.append(stone_features[f"{stone}_here"])
            features.append(stone_features[f"{stone}_needed"])
            features.append(stone_features[f"{stone}_inv"])

        data.append(features + [action])

    return pd.DataFrame(data, columns=FEATURES + ["action"])


def train_model(df):
    X = df[FEATURES]
    y = df["action"]

    clf = DecisionTreeClassifier(criterion="gini", max_depth=5, random_state=1)
    clf.fit(X, y)

    return clf


def save_model(clf):
    joblib.dump(clf, MODEL_FILE)


def load_model():
    return joblib.load(MODEL_FILE)


def load_or_train_model():
    global model, feature_names

    if os.path.exists(MODEL_FILE):
        model = load_model()
        print(f"Model loaded from {model}")
        print("Loaded pre-trained decision tree model")
    else:
        print("Training new decision tree model...")
        df = generate_synthetic_data(5000)
        model = train_model(df)
        print(f"Saved pre-trained decision tree model {model}")
        save_model(model)
        print("Trained and saved new model")

    return model


def extract_features(client):
    inv = client["inventory"]
    look = client["last_look"]
    level = client["level"]

    current_tile = look[0].split() if look and look[0] else []

    food_inv = inv.get("food", 0)
    missing_food = 1 if food_inv < 5 else 0
    food_here = 1 if "food" in current_tile else 0

    features = [level, food_inv, missing_food, food_here]

    next_level = level + 1
    req = requirements.get(next_level, {})

    for stone in stones:
        stone_here = 1 if stone in current_tile else 0
        features.append(stone_here)

        needed = 1 if req.get(stone, 0) > 0 else 0
        features.append(needed)

        stone_inv = inv.get(stone, 0)
        features.append(stone_inv)

    return features


def decision_tree_strategy(client):
    global model

    if model is None:
        model = load_or_train_model()
        print(f"model -> {model}")

    if not client["is_alive"]:
        return

    food_count = client["inventory"].get("food", 0)
    print(f"food_count -> {food_count}")
    if food_count < 2:  # Critical food level
        current_tile = client["last_look"][0].split() if client["last_look"] else []
        if "food" in current_tile:
            execute_command(client, "Take", "food")
            return
        else:
            execute_command(client, random.choice(["Forward", "Left", "Right"]), None)
            return

    features = extract_features(client)
    features_df = pd.DataFrame([features], columns=feature_names)
    action = model.predict(features_df)[0]

    if action == "Take food":
        execute_command(client, "Take", "food")
    elif action.startswith("Take "):
        stone = action.split()[1]
        execute_command(client, "Take", stone)
    else:
        execute_command(client, action, None)


def execute_command(client, command, arg):
    client["last_command"] = command
    client["last_command_args"] = arg

    if command in ["Take", "Set"]:
        client["socket"].send(f"{command} {arg}\n".encode())
    else:
        client["socket"].send(f"{command}\n".encode())

    if len(client["commandes"]) < 10:
        client["commandes"].append(command)


def strategy(client):
    try:
        decision_tree_strategy(client)
    except Exception as e:
        print(f"AI error: {e}")
        execute_command(client, random.choice(["Forward", "Left", "Right"]), None)