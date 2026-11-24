#ifndef IR_H
#define IR_H
#include <stdlib.h>

typedef enum {
	INC,
	MOV,
	OB,
	CB,
	OUT,
	INP,
} instruction;

typedef struct {
	instruction ins;
	int am;
} ins;

typedef struct {
	ins *buf;
	size_t cap;
	size_t len;
} list;

list genIr(char *argv);

#endif
