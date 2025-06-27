import pandas as pd
from skelarn.model_selection import train_test_split

df = pd.read_csv("data/dataset.csv", encodings = 'latin-1')
df.head()
X = df.drop(columns=['level'])
y = df['level']
print(X.head())
print(X.shape())
print(y.shape())
print(y.head())

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=11)
print(X_train.shape())
print(X_test.shape())
print(y_train.shape())
print(y_test.shape())
print(X_train.head())
print(X_train.describe().round(3))