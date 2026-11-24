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

size_t findCb(size_t ptr, char *code) {
	size_t i = ptr;
	size_t indx = ptr;
	int count = 0;

	while (code[i] != '\0') {
		switch (code[i]) {
		case '[': count++; break;
		case ']': count--; break;
		default :          break;
		}
		if (count == 0) {
			return indx;
		}
		i++;
		if (isIns(code[i])) {
			continue;
		}
		indx++;
	}
	fprintf(stderr, "syntax error\n");
	exit(1);
}

size_t findOb(size_t ptr, char *code) {
	size_t i = ptr;
	size_t indx = ptr;
	int count = 0;

	while (i != 0) {
		switch (code[i]) {
		case '[': count--; break;
		case ']': count++; break;
		default :          break;
		}
		if (count == 0) {
			return i;
		}
		i--;
		if (isIns(code[i])) {
			continue;
		}
		indx--;
	}
	fprintf(stderr, "syntax error\n");
	exit(1);
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
		case '[': appendIns(&l,(ins) {.ins =  OB, .am = findCb(i, buf)}); break;
		case ']': appendIns(&l,(ins) {.ins =  CB, .am = findOb(i, buf)}); break;
		default :                                                                 break;
		}
	}
	
	free(buf);
	fclose(f);
	return l;
}
