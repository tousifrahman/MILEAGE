# from PyTsetlinMachineCUDA.tm import MultiClassConvolutionalTsetlinMachine2D
import numpy as np
from time import time
import cv2
from keras.datasets import fashion_mnist

factor = 20.0

s = 10.0

T = int(factor*25*10)

patch_size = 10

ensembles = 10
epochs = 250

(X_train, Y_train), (X_test, Y_test) = fashion_mnist.load_data()
X_train = np.copy(X_train)
X_test = np.copy(X_test)


# Initialize the grayscale arrays for the data
X_train_grayscale = np.empty([X_train.shape[0], 28*28], dtype=int)
X_test_grayscale = np.empty([X_test.shape[0], 28*28], dtype=int)

for i in range(X_train.shape[0]):
        gray = cv2.adaptiveThreshold(X_train[i], 1, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        gray = np.reshape(gray, (1, 28*28))
        X_train_grayscale[i] = gray

for i in range(X_test.shape[0]):
        gray = cv2.adaptiveThreshold(X_test[i], 1, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        gray = np.reshape(gray, (1, 28*28))
        X_test_grayscale[i] = gray

Y_test = np.vstack(Y_test)
X_test_file = np.hstack((X_test_grayscale,Y_test))
np.savetxt('FASHION_MNIST_test.txt', X_test_file, delimiter=' ', fmt='%i')

# X_train = np.reshape(X_train, (X_train.shape[0], 32*32*3))
Y_train = np.vstack(Y_train)
X_train_file = np.hstack((X_train_grayscale,Y_train))
np.savetxt('FASHION_MNIST_train.txt', X_train_file, delimiter=' ', fmt='%i')