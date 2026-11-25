FLAGS := -Wall -Wextra -g
CC    :=            clang

.PHONY: run

run:
	${CC} ${FLAGS} brain.c ir.c -o brain -fsanitize=address,undefined
