/*
	Implementing a Dynamic Integer Array in C

	Problem is: C arrays must be defined with a given size. This implements a
	solution which allows an int array to have no specified size at compile
	time, and instead dynamically expand and contract as needed to accomodate
	runtime changes.

	The initial length and capacity have to be specified up-front just like a
	regular C array; but the helper methods are what make this particular
	struct dynamic.
*/


#include <stdlib.h>


const int INIT_BUFFER = 8;  // multiple of sizeof(int)
const int BUFFER_MULT = 2;  // multiple of current size when reallocating


typedef struct DynArray {
   int* data; 			// pointer to (first element of) actual data
   size_t len; 			// amount of elements currently in the array
   size_t capacity;		// total amount of elements that can currently fit
} DynArray;


void da_init(DynArray* arr) {
	/*
	    Initialize an array with valid but essentially empty data.
	*/
	arr->data = NULL;
	arr->len = 0;
	arr->capacity = 0;
}

void da_append(DynArray* arr, int value) {
	/*
	    Idea: check current len and capacity. If adding the new value would
	    push the arr over its allocated memory limit, then increase its limit
	    by some amount to make room for the new value; append it to the end of
	    the array.
	*/


	if (arr->len == 0) {
	    // First element: we need to grab an initial amount of space
	    arr->data = malloc(sizeof(int) * INIT_BUFFER);
	    *(arr->data) = value;
	    arr->len = 1;
	    arr->capacity = INIT_BUFFER;
	    return;
	} else if ((arr->len + 1) > arr->capacity) {
	    arr->data = realloc(
			arr->data, sizeof(*(arr->data) * arr->len * BUFFER_MULT)
		);
	    arr->capacity *= BUFFER_MULT;
	}

	*(arr->data + arr->len) = value;
	arr->len++;
}

