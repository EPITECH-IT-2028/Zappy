#python version 3.8
from keras import *

model = Sequential()

model.add(layers.Dense(units=3, input_shape=[1]))
model.add(layers.Dense(units=64))
model.add(layers.Dense(units=1))

x_input = [1,2,3,4,5]
x_output = [2,4,6,8,10]

model.compile(loss='mean_squared_error', optimizer='adam')
model.fit(x=x_input, y=x_output, epochs=1000)

while True:
    x = int(input('Nombre :'))
    print('Prediction :', str(model.predict([x])))
