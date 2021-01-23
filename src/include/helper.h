#ifndef __H_HELPER__
#define __H_HELPER__

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

extern const int max_ascii_char;

typedef struct _words {
	char* values[BUFFER_SIZE];
	int   n;
} words;

// Error management
static inline void print_error(const char* err) {
    fprintf(stderr, "%s: %s\n%s\n", __FILE__, err, strerror(errno));
}

// Charset management
static inline bool is_valid_ascii_char(const char c)
{
    return (c >= 0 && c <= max_ascii_char) ? true : false;
}

static inline void strtolower(char* str) {
    while((*str = tolower(*str)) != 0) { ++ str; }
}

char* norm_ascii_str(const char* str);

void free_words(words w);

words read_words(const char* filepath, bool is_binary_mode);

#endif
