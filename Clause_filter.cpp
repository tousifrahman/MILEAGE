/*
This is a modified version of the Tsetlin Machine C code from 
Ole-Christoffer Granmo. 

# Copyright (c) 2020 Ole-Christoffer Granmo
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

Some modifications have been done to allow for clause dropping at runtime.
*/

#include "MultiClassTsetlinMachine.h"
#include <iostream>
#include <iomanip> 
#include "Setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// pass threshold through the TM functions so it can be changed 
// however, it works fine to leave it constant.
int Threshold = 30; 

// re-work the approach -- only run the clause update when the training has stablized
// then run the inference -- then run the drop
// then wait again 
// then run the inference 

int X_train[NUMBER_OF_EXAMPLES_TRAIN][FEATURES];
int y_train[NUMBER_OF_EXAMPLES_TRAIN];

int X_test[NUMBER_OF_EXAMPLES_TEST][FEATURES];
int y_test[NUMBER_OF_EXAMPLES_TEST];

void read_file(void);

int main(void)
{	
	srand(time(NULL));
	read_file();

	// float initial_training_acc = 0.0;
	float current_train_acc = 1.0;
	float current_test_acc = 1.0;
	int epoch = 1;

	// FILE *TAs;
	// FILE *TAs_reordered;
	// FILE *Classes;
	std::cout.setf(std::ios::fixed, std::ios::floatfield);

	// Start initial TM setup 
	struct MultiClassTsetlinMachine *mc_tsetlin_machine = CreateMultiClassTsetlinMachine();
	mc_tm_initialize(mc_tsetlin_machine);

	// Initial training epoch 
	clock_t start_total = clock();
	clock_t end_total = clock();
	double time_used = 0.0;
	// mc_tm_fit(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, 1, S, Threshold);
	// clock_t end_total = clock();
	// double time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << "EPOCH 0" << " TRAINING TIME	: " << time_used << std::endl; 
	// start_total = clock();
	// current_train_acc = mc_tm_evaluate(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold);
	// end_total = clock();
	// time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << " INFERENCE TIME ON TRAINING SET: " << time_used << std::endl; 
	// std::cout << "Training Accuracy	: "  	<< current_train_acc*100 <<std::endl;
	// std::cout << "\n";

	// start_total = clock();
	// current_test_acc = mc_tm_evaluate(mc_tsetlin_machine, X_test, y_test, NUMBER_OF_EXAMPLES_TEST, Threshold);
	// end_total = clock();
	// time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << " INFERENCE TIME ON TESTING SET: " << time_used << std::endl; 
	// std::cout << "Test Accuracy	: "  	<< current_test_acc*100 <<std::endl;
	// std::cout << "\n";
	
	// start_total = clock();
	// mc_tm_fit(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, 1, S, Threshold);
	// end_total = clock();
	// time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << "EPOCH " << epoch << " TRAINING TIME	: " << time_used << std::endl; 
	// start_total = clock();
	// current_train_acc = mc_tm_evaluate(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold);
	// end_total = clock();
	// time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << " INFERENCE TIME ON TRAINING SET: " << time_used << std::endl; 
	// std::cout << "Training Accuracy	: "  	<< current_train_acc*100 <<std::endl;
	// std::cout << "\n";

	// start_total = clock();
	// current_test_acc = mc_tm_evaluate(mc_tsetlin_machine, X_test, y_test, NUMBER_OF_EXAMPLES_TEST, Threshold);
	// end_total = clock();
	// time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
	// std::cout << " INFERENCE TIME ON TESTING SET: " << time_used << std::endl; 
	// std::cout << "Test Accuracy	: "  	<< current_test_acc*100 <<std::endl;
	// std::cout << "\n";
	
	// epoch++;

	// Initial training loop to reach first equilibrium 
	for(epoch=0;epoch<25;epoch++)
	{
		// initial_training_acc = current_train_acc;
		start_total = clock();
		mc_tm_fit(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, 1, S, Threshold);
		end_total = clock();
		time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
		std::cout << "EPOCH " << epoch << " TRAINING TIME	: " << time_used << std::endl; 
	}

		start_total = clock();
		current_train_acc = mc_tm_evaluate(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold);
		end_total = clock();
		time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
		std::cout << "INFERENCE TIME ON TRAINING SET: " << time_used << std::endl; 
		std::cout << "Training Accuracy	: "  	<< current_train_acc*100 <<std::endl;
		std::cout << "\n";

		start_total = clock();
		current_test_acc = mc_tm_evaluate(mc_tsetlin_machine, X_test, y_test, NUMBER_OF_EXAMPLES_TEST, Threshold);
		end_total = clock();
		time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
		std::cout << "INFERENCE TIME ON TESTING SET: " << time_used << std::endl; 
		std::cout << "Test Accuracy	: "  	<< current_test_acc*100 <<std::endl;
		std::cout << "\n";

	// Keep running the training loop then downsize 
	float prev_best_acc = current_train_acc;
	while(epoch < EPOCH){

		if(current_train_acc <= prev_best_acc){
			start_total = clock();
			mc_tm_fit(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, 1, S, Threshold);
			end_total = clock();
			time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
			std::cout << "EPOCH " << epoch << " TRAINING TIME	: " << time_used << std::endl; 
			start_total = clock();
			current_train_acc = mc_tm_evaluate(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold);
			end_total = clock();
			time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
			
			std::cout << "INFERENCE TIME ON TRAINING SET: " << time_used << std::endl; 
			std::cout << "Training Accuracy	: "  	<< current_train_acc*100 <<std::endl;
			std::cout << "\n";

			start_total = clock();
			current_test_acc = mc_tm_evaluate(mc_tsetlin_machine, X_test, y_test, NUMBER_OF_EXAMPLES_TEST, Threshold);
			end_total = clock();
			time_used = ((double) (end_total - start_total)) / CLOCKS_PER_SEC;
			
			std::cout << "INFERENCE TIME ON TESTING SET: " << time_used << std::endl; 
			std::cout << "Test Accuracy	: "  	<< current_test_acc*100 <<std::endl;
			std::cout << "\n";
			epoch++;
		}
		else{
			prev_best_acc = current_train_acc;
			// rank clauses 
			mc_tm_evaluate_clause_strengths(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold);
			float compresison = 0.0;
			// downsize the TM
			current_train_acc = mc_tm_downsize(mc_tsetlin_machine, X_train, y_train, NUMBER_OF_EXAMPLES_TRAIN, Threshold, &compresison);
			std::cout <<"Compression - out: " << compresison << std::endl;
			// Threshold = initial_T*(1-compresison);
			std::cout << "Clauses Dropped!, Threshold:" << Threshold << std::endl;
			std::cout << "Training Accuracy	after dropping: "  	<< current_train_acc*100 <<std::endl;			
		}
	}

	// Write the model to file 
	FILE *TAs_reordered;
	FILE *Classes;
	FILE *indexes;
	FILE *weights;

	// TM reordered Clauses - all the remaining +ve clauses and then remaining -ve clauses
	TAs_reordered = fopen ("TAs_reordered.txt","w");
	Classes = fopen ("classes.txt","w");
	indexes= fopen ("indexes.txt","w");
	weights = fopen("weights.csv","w"); // not using for now 
	mc_tm_write_reordred_tas(mc_tsetlin_machine, TAs_reordered, Classes, indexes, weights);

	return 0;
}

void read_file(void)
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t bytes_written = 0; //new
	const char *s = " ";
	char *token = NULL;

	fp = fopen(TRAIN_FILE, "r");
	if (fp == NULL) {
		printf("Error opening\n");
		exit(EXIT_FAILURE);
	} 

	for (int i = 0; i < NUMBER_OF_EXAMPLES_TRAIN; i++) {
		bytes_written += getline(&line, &len, fp);
	
		token = strtok(line, s);
		for (int j = 0; j < FEATURES; j++) {
			X_train[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		y_train[i] = atoi(token);
	}

	fp = fopen(TEST_FILE, "r");
	if (fp == NULL) {
		printf("Error opening\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBER_OF_EXAMPLES_TEST; i++) {
		bytes_written += getline(&line, &len, fp);

		token = strtok(line, s);
		for (int j = 0; j < FEATURES; j++) {
			X_test[i][j] = atoi(token);
			token=strtok(NULL,s);
		}
		y_test[i] = atoi(token);
	}
}