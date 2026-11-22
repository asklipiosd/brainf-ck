#include  <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

size_t findCb(size_t ptr, char *code) {
	size_t i = ptr;
	int count = 0;

	while (code[i] != '\0') {
		switch (code[i]) {
		case '[': count++; break;
		case ']': count--; break;
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

size_t findOb(size_t ptr, char *code) {
	size_t i = ptr;
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
	}
	fprintf(stderr, "syntax error\n");
	exit(1);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "provide a file and a file only\n");
		return 1;
	}
	(void) argc;
	uint8_t stack[30000] = {0};
	FILE *f = fopen(argv[1], "r");
	size_t len;
	size_t ptr = 0;
	char *buf = read_all(f, &len);
	fclose(f);

	for (size_t i = 0; i < len; i++) {
		switch (buf[i]) {
		case '>': ptr++;                    	break;
		case '<': ptr--;                    	break;
		case '+': stack[ptr]++;             	break;
		case '-': stack[ptr]--;             	break;
		case '.': putc(stack[ptr], stdout);     break;
		case ',': stack[ptr] = getc(stdin); 	break;
		case '[': 
			if (stack[ptr] == 0) {
				i = findCb(i, buf);
			}
			break;
		case ']': 
			if (stack[ptr] != 0) {
				i = findOb(i, buf);
			}
			break;
		default: break;
		}
	}

	free(buf);
	return 0;
}
