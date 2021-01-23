#ifndef __H_AUTOMATA__
#define __H_AUTOMATA__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Derministic automata datastructure
typedef struct deterministic_automata{
	int*	qFinals;
	int*	mapAlphabet;
	int**	matTransition;
	char**	correspondance;
	int		qInit;
	int		nStates;
	int		nFinalStates;
	int		cursor;
	int		nAlphabet;
	int		sizeAllocate; //Prevent padding
} deterministic_automata;

int** init_transition(const int, const int);
void free_deterministic_automata(deterministic_automata*);

void print_automata(deterministic_automata*);

// Other
bool is_final_automata(const deterministic_automata*);
bool is_automata_state_final(const deterministic_automata*, const int);

void read_letter(deterministic_automata*, const char);
int map_letter(const char, deterministic_automata*);

#endif
