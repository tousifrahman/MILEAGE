import numpy as np
from time import time
import cv2

path = "/home/tousif/ISTM_paper/K_MNIST/kmnist-master/"

X_train_compressed = np.load(path + "kmnist-train-imgs.npz")
y_train_compressed = np.load(path + "kmnist-train-labels.npz")

X_test_compressed = np.load(path  +"kmnist-test-imgs.npz")
y_test_compressed = np.load(path  +"kmnist-test-labels.npz")

keys = X_train_compressed.files
for item in keys:
        X_train_raw = X_train_compressed[item]

keys = X_test_compressed.files
for item in keys:
        X_test_raw = X_test_compressed[item]

keys = y_train_compressed.files
for item in keys:
        y_train_raw = y_train_compressed[item]

keys = y_test_compressed.files
for item in keys:
        y_test_raw = y_test_compressed[item]

# # Initialize the grayscale arrays for the data
X_train_grayscale = np.empty([X_train_raw.shape[0], 28*28], dtype=int)
X_test_grayscale = np.empty([X_test_raw.shape[0], 28*28], dtype=int)

for i in range(X_train_raw.shape[0]):
        gray = cv2.adaptiveThreshold(X_train_raw[i], 1, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        gray = np.reshape(gray, (1, 28*28))
        X_train_grayscale[i] = gray

for i in range(X_test_raw.shape[0]):
        gray = cv2.adaptiveThreshold(X_test_raw[i], 1, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        gray = np.reshape(gray, (1, 28*28))
        X_test_grayscale[i] = gray

Y_test = np.vstack(y_test_raw)
X_test_file = np.hstack((X_test_grayscale,Y_test))
np.savetxt('K_MNIST_test.txt', X_test_file, delimiter=' ', fmt='%i')

Y_train = np.vstack(y_train_raw)
X_train_file = np.hstack((X_train_grayscale,Y_train))
np.savetxt('K_MNIST_train.txt', X_train_file, delimiter=' ', fmt='%i')