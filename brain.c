#include  <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ir.h"

int main(int argc, char **argv) {
	size_t ptr = 0;
	if (argc != 2) {
		fprintf(stderr, "provide a file and a file only\n");
		return 1;
	}
	uint8_t stack[30000] = {0};
	list l = genIr(argv[1]);

	for (size_t i = 0; i < l.len; i++) {
		switch (l.buf[i].ins) {
		case INC: stack[ptr] += l.buf[i].am; break;
		case MOV: ptr += l.buf[i].am; break;
		case  OB: if (stack[ptr] == 0) i = l.buf[i].am; break;
		case  CB: if (stack[ptr] != 0) i = l.buf[i].am; break;
		case OUT: putc(stack[ptr], stdout); break;
		case INP: stack[ptr] = getc(stdin); break;
		}
	}
	free(l.buf);
	putc('\n', stdout);
	return 0;
}
