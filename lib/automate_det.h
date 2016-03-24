#ifndef __H_AUTOMATE_DET__
#define __H_AUTOMATE_DET__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct automate_det{
	int*	qFinals;
	int*	mapAlphabet;
	int**	matTransition;
	char**	correspondance;
	int	qInit;
	int	nStates;
	int	nFinalStates;
	int	cursor;
	int	nAlphabet;
	int	sizeAllocate; //Prevent padding from OS
} automate_det;

// Constructeur & Destructeur
int** init_transition(const int, const int);
void free_automate_det(automate_det*);

// Affichage
void print_automata(automate_det*);

// Other
int is_final(const automate_det*);
int state_is_final(const automate_det*, const int);

void read_letter(automate_det*, const char);
inline int map_letter(const char, automate_det*);

#endif
