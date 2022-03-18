#include <stdio.h>
#include <stdlib.h>
// #include <random>      // switched to C based shuffling instead
// #include <algorithm>
// #include <iterator>
#include "MultiClassTsetlinMachine.h"

void shuffle(int *array, size_t n);

/**************************************/
/*** The Multiclass Tsetlin Machine ***/
/**************************************/
/*** Initialize Tsetlin Machine ***/
struct MultiClassTsetlinMachine *CreateMultiClassTsetlinMachine()
{
	struct MultiClassTsetlinMachine *mc_tm;
    mc_tm = new struct MultiClassTsetlinMachine;
	for (int i = 0; i < CLASSES; i++) {
		mc_tm->tsetlin_machines[i] = CreateTsetlinMachine();
	}
	return mc_tm;
}

void mc_tm_initialize(struct MultiClassTsetlinMachine *mc_tm)
{
	for (int i = 0; i < CLASSES; i++) {
		tm_initialize(mc_tm->tsetlin_machines[i]);
	}
}

/********************************************/
/*** Evaluate the Trained Tsetlin Machine ***/
/********************************************/
void mc_tm_evaluate_clause_strengths(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples, int Threshold)
{
	int max_class;
	int max_class_sum;
	for(int i = 0; i< CLASSES; i++){
		clear_clauses(mc_tm->tsetlin_machines[i]);
	}
	for (int l = 0; l < number_of_examples; l++) {
		/******************************************/
		/*** Identify Class with Largest Output ***/
		/******************************************/
		max_class_sum = tm_score(mc_tm->tsetlin_machines[0], X[l], Threshold);
		max_class = 0;
		for (int i = 1; i < CLASSES; i++) {	
			int class_sum = tm_score(mc_tm->tsetlin_machines[i], X[l], Threshold);
			if (max_class_sum < class_sum) {
				max_class_sum = class_sum;
				max_class = i;
			}
		}
		for(int i = 0; i < CLASSES; i++){
			update_clause_strengths(mc_tm->tsetlin_machines[i], max_class, y[l], i);
		}
	}
	for(int i = 0; i < CLASSES; i++){
		print_clause_strengths(mc_tm->tsetlin_machines[i]);    
	}
}

float mc_tm_evaluate(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples, int Threshold)
{
	int errors;
	int max_class;
	int max_class_sum;
	errors = 0;
	for (int l = 0; l < number_of_examples; l++) {
		/******************************************/
		/*** Identify Class with Largest Output ***/
		/******************************************/
		max_class_sum = tm_score(mc_tm->tsetlin_machines[0], X[l], Threshold);
		max_class = 0;
		for (int i = 1; i < CLASSES; i++) {	
			int class_sum = tm_score(mc_tm->tsetlin_machines[i], X[l], Threshold);
			if (max_class_sum < class_sum) {
				max_class_sum = class_sum;
				max_class = i;
			}
		}
		// std::cout << "max_class: " << max_class << " y: " << y[l] << std::endl;
		if (max_class != y[l]) {
			errors += 1;
		}
		// std::cout << "errors: " << errors <<std::endl;
	}
	return 1.0 - (1.0 * errors / number_of_examples);
}

void mc_tm_write_tas(struct MultiClassTsetlinMachine *mc_tm, FILE *ptr){
	for(int i = 0; i < CLASSES; i++){
		write_tas(mc_tm->tsetlin_machines[i], ptr);
	}
}

void mc_tm_write_reordred_tas(struct MultiClassTsetlinMachine *mc_tm, FILE *ptr, FILE *ptr2, FILE *ptr3, FILE *ptr4){
	for(int i = 0; i < CLASSES; i++){
		write_tas_reordered(mc_tm->tsetlin_machines[i], ptr, ptr2, ptr3, ptr4);
	}
}

float mc_tm_downsize(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples, int Threshold, float * compresison){
	int total_clauses = 0;
	for(int i = 0; i < CLASSES; i++){
		// Remove the clauses that have been deemed not useful
		// Write the indexes of useful clauses to a vector 
		down_size_clauses(mc_tm->tsetlin_machines[i]);
		total_clauses += get_clauses(mc_tm->tsetlin_machines[i]);
	}
	std::cout << "Original Total Clauses:\t" << (CLAUSES*CLASSES) << std::endl;
	std::cout << "New Total Clauses:\t" << total_clauses << std::endl;
	float comp = 1.0 - (float)((float)total_clauses/(CLAUSES*CLASSES));
	std::cout << "Compression: " << comp << std::endl;
	*compresison = (float)comp;
	int errors;
	int max_class;
	int max_class_sum;
	errors = 0;	
	for (int l = 0; l < number_of_examples; l++) {
		/******************************************/
		/*** Identify Class with Largest Output ***/
		/******************************************/
		max_class_sum = tm_score(mc_tm->tsetlin_machines[0], X[l], Threshold);
		max_class = 0;
		for (int i = 1; i < CLASSES; i++) {	
			int class_sum = tm_score(mc_tm->tsetlin_machines[i], X[l], Threshold);
			if (max_class_sum < class_sum) {
				max_class_sum = class_sum;
				max_class = i;
			}
		}
		if (max_class != y[l]) {
			errors += 1;
		}
	}
	return 1.0 - 1.0 * errors / number_of_examples;
}

/******************************************/
/*** Online Training of Tsetlin Machine ***/
/******************************************/

// The Tsetlin Machine can be trained incrementally, one training example at a time.
// Use this method directly for online and incremental training.

void mc_tm_update(struct MultiClassTsetlinMachine *mc_tm, int Xi[], int target_class, float s, int Threshold)
{
	tm_update(mc_tm->tsetlin_machines[target_class], Xi, 1, s, Threshold);

	// Randomly pick one of the other classes, for pairwise learning of class output 
	unsigned int negative_target_class = (unsigned int)CLASSES * 1.0*rand()/((unsigned int)RAND_MAX+1);
	while (negative_target_class == (unsigned int)target_class) {
		negative_target_class = (unsigned int)CLASSES * 1.0*rand()/((unsigned int)RAND_MAX+1);
	}

	tm_update(mc_tm->tsetlin_machines[negative_target_class], Xi, 0, s, Threshold);
}

/**********************************************/
/*** Batch Mode Training of Tsetlin Machine ***/
/**********************************************/

void mc_tm_fit(struct MultiClassTsetlinMachine *mc_tm, int X[][FEATURES], int y[], int number_of_examples, int epochs, float s, int Threshold)
{
	for (int epoch = 0; epoch < epochs; epoch++) {
		// Added shuffling here...
		// First get the vector of random numbers 
		// int random_vect[number_of_examples]; 
		// for(int i =0; i < number_of_examples; i++){
		// 	random_vect[i] = i;
		// }
		// shuffle(random_vect, number_of_examples);
		
		// // Now swap the rows of X and the vaues of y using random_vect
		// for(int i = 0; i < number_of_examples; i++){
		// 	y[i] = y[random_vect[i]];
		// 	for(int j = 0; j < FEATURES; j++){
		// 		X[i][j] = X[random_vect[i]][j];
		// 	}
		// }
		for (int i = 0; i < number_of_examples; i++) {
			mc_tm_update(mc_tm, X[i], y[i], s, Threshold);
		}
	}
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}