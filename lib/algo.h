#ifndef __H_ALGO__
#define __H_ALGO__

#include <stdio.h>
#include <stdlib.h>
#include "automate_det.h"
#include "file.h"

int* table_bord(const char*);

void get_alphabet(const char**, const int, int**, int*);

automate_det* automate_localisation(const char*);
automate_det* build_trie(char**, const int, const int);
automate_det* aho_corasick(char**, const int, const int);

#endif
