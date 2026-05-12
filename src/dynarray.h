/*
Implementing a Dynamic Integer Array in C

Problem is: C arrays must be defined with a given size. This implements a
solution which allows an int array to have no specified size at compile
time, and instead dynamically expand and contract as needed to accomodate
runtime changes.
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
	    int* np = realloc(
			arr->data, sizeof(*(arr->data)) * arr->len * BUFFER_MULT
		);
		arr->data = np;
	    arr->capacity *= BUFFER_MULT;
	}

	*(arr->data + arr->len) = value;
	arr->len++;
}

void da_remove(DynArray* arr, unsigned int index) {
	/*
	Removes the value at the specified index from the array, updating its
	len accordingly. All further elements shift left. If it's the only
	element in the array, we essentially re-initialize it. If it's the last
	element in the array, we just update len.
	*/

	if (arr->len == 1) {
		da_init(arr);
		return;
	} else if (index == arr->len) {
		arr->len--;
		return;
	}

	for (size_t i = index; i < (arr->len - 1); i++) {
		*(arr->data + i) = *(arr->data + i + 1);
	}
	arr->len--;
}

void da_modify(DynArray* arr, unsigned int index, int newValue) {
	/*
	Sets the value at the passed index to the passed newValue. Non-op for
	indices that are out-of-bounds.
	*/

	if (index >= arr->len) { return; }

	*(arr->data + index) = newValue;
}

int* da_get(DynArray* arr, unsigned int index) {
	/*
		Returns a pointer to the value in arr->data at the requested index; if
		the requested index is out-of-bounds, returns NULL.
	*/

	if ((index < 0) || (arr->len < (index + 1))) { return NULL; }
	return (arr->data + index);
}

int da_pop(DynArray* arr) {
	/*
	Returns the last integer in the arr->data array. Undefined behavior for
	empty arrays; will crash. Memory at the original location is not
	modified; we "remove" by simply changing the tracked length.
	*/

	arr->len--;
	return *(arr->data + arr->len);
}

void da_print(DynArray* arr) {
	/*
	Helper function to print the elements of the array followed by its
	current len and capacity.
	*/

	for (size_t i = 0; i < arr->len; i++) {
		printf("%d", *(arr->data + i));
		if (i < (arr->len - 1)) { printf(", "); }
	}
	printf("\nCurrent Len: %zu\nCurrent Capacity: %zu\n", arr->len, arr->capacity);
}

