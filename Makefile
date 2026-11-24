FLAGS := -Wall -Wextra -g
CC    :=              gcc

.PHONY: run

run:
	${CC} ${FLAGS} brain.c ir.c -o brain
