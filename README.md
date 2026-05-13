# Dynamic Array in C

This is a project I put together just to learn the basics of C and memory management. An array of dynamic size and of arbitrary but homogeneous type, `DynArray`, is defined in `src/dynarray.h`. `#include`ing this gives access to the `DynArray` struct as well as its related methods, listed below.

I wrote all code by hand, the good old fashioned way. I did use LLMs as essentially suped-up Google, but tried my darndest to prompt them in such a way to gently nudge and guide rather than provide solutions. This was surprisingly difficult to accomplish.

## Methods + Signatures

* `void da_init(DynArray* arr, size_t el_size)` - constructor, initializes an empty instance. This is what sets the type of the array, though it's not strictly "typed"; rather it has elements of homogeneous *size* as defined by this parameter, e.g., `sizeof(int)`.
* `void da_destroy(DynArray* arr)` - destructor, frees the underlying buffer and resets all fields. The instance must be re-initialized with `da_init` before reuse.

Accessors:
* `size_t da_size(DynArray* arr)` - returns size of the array in bytes
* `size_t da_capacity(DynArray* arr)` - returns the currently allocated capacity in bytes
* `size_t da_len(DynArray* arr)` - returns the number of elements currently in the array
* `size_t da_el_size(DynArray* arr)` - returns the size of a single element in bytes (as set at init)
* `void* da_get(DynArray* arr, unsigned int index)` - returns a pointer to the element at `index`, or `NULL` if out-of-bounds. The caller is responsible for casting back to the appropriate type. (Feels weird calling this an accessor also because it doesn't access struct fields, but rather actual elements in the data; but it makes literal sense here, so here it goes.)

Mutations:
* `void da_append(DynArray* arr, void* value)` - copies the element pointed to by `value` onto the end of the array, growing the buffer if needed.
* `void da_set(DynArray* arr, unsigned int index, void* newValue)` - overwrites the element at `index` with the value pointed to by `newValue`. No-op if the index is out-of-bounds.
* `void da_remove(DynArray* arr, unsigned int index)` - removes the element at `index`, shifting all subsequent elements left.
* `void da_pop(DynArray* arr, void* dest)` - removes the last element and copies its value to the pointer `dest` provided by the caller. Going through a caller-provided destination avoids ever handing back a pointer into the array that the caller could be tempted to dereference after a subsequent `da_append` invalidates it.

## Design notes

No type safety for you! I decided to use void-pointers instead of _Generic because I like to pretend it's 1989, before C was of drinking age. This basically means the DynArray struct understands the size of its elements, but not their actual types.

Growth strategy is geometric. I guess I designed this to expect not a lot of elements to be loaded most of the time. The first call to `da_append` always initializes a set buffer amount rather than just enough for the first element. This is to only resize once we hit this threshold. No shrinking is a gap that I will be addressing.

## Building and running

Makefile has the following commands:

* `brun` - builds and runs `main.c` - doesn't do anything, just tests that the header file compiles
* `trun-int` - builds and runs the `int`-focused tests in `tests/test_int.c`
* `trun-arb` - builds and runs the arbitrary-type tests in `tests/test_arb.c` (structs, doubles, chars, string pointers)
* `test` - runs both test suites

All builds go to `build/`. Tests use a tiny assertion macro suite defined in `tests/test_common.h`.

## Caveats

Arrays don't shrink (yet).

Claude wrote the tests. You'd be amazed at how difficult it was to write me ACTUAL tests that actually TESTED things and didn't just verify existing behavior (a lesson I will be taking with me).

