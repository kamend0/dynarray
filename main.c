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

	return 0;
}

