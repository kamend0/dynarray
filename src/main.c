#include <stdio.h>
#include "dynarray.h"

const int LIM = 128;

int main() {
	DynArray da;
	da_init(&da);

	for (int i = 0; i < LIM; i++) {
		da_append(&da, i);  // currently, this should crash
		printf("%d\n", *(da.data + i));
	}

	return 0;
}

