import pandas as pd 
from sklearn.model_selection import train_test_split

df = pd.read_csv('500hits.csv', encoding = 'latin-1')

print("df.head: ", df.head())

X = df.drop(columns=['PLAYER', 'HOF'])
y = df['HOF']

print("\nX.head: ", X.head())
print("\nX.shape: ", X.shape)
print("\ny.head: ", y.head())
print("\ny.shape: ", y.shape)

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=11, test_size=0.2)

print("\nX_train.shape: ", X_train.shape)
print("\nX_test.shape: ", X_test.shape)
print("\ny_train.shape: ", y_train.shape)
print("\ny_test.shape: ", y_test.shape)
print("\nX_train.head: ", X_train.head())

print("\nX_train.describe: ", X_train.describe().round(3))
print("\nX_test.describe: ", X_test.describe().round(3))