/* Implementing a Dynamic Integer Array in C
 *
 * Problem is: C arrays must be
 * defined with a given size. This implements a solution which allows an int
 * array to have no specified size at compile time, and instead dynamically
 * expand and contract as needed to accomodate runtime changes. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int INIT_BUFFER = 8; // multiple of first element size for malloc()
const int BUFFER_MULT = 2; // multiple of current size when reallocating

typedef struct DynArray {
    void *data;      // pointer to (first element of) actual data
    size_t el_size;  // size of the elements in the array
    size_t len;      // amount of elements currently in the array
    size_t capacity; // total amount of elements that can currently fit
} DynArray;

// INIT & DESTROY

void da_init(DynArray *arr, size_t el_size) {
    /* * Initialize the array with empty data. Requires size of elements to be *
     * passed. This locks the array into a particular type (or be precise, *
     * element type). */
    arr->data = NULL;
    arr->el_size = el_size;
    arr->len = 0;
    arr->capacity = 0;
}

void da_destroy(DynArray *arr) {
    /* Counterpart to init: free the buffer and reset all fields. Note that
     * el_size becomes 0, not null. Must be reset with init anyway. */

    free(arr->data);
    arr->data = NULL;
    arr->el_size = 0;
    arr->len = 0;
    arr->capacity = 0;
}

// ACCESSORS

size_t da_size(DynArray *arr) { return (arr->el_size * arr->len); }

size_t da_capacity(DynArray *arr) { return (arr->el_size * arr->capacity); }

size_t da_len(DynArray *arr) { return (arr->len); }

size_t da_el_size(DynArray *arr) { return (arr->el_size); }

void *da_get(DynArray *arr, unsigned int index) {
    /* Returns a pointer to the value in arr->data at the requested index; if
     * the requested index is out-of-bounds, returns NULL. */

    if ((index < 0) || (arr->len < (index + 1))) {
        return NULL;
    }
    return ((char *)arr->data + (index * arr->el_size));
}

// MUTATIONS

void da_append(DynArray *arr, void *value) {
    /* Adds the passed element to the end of the array. Assumes its size matches
     * arr->el_size as initialized. Value received as pointer, value is copied
     * to the end of the array. */

    if (arr->len == 0) {
        arr->data = malloc(arr->el_size * INIT_BUFFER);
        arr->capacity = INIT_BUFFER;
    } else if ((arr->len + 1) > arr->capacity) {
        void *np = realloc(arr->data, arr->el_size * arr->len * BUFFER_MULT);
        arr->data = np;
        arr->capacity = arr->len * BUFFER_MULT;
    }

    memcpy((char *)arr->data + (arr->len * arr->el_size), value, arr->el_size);
    arr->len++;
}

void da_remove(DynArray *arr, unsigned int index) {
    /* Removes the value at the specified index from the array, updating its len
     * accordingly. All further elements shift left. If it's the only element in
     * the array, we essentially re-initialize it. If it's the last element in
     * the array, we just update len. */

    if (arr->len == 1) {
        da_init(arr, arr->el_size);
        return;
    }

    for (size_t i = index; i < (arr->len - 1); i++) {
        // Move the chunk of memory at the further address to the earlier one
        memmove(((char *)arr->data + (i * arr->el_size)),
                ((char *)arr->data + ((i + 1) * arr->el_size)), arr->el_size);
    }
    arr->len--;
}

void da_set(DynArray *arr, unsigned int index, void *newValue) {
    /* Sets the value at the passed index to the passed newValue. Non-op for
     * indices that are out-of-bounds. */

    if (index >= arr->len) {
        return;
    }

    memcpy((char *)arr->data + (index * arr->el_size), newValue, arr->el_size);
}

void da_pop(DynArray *arr, void *dest) {
    /* Removes the last element in the array and copies its value to a user-
     * provided pointer. This way, we avoid giving the user a pointer to the
     * removed value, which they can immediately write over with a call to
     * da_append(). If we require they pass a pointer themselves, then we define
     * that behavior out of existence. If array has no elements, we return. */

    if (arr->len == 0) {
        return;
    }

    arr->len--;
    memcpy(dest, (char *)arr->data + (arr->len * arr->el_size), arr->el_size);
}

