#include <stdio.h>
#include "src/dynarray.h"

const int LIM = 128;

int main() {
	DynArray da;
	da_init(&da);

	for (int i = 0; i < LIM; i++) {
		da_append(&da, i);
		if (i == 0) { da_print(&da); }
		printf("%d\n", *(da_get(&da, i)));
	}

	da_print(&da);
	int lastValue = da_pop(&da);
	printf("Popped: %d\n", lastValue);
	da_print(&da);

	// Let's try removing a random element.
	int doomedIndex = 63;
	printf("Removing element at index %d; should be %d\n", doomedIndex, *da_get(&da, doomedIndex));
	da_remove(&da, doomedIndex);
	da_print(&da);

	// Testing da_modify
	printf("Putting a new value, 10, at index 0\n");
	da_modify(&da, 0, 10);
	da_print(&da);
	printf("Now, putting 25 at index 66\n");
	da_modify(&da, 66, 25);
	da_print(&da);
	printf("Lastly, a non-op: putting 11 at index 256, well out-of-bounds\n");
	da_modify(&da, 256, 11);
	da_print(&da);


	return 0;
}

