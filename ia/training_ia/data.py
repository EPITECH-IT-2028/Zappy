import sys
import pandas as pd


def open_csv(file: str):
     df = pd.read_csv(file, encoding='utf-8')
     print(df.head())
     df = df.drop(columns=[''])