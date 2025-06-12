import sys
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, MinMaxScaler, OneHotEncoder


def open_csv(file: str):
    df = pd.read_csv(file, encoding='utf-8')
    # print(df.head())
    X = df.drop(columns=['current_level', 'food_units'])
    # print(df.info())
    y = df['current_level']
    # print(X.head())
    # print(X.shape)
    # print(y.shape)
    # print(y.head())
    X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=11, test_size=0.2)
    # print(X_train.shape)
    # print(X_test.shape)
    # print(y_train.shape)
    # print(y_test.shape)
    # print(X_train.head())
    # print(X_train.describe().round(2))
    # print(X_test.describe().round(2))
    X1 = df.iloc[:, 0:16]
    X2 = df.iloc[:, 0:16]
    scaler = StandardScaler()
    X1 = scaler.fit_transform(X1)
    X1 = pd.DataFrame(X1, columns=['linemate', 'deraumere', 'sibur', 'mendiane', 'phiras', 'thystame',
                                'players_same_level_nearby', 'team_players_nearby', 'enemy_players_nearby',
                                'food_on_tile', 'resources_on_tile', 'time_since_last_action', 'vision_range',
                                'map_position_x', 'map_position_y', 'action'])
    # print("X1 head ---> \n", X1.head())
    # print("X1 round(2) ---> \n", X1.describe().round(2))
    scaleMinMax = MinMaxScaler(feature_range=(0, 1))
    X2 = scaleMinMax.fit_transform(X2)
    X2 = pd.DataFrame(X2, columns=['linemate', 'deraumere', 'sibur', 'mendiane', 'phiras', 'thystame',
                                'players_same_level_nearby', 'team_players_nearby', 'enemy_players_nearby',
                                'food_on_tile', 'resources_on_tile', 'time_since_last_action', 'vision_range',
                                'map_position_x', 'map_position_y', 'action'])
    # print("X2 head ---> \n", X2.head())
    # print("X1 round(2) ---> \n", X2.describe().round(2))
    print("df head ---> \n", df.head())
    print("df['current_level'].unique\n", df['current_level'].unique())
    ohe = OneHotEncoder(handle_unknown='ignore', sparse_output=False).set_output(transform='pandas')
    ohetransform = ohe.fit_transform(df[['current_level']])
    print("ohetransform ---> \n", ohetransform)
    print("ohetransform ---> \n", ohetransform.head())
    df = pd.concat([df, ohetransform], axis=1).drop(columns=['action'])
    print("df head ---> \n", df.head(30))
