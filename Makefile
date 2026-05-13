cbuild:
	@gcc -Wall -Wextra -o "build/main" "main.c"

brun: cbuild
	@build/main

tbuild-int:
	@gcc -Wall -Wextra -o "build/test_int" "tests/test_int.c"

trun-int: tbuild-int
	@build/test_int

tbuild-arb:
	@gcc -Wall -Wextra -o "build/test_arb" "tests/test_arb.c"

trun-arb: tbuild-arb
	@build/test_arb

test: trun-int trun-arb
