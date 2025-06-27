import pandas as pd

df = pd.read_csv('500hits.csv', encoding='latin-1')
print(df.head())

df = df.drop(columns= ['PLAYER', 'CS'])
X = df.iloc[:, 0:13]
y = df.iloc[:, 13]

from sklear.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=17, test_size=0.2)
print(X_train.shape())
print(X_test.shape())
print(y_train.shape())
print(y_test.shape())

from sklearn.tree import DecisionTreeClassifier

dtc = DecisionTreeClassifier()
print(dtc.get_params())
print(dtc.fit(X_train, y_train))
y_pred = dtc.predict(X_test)

from sklearn.metrics import confusion_matrix

print(confusion_matrix(y_test, y_pred))

from sklearn.metrics import classification_report

print(classification_report(y_test, y_pred))

