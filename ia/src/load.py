# import tensorflow as tf
# from tensorflow import keras
# import numpy as np
# import matplotlib.pyplot as plt
#
# def predict_data():
#     data = keras.datasets.fashion_mnist
#     (train_images, train_labels), (test_images, test_labels) = data.load_data()
#     class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
#                    'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']
#     train_images = train_images / 255.0
#     test_images = test_images / 255.0
#
#     model = keras.Sequential([
#         keras.layers.Flatten(input_shape=(28, 28)),
#         keras.layers.Dense(128, activation="relu"),
#         keras.layers.Dense(10, activation="softmax")
#     ])
#     model.compile(optimizer="adam", loss="sparse_categorical_crossentropy", metrics=["accuracy"])
#     model.fit(train_images, train_labels, epochs=5)
#     # test_loss, test_acc = model.evaluate(test_images, test_labels)
#     # print("Test accuracy: ",test_acc)
#     predictions = model.predict(test_images)
#     # print("Predictions: ", predictions[0])
#     # print("Predicted class: ", class_names[np.argmax(predictions[0])])
#     for i in range(5):
#         plt.grid(False)
#         plt.imshow(test_images[i], cmap=plt.cm.binary)
#         plt.xlabel("Actual: " + class_names[test_labels[i]])
#         plt.title("Predicted: " + class_names[np.argmax(predictions[i])])
#         plt.show()
#
#
# def decode_review(text, reverse_word_index):
#     return ' '.join([reverse_word_index.get(i, '?') for i in text])


def load():
    data = keras.datasets.imdb
    (train_data, train_labels), (test_data, test_labels) = data.load_data(num_words=10000)
    print(train_data[0])
    word_index = data.get_word_index()
    word_index = {k: (v + 3) for k, v in word_index.items()}
    word_index["<PAD>"] = 0
    word_index["<START>"] = 1
    word_index["<UNK>"] = 2
    word_index["<UNUSED>"] = 3
    reverse_word_index = dict([(value, key) for (key, value) in word_index.items()])
    train_data = keras.preprocessing.sequence.pad_sequences(train_data, value=word_index["<PAD>"], padding='post', maxlen=250)
    test_data = keras.preprocessing.sequence.pad_sequences(test_data, value=word_index["<PAD>"], padding='post', maxlen=250)

    # print(decode_review(test_data[], reverse_word_index))
    print(len(test_data[0]), len(test_data[0]))
