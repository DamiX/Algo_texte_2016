#ifndef __H_ALGO__
#define __H_ALGO__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "automate_det.h"
#include "file.h"

#define true 1
#define false 0

typedef int bool;

inline void print_error(const char*);

//Manage Charset
inline bool is_valid_caractere(const int);
char* normalize_string(const char*);
inline void strtolower(char*);


int* table_bord(const char*);

void get_alphabet(const char**, const int, int**, int*);

automate_det* automate_localisation(const char*);
automate_det* build_trie(char**, const int, const int);
automate_det* aho_corasick(char**, const int, const int);

#endif
