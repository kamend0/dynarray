#include <stdio.h>
#include "src/dynarray.h"

const int LIM = 128;

int main() {
	DynArray da;
	da_init(&da);

	for (int i = 0; i < LIM; i++) {
		da_append(&da, i);
		printf("%d\n", *(da_get(&da, i)));
	}

	printf("Arr has len %zu.\n", da.len);
	int lastValue = da_pop(&da);
	printf("The last value was %d, but now it's %d.\n", lastValue, *(da_get(&da, (da.len - 1))));
	printf("Arr now has len %zu.\n", da.len);

	return 0;
}

