cbuild:
	@gcc -Wall -Wextra -o "build/main" "main.c"

brun: cbuild
	@build/main

