#include <stdio.h>
#include <stdlib.h>
#include <iomanip> 
#include <algorithm>    // std::sort
#include "TsetlinMachine.h"
#include <algorithm>    // std::sort, std::stable_sort

/**************************************/
/*** The Multiclass Tsetlin Machine ***/
/**************************************/

/*** Initialize Tsetlin Machine ***/
struct TsetlinMachine *CreateTsetlinMachine()
{
  struct TsetlinMachine *tm = new struct TsetlinMachine;
	/* Set up the Tsetlin Machine structure */
	tm_initialize(tm);
	// zero the clause strengths
	for(int i = 0; i < CLAUSES; i++){
		(*tm).clause_strengths.push_back(0);			// clause strengths are evaluated using update_clause_strengths()
		// (*tm).thresholded_clauses.push_back(0);		// convert the clause strengths to a 0 or 1 
		(*tm).useful_clause_indexes.push_back(i);	// update the list of useful clauses each time 
	}
	return tm;
}

void tm_initialize(struct TsetlinMachine *tm)
{
	for (int j = 0; j < (int)(*tm).useful_clause_indexes.size(); j++) {				
		for (int k = 0; k < FEATURES; k++) {
			if (1.0 * rand()/RAND_MAX <= 0.5) {
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] = NUMBER_OF_STATES;
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] = NUMBER_OF_STATES + 1; 
			} else {
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] = NUMBER_OF_STATES + 1;
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] = NUMBER_OF_STATES; 
			}
		}
	}
}

/* Translates automata state to action */
static inline int action(int state)
{
		return state > NUMBER_OF_STATES;
}

void clear_clauses(struct TsetlinMachine *tm){
	(*tm).clause_strengths.clear();
	for(int i = 0; i < (int)(*tm).useful_clause_indexes.size(); i++){
		(*tm).clause_strengths.push_back(0); 
	}
}

void update_clause_strengths(struct TsetlinMachine *tm, int guess, int actual, int class_identity){
	// use clause outputs to update clause vector
	if(class_identity == guess){
		for(int i = 0; i < (int)(*tm).useful_clause_indexes.size(); i++){
			if((*tm).clause_output[(*tm).useful_clause_indexes[i]] == 1 && (guess == actual)){
			// Guessed correct, increment the positive clauses, decrement negative clauses
				if(i%2 == 0){
					(*tm).clause_strengths[(*tm).useful_clause_indexes[i]] += 1;
				}
			// else{
			// 	(*tm).clause_strengths[(*tm).useful_clause_indexes[i]] -= 1;
			// }
			}
			else if((*tm).clause_output[(*tm).useful_clause_indexes[i]] == 1 && (guess != actual)){
			// Guessed incorrect, increment the negative clauses, decrement positive clauses
				if(i%2 == 0){
					(*tm).clause_strengths[(*tm).useful_clause_indexes[i]] -= 1;
				}
				else{
					(*tm).clause_strengths[(*tm).useful_clause_indexes[i]] += 1;
				}
			}
		}
	}
}

// This function is unnecessary now 
void print_clause_strengths(struct TsetlinMachine *tm){
	// for(int i = 0; i < (int)(*tm).useful_clause_indexes.size() ; i++){
	// 	float q = (float)((*tm).clause_strengths[(*tm).useful_clause_indexes[i]]/(NUMBER_OF_EXAMPLES_TRAIN));
	// 	if((q <= 0 )){
	// 		q = 0.0;
	// 		(*tm).thresholded_clauses[(*tm).useful_clause_indexes[i]] = (int)q;
	// 	}
	// 	else{
	// 		q =1.0;
	// 		(*tm).thresholded_clauses[(*tm).useful_clause_indexes[i]] = (int)q;
	// 	}
	// }

}

void down_size_clauses(struct TsetlinMachine *tm){
	std::vector<int> dummy_vect; 
	for(int i = 0; i < (int)(*tm).useful_clause_indexes.size(); i++){
		if((*tm).clause_strengths[(*tm).useful_clause_indexes[i]] != 0){
			dummy_vect.push_back(i);
		}
	}
	(*tm).useful_clause_indexes.clear();
	(*tm).useful_clause_indexes = dummy_vect;
}

int get_clauses(struct TsetlinMachine *tm){
	return (*tm).useful_clause_indexes.size();	
}

// int tm_weighted_score(struct TsetlinMachine *tm, int Xi[]){

// 	int predict = 1;
// 	// Reduced clause output using (*tm).useful_clause_indexes
// 	int j, k; 
// 	int action_include, action_include_negated;
// 	int all_exclude;
// 	// Print the number of useful clauses here:
// 	// std::cout<< "Useful Clauses:" << (*tm).useful_clause_indexes.size() << std::endl;
// 	for (j = 0; j < (int)(*tm).clause_ordered.size(); j++) {
// 		(*tm).clause_output[(*tm).clause_ordered[j].second] = 1;
// 		all_exclude = 1;
// 		for (k = 0; k < FEATURES; k++) {
// 			action_include = action((*tm).ta_state[(*tm).clause_ordered[j].second][k][0]);
// 			action_include_negated = action((*tm).ta_state[(*tm).clause_ordered[j].second][k][1]);

// 			all_exclude = all_exclude && !(action_include == 1 || action_include_negated == 1);

// 			if ((action_include == 1 && Xi[k] == 0) || (action_include_negated == 1 && Xi[k] == 1)) {
// 				(*tm).clause_output[(*tm).clause_ordered[j].second] = 0;
// 				break;
// 			}
// 		}
// 		(*tm).clause_output[(*tm).clause_ordered[j].second] = (*tm).clause_output[(*tm).clause_ordered[j].second] && !(predict == PREDICT && all_exclude == 1);
// 	}	

// 	// Reduced class sum
// 	int class_sum = 0;
// 	int clause_number = (*tm).clause_ordered.size();
// 	for (int j = 0; j <(int)(*tm).clause_ordered.size() ; j++) {
// 		int sign = 1 - 2 * ((*tm).clause_ordered[j].second & 1);
// 		class_sum += clause_number*(*tm).clause_output[(*tm).clause_ordered[j].second]*sign;
// 		// clause_number-gradient;
// 	}
	
// 	// class_sum = (class_sum > THRESHOLD) ? THRESHOLD : class_sum;
// 	// class_sum = (class_sum < -THRESHOLD) ? -THRESHOLD : class_sum;
// 	// printf("%d ", class_sum);
// 	return class_sum;
// }
/* Calculate the output of each clause using the actions of each Tsetline Automaton. */
/* Output is stored an internal output array. */

static inline void calculate_clause_output(struct TsetlinMachine *tm, int Xi[], int predict)
{
	int j, k;
	int action_include, action_include_negated;
	int all_exclude;

	for (j = 0; j < (int)(*tm).useful_clause_indexes.size(); j++) {
		(*tm).clause_output[(*tm).useful_clause_indexes[j]] = 1;
		all_exclude = 1;
		for (k = 0; k < FEATURES; k++) {
			action_include = action((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0]);
			action_include_negated = action((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1]);

			all_exclude = all_exclude && !(action_include == 1 || action_include_negated == 1);

			if ((action_include == 1 && Xi[k] == 0) || (action_include_negated == 1 && Xi[k] == 1)) {
				(*tm).clause_output[(*tm).useful_clause_indexes[j]] = 0;
				break;
			}
		}
		(*tm).clause_output[(*tm).useful_clause_indexes[j]] = (*tm).clause_output[(*tm).useful_clause_indexes[j]] && !(predict == PREDICT && all_exclude == 1);
	}
}

/* Sum up the votes for each class (this is the multiclass version of the Tsetlin Machine) */
static inline int sum_up_class_votes(struct TsetlinMachine *tm, int Threshold)
{
	int class_sum = 0;
	for (int j = 0; j < (int)(*tm).useful_clause_indexes.size(); j++) {
		int sign = 1 - 2 * ((*tm).useful_clause_indexes[j] & 1);
		class_sum += (*tm).clause_output[(*tm).useful_clause_indexes[j]]*sign;
	}
	
	class_sum = (class_sum > Threshold) ? Threshold : class_sum;
	class_sum = (class_sum < -Threshold) ? -Threshold : class_sum;

	return class_sum;
}

/* Get the state of a specific automaton, indexed by clause, feature, and automaton type (include/include negated). */
int tm_get_state(struct TsetlinMachine *tm, int clause, int feature, int automaton_type)
{
	return (*tm).ta_state[clause][feature][automaton_type];
}

void write_tas(struct TsetlinMachine *tm, FILE *ptr){
	int action_include, action_include_negated;
	for (int j = 0; j < CLAUSES; j++) {
		for (int k = 0; k < FEATURES; k++) {
			action_include = action((*tm).ta_state[j][k][0]);
			action_include_negated = action((*tm).ta_state[j][k][1]);
			fprintf(ptr, "%d %d ", (int)action_include, (int)action_include_negated);
		}
	}
}

bool sortrev(const std::pair<float,int> &a,  const std::pair<float,int> &b) 
{ 
  return (a.first > b.first); 
}

void write_tas_reordered(struct TsetlinMachine *tm, FILE *ptr, FILE *ptr2, FILE *ptr3, FILE *ptr4){

	// ptr 	= TA file 
	// ptr2 = Number of useful clauses in this class
	// ptr3 = Number of even clauses in this class
	// prt4 = Clause strenghts - not using this one currently  

	// get the total number of clauses 
	fprintf(ptr2, "%ld ", (*tm).useful_clause_indexes.size());
	// get the polarities of the even clauses  and odd clauses
	std::vector<int> even_indexes; 
	std::vector<int> odd_indexes; 
	for (int j = 0; j <(int)(*tm).useful_clause_indexes.size() ; j++) {
		int sign = 1 - 2 * ((*tm).useful_clause_indexes[j] & 1);
		if(sign == 1){
			even_indexes.push_back((*tm).useful_clause_indexes[j]);
		}
		else if(sign == -1){
			odd_indexes.push_back((*tm).useful_clause_indexes[j]);
		}
	}

	// write even indexes to file 
	fprintf(ptr3, "%ld ", even_indexes.size());
	// std::cout << even_indexes.size() << " " << odd_indexes.size() << "\n";

	// std::cout << "EVEN:";
	// for(int i = 0; i < even_indexes.size(); i++){
	// 	std::cout << even_indexes[i] << " "; 
	// }

	// std::cout<< "\n---------------------\n";
	// std::cout << "ODD:";
	// for(int i = 0; i < odd_indexes.size(); i++){
	// 	std::cout << odd_indexes[i] << " "; 
	// }
	// std::cout<<"\n";
	// // append the two together in the even_indexes
	even_indexes.insert(even_indexes.end(), odd_indexes.begin(), odd_indexes.end());
	// for(int i =0; i < even_indexes.size(); i++){
	// 	std::cout << even_indexes[i] << " ";
	// }
	// std::cout<<"\n";

	int action_include, action_include_negated;
	for (int j = 0; j <(int)(*tm).useful_clause_indexes.size() ; j++) {
		for (int k = 0; k < FEATURES; k++) {
			action_include = action((*tm).ta_state[even_indexes[j]][k][0]);
			action_include_negated = action((*tm).ta_state[even_indexes[j]][k][1]);
			fprintf(ptr, "%d %d ", (int)action_include, (int)action_include_negated);
		}
	}
	// re-order the indexes according to the sizes
	// for (std::vector<int>::iterator it=(*tm).clause_strengths.begin(); it!=(*tm).clause_strengths.end(); ++it)
	// Rank them 
	// std::sort((*tm).clause_strengths.begin(), (*tm).clause_strengths.begin()+(*tm).clause_strengths.size(), std::greater<float>());
	// // std::vector<int> useful_clause_indexes_reordered; 
	// for(int i = 0; i < (int)(*tm).useful_clause_indexes.size(); i++){
	// 	// Take the absolute values of the clause_strengths 
	// 	// add the useful clause strengths to clauses_ordered.
	// 	(*tm).clause_ordered.push_back(std::make_pair((*tm).clause_strengths[(*tm).useful_clause_indexes[i]], (*tm).useful_clause_indexes[i]));
	// }

	// std::stable_sort((*tm).clause_ordered.begin(), (*tm).clause_ordered.end(), sortrev);
	// for(int i = 0; i < (int)(*tm).clause_ordered.size(); i++){
	// 	fprintf(ptr4, "%f", (*tm).clause_ordered[i].first);
	// 	if(i != (int)(*tm).clause_ordered.size() -1){
	// 		fprintf(ptr4, ",");
	// 	}
	// }
	// fprintf(ptr4, "\n");
	// std::cout<<"\n";
}

static inline void type_i_feedback(struct TsetlinMachine *tm, int Xi[], int j, float s)
{
	if ((*tm).clause_output[(*tm).useful_clause_indexes[j]] == 0)	{
		for (int k = 0; k < FEATURES; k++) {
			// reward if already exc
			// penalty if inc
			(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] -= ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] > 1) && (1.0*rand()/RAND_MAX <= 1.0/s);
			// reward if already exc
			// penalty if inc					
			(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] -= ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] > 1) && (1.0*rand()/RAND_MAX <= 1.0/s);
		}
	} else if ((*tm).clause_output[(*tm).useful_clause_indexes[j]] == 1) {					
		for (int k = 0; k < FEATURES; k++) {
			if (Xi[k] == 1) {
				// reward if already inc
				// penalty if exc
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] += ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] < NUMBER_OF_STATES*2) && (BOOST_TRUE_POSITIVE_FEEDBACK == 1 || 1.0*rand()/RAND_MAX <= (s-1)/s);

				// reward if already exc
				// penalty if inc
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] -= ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] > 1) && (1.0*rand()/RAND_MAX <= 1.0/s);

			} else if (Xi[k] == 0) {
				// reward if already inc
				// penalty if exc
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] += ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] < NUMBER_OF_STATES*2) && (BOOST_TRUE_POSITIVE_FEEDBACK == 1 || 1.0*rand()/RAND_MAX <= (s-1)/s);
				
				// reward if alreaft exc
				// penality if inc
				(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] -= ((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] > 1) && (1.0*rand()/RAND_MAX <= 1.0/s);
			}
		}
	}
}

static inline void type_ii_feedback(struct TsetlinMachine *tm, int Xi[], int j) {
	int action_include;
	int action_include_negated;

	if ((*tm).clause_output[(*tm).useful_clause_indexes[j]] == 1) {
		for (int k = 0; k < FEATURES; k++) { 
			action_include = action((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0]);
			action_include_negated = action((*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1]);

			// reward if inc
			// penalty if exc
			(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] += (action_include == 0 && (*tm).ta_state[(*tm).useful_clause_indexes[j]][k][0] < NUMBER_OF_STATES*2) && (Xi[k] == 0);
			// reward if inc
			// penalty if exc
			(*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] += (action_include_negated == 0 && (*tm).ta_state[(*tm).useful_clause_indexes[j]][k][1] < NUMBER_OF_STATES*2) && (Xi[k] == 1);
		}
	}
}



/******************************************/
/*** Online Training of Tsetlin Machine ***/
/******************************************/

// The Tsetlin Machine can be trained incrementally, one training example at a time.
// Use this method directly for online and incremental training.

void tm_update(struct TsetlinMachine *tm, int Xi[], int target, float s, int Threshold) {
	/*******************************/
	/*** Calculate Clause Output ***/
	/*******************************/

	calculate_clause_output(tm, Xi, UPDATE);

	/***************************/
	/*** Sum up Clause Votes ***/
	/***************************/

	int class_sum = sum_up_class_votes(tm, Threshold);

	/*************************************/
	/*** Calculate Feedback to Clauses ***/
	/*************************************/

	// Calculate feedback to clauses
	for (int j = 0; j < (int)(*tm).useful_clause_indexes.size(); j++) {
		(*tm).feedback_to_clauses[j] = (2*target-1)*(1 - 2 * ((*tm).useful_clause_indexes[j] & 1))*(1.0*rand()/RAND_MAX <= (1.0/(Threshold*2))*(Threshold + (1 - 2*target)*class_sum));
	}
	
	/*********************************/
	/*** Train Individual Automata ***/
	/*********************************/

	for (int j = 0; j < (int)(*tm).useful_clause_indexes.size(); j++) {
		if ((*tm).feedback_to_clauses[j] > 0) {
			type_i_feedback(tm, Xi, j, s);
		} else if ((*tm).feedback_to_clauses[j] < 0) {
			type_ii_feedback(tm, Xi, j);
		}
	}
}

int tm_score(struct TsetlinMachine *tm, int Xi[], int Threshold) {
	/*******************************/
	/*** Calculate Clause Output ***/
	/*******************************/
	calculate_clause_output(tm, Xi, PREDICT);

	/***************************/
	/*** Sum up Clause Votes ***/
	/***************************/
	return sum_up_class_votes(tm, Threshold);
}