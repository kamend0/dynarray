cbuild:
	@gcc -Wall -Wextra -o "build/main" "src/main.c"

brun: cbuild
	@build/main

