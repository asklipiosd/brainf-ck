FLAGS := -Wall -Wextra -g
CC    :=              gcc

.PHONY: run

brain: brain.c
	${CC} ${FLAGS} brain.c -o brain
run: brain
	./brain
