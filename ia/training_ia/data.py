import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import os
import joblib

DATASET_PATH = "data/dataset.csv"
MODEL_PATH = "data/model.pkl"
FEATURES = [
    "food", "linemate", "sibur", "deraumere", "mendiane",
    "phiras", "thystame", "level", "players_nearby"
]

model = None

def load_dataset():
    if os.path.exists(DATASET_PATH):
        # print("Loading dataset...")
        # print(pd.read_csv(DATASET_PATH))
        return pd.read_csv(DATASET_PATH)
    return pd.DataFrame(columns=FEATURES + ["action"])

def save_dataset(df):
    df.to_csv(DATASET_PATH, index=False)

def record_state(state, action):
    df = load_dataset()
    new_row = {**state, "action": action}
    df = pd.concat([df, pd.DataFrame([new_row])], ignore_index=True)
    save_dataset(df)

def train_model():
    global model
    df = load_dataset()
    if len(df) < 10:
        print("[ML] Pas assez de données pour entraîner le modèle.")
        return
    X = df[FEATURES]
    y = df["action"]
    model = DecisionTreeClassifier()
    model.fit(X, y)
    joblib.dump(model, MODEL_PATH)
    print("[ML] Modèle entraîné et sauvegardé.")

def load_model():
    global model
    print("-----------------------------------------------------------------",MODEL_PATH)
    if os.path.exists(MODEL_PATH):
        model = joblib.load(MODEL_PATH)
        result = model.predict(load_dataset())
    else:
        print("ca va train model\n")
        train_model()

def predict_action(state):
    global model
    if model is None:
        print("premiere condition\n")
        load_model()
    # if model is None:
    #     return "Look"
    X = pd.DataFrame([state])[FEATURES]
    return model.predict(X)[0]
