// Dummy header file for placing new values

#ifndef SETUP_H
#define SETUP_H

#define S 10
// #define THRESHOLD 5.0  // changed at runtime now 
#define CLAUSES 1000
#define CLASSES 10 
#define FEATURES 784
#define NUMBER_OF_EXAMPLES_TRAIN 60000
#define NUMBER_OF_EXAMPLES_TEST 10000
#define DIFF_TRAIN_ACC 0.005



#define NUMBER_OF_STATES 100
#define BOOST_TRUE_POSITIVE_FEEDBACK 1
#define PREDICT 1
#define UPDATE 0
#define cR 10
constexpr const char* TRAIN_FILE = "FASHION_MNIST_train.txt";
constexpr const char* TEST_FILE = "FASHION_MNIST_test.txt";

#define EPOCH 200
#endif //USERSETUP_H