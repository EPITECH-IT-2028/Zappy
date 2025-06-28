import pandas as pd

df = pd.read_csv('500hits.csv' , encoding='latin-1')
print("df.head(): ", df.head())

df = df.drop(columns= ['PLAYER', 'CS'])
X = df.iloc[:, 0:13]
y = df.iloc[:, 13]

from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=17, test_size=0.2)
print("\nX_train.shape(): ", X_train.shape)
print("\nX_test.shape(): ", X_test.shape)
print("\ny_train.shape(): ", y_train.shape)
print("\ny_test.shape(): ", y_test.shape)

from sklearn.tree import DecisionTreeClassifier

dtc = DecisionTreeClassifier()
print("\ndtc.get_params(): ", dtc.get_params())
print("\ndtc.fit(X_train, y_train): ", dtc.fit(X_train, y_train))
y_pred = dtc.predict(X_test)

from sklearn.metrics import confusion_matrix

print("\nconfusion_matrix(y_test, y_pred): ", confusion_matrix(y_test, y_pred))

from sklearn.metrics import classification_report

print("\nclassification_report(y_test, y_pred): ", classification_report(y_test, y_pred))
