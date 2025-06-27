import pandas as pd

df = pd.read_csv('500hits.csv', encoding='latin-1')
print(df.head())
df = df.drop(columns=['PLAYER', 'CS'])
print(df.info())
print(df.describe().round(3))
X1 = df.iloc[:,0:13]
X2 = df.iloc[:,0:13]

from sklearn.preprocessing import StandardScaler

scaleStandard = StandardScaler()
X1 = scaleStandard.fit_transform(X1)
X1 = pd.DataFrame(X1, columns=['YRS', 'G', 'AB', 'R', 'H', '2B', '3B', 'HR', 'RBI','BB','SO', 'SB'])
print(X1.head())
print(X1.describe().round(3))

from sklearn.preprocessing import MinMaxScaler

scaleMinMax = MinMaxScaler(feature_range=(0,1))
X2 = scaleMinMax.fit_transform(X2)
X2 = pd.DataFrame(X2, columns=['YRS', 'G', 'AB', 'R', 'H', '2B', '3B', 'HR', 'RBI','BB','SO', 'SB'])
print(X2.head())
print(X2.describe().round(3))
