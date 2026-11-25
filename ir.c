#include "ir.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char *read_all(FILE *fp, size_t *out_len) {
	if (fseek(fp, 0, SEEK_END) != 0) return NULL;
	long size = ftell(fp);
	if (size < 0) return NULL;
	rewind(fp);
	
	char *buf = malloc((size_t)size + 1);
	if (!buf) return NULL;
	
	size_t n = fread(buf, 1, (size_t)size, fp);
	buf[n] = '\0';
	
	if (out_len) *out_len = n;
	return buf;
}
static inline void initIr(list *s) {
	s->cap = 5;
	s->len = 0;
	s->buf = malloc(sizeof(ins) * s->cap);
}

static inline void appendIns(list *s, ins c) {
	if (s->len < s->cap) {
	} else {
 		s->cap = 2 * s->cap + 1;
		s->buf = realloc(s->buf, sizeof(ins) * s->cap);
	}
	s->buf[s->len] = c;
	s->len++;
}

bool isIns(char c) {
	size_t len = strlen(".,<>+-");

	for (size_t y = 0; y < len; y++) {
		if (c == ".,<>+-"[y]) {
			return true;
		}
	}
	return false;
}

size_t findCb(size_t ptr, list l) {
	size_t i = ptr;
	int count = 0;

	while (true) {
		switch (l.buf[i].ins) {
		case OB: count++; break;
		case CB: count--; break;
		default :          break;
		}
		if (count == 0) {
			return i;
		}
		i++;
	}
	fprintf(stderr, "syntax error\n");
	exit(1);
}

size_t findOb(size_t ptr, list l) {
	size_t i = ptr;
	int count = 0;

	while (true) {
		switch (l.buf[i].ins) {
		case  OB: count--; break;
		case  CB: count++; break;
		default :          break;
		}
		if (count == 0) {
			return i;
		}
		i--;
	}
	fprintf(stderr, "syntax error\n");
	exit(1);
}


list minIr(list old) {
	list l;
	initIr(&l);
	instruction lastIns = OB;

	for (size_t i = 0; i < old.len; i++) {
		if (old.buf[i].ins != CB &&
 			old.buf[i].ins != OB &&
			old.buf[i].ins == lastIns) {
			l.buf[l.len - 1].am += old.buf[i].am;
		} 
		else {appendIns(&l, old.buf[i]);}
		lastIns = old.buf[i].ins;
	}
	
	return l;
}


list genIr(char *argv) {
	FILE *f = fopen(argv, "r");
	if (f == NULL) {
		fprintf(stderr, "failed to open file: %s\n", argv);
		exit(1);
	}
	list l;
	initIr(&l);
	size_t len;
	char *buf = read_all(f, &len);

	for (size_t i = 0; i < len; i++) {
		switch (buf[i]) {
		case '>': appendIns(&l,(ins) {.ins = MOV, .am =  1});             break;
		case '<': appendIns(&l,(ins) {.ins = MOV, .am = -1});             break;
		case '+': appendIns(&l,(ins) {.ins = INC, .am =  1});             break;
		case '-': appendIns(&l,(ins) {.ins = INC, .am = -1});             break;
		case '.': appendIns(&l,(ins) {.ins = OUT, .am =  0});             break;
		case ',': appendIns(&l,(ins) {.ins = INP, .am =  0});             break;
		case '[': appendIns(&l,(ins) {.ins =  OB, .am =  0});             break;
		case ']': appendIns(&l,(ins) {.ins =  CB, .am =  0});             break;
		default :                                                         break;
		}
	}
	
	free(buf);
	fclose(f);
	list new  = minIr(l);
	free(l.buf);

	for (size_t i = 0; i < new.len; i++) {
		switch(new.buf[i].ins) {
		case  OB: new.buf[i].am = findCb(i ,new); break;
		case  CB: new.buf[i].am = findOb(i ,new); break;
		default :          break;
		}
	}

	return new;
}
