import pandas as pd

d = {'sales': [100000, 222000, 1000000, 522000, 111111],
    'city': ['Tampa', 'Tampa', 'Orlando', 'Jacksonville', 'Miami'],
    'size': ['Small', 'Medium', 'Large', 'Large', 'Small']}

df = pd.DataFrame(data=d)
print("df.head: ", df.head())
print("\ndf['city']: ", df['city'].unique())

from sklearn.preprocessing import OneHotEncoder

ohe = OneHotEncoder(handle_unknown='ignore', sparse_output=False).set_output(transform='pandas')
ohetransform = ohe.fit_transform(df[['city']])
print("ohetransform: ", ohetransform)
df = pd.concat([df, ohetransform], axis=1).drop(columns=['city'])
print("df.head(10): ", df.head(10))
