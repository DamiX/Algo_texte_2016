#ifndef __H_ALGORITHM__
#define __H_ALGORITHM__

#include <stdlib.h>
#include <string.h>
#include "automata.h"

int* table_bord(const char*);
void get_alphabet(const char**, const int, int**, int*);

deterministic_automata* automate_localisation(const char*);
deterministic_automata* build_trie(char**, const int, const int);
deterministic_automata* aho_corasick(char**, const int, const int);

#endif
