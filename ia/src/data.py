import sys
import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import os
import joblib
import time


DATASET_FILE = sys.argv[1]
MODEL_FILE = sys.argv[2]
FEATURES = [
    "food", "linemate", "sibur", "deraumere", "mendiane",
    "phiras", "thystame", "level", "players_nearby"
]
model = None

def load_data() -> pd.DataFrame:
    if os.path.exists(DATASET_FILE):
        return pd.read_csv(DATASET_FILE)
    return pd.DataFrame(columns=FEATURES + ["actions"])

def save_data(df:pd.DataFrame) -> None:
    df.to_csv(DATASET_FILE, index=False)

def record_state(state, action:str):
    df = load_data()
    new_row = {**state, "action": action}
    df = pd.concat([df, pd.DataFrame(new_row)], ignore_index=True)
    save_data(df)

def train_model() -> None:
    global model
    df = load_data()
    try:
        if len(df) < 10:
            e = "[ML] Not enough data to train model"
        else:
            X = df[FEATURES]
            y = df[["action"]]
            model = DecisionTreeClassifier()
            model.fit(X, y)
            joblib.dump(model, MODEL_FILE)
            print("[ML] Trained model and saved to", MODEL_FILE)
    except Exception as e:
        print(f"Error message: {e}", file=sys.stderr)
        (sys.exit(84))

def load_model() -> None:
    global model
    if os.path.exists(MODEL_FILE):
        model = joblib.load(MODEL_FILE)
    else:
        train_model()

def predict_action(state) -> model or str:
    global model
    if model is None:
        load_model()
    if model is None:
        return "Look"
    X = pd.DataFrame([state])[FEATURES]
    return model.predict(X)[0]