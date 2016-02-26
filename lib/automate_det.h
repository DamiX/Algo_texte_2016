#ifndef __H_AUTOMATE_DET__
#define __H_AUTOMATE_DET__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct automate_det{
	int	qInit;
	int	nbEtats;
	int	nbEtatsFinals;
	int	curseur;
	int	n_alphabet;
	int*	qFinals;
	int**	matTransition;
	int*	map_alphabet;
} automate_det;

//Error management
inline void print_error(char*);

// Constructeur & Destructeur
automate_det* init_automate();
automate_det* init_automate(const int, const int, const int, int*, int, int*);
void free_automate_det(automate_det*);

// Accessor
void set_transition(automate_det*, const int, const char, const int);

int is_final(const automate_det*);
int etat_is_final(const automate_det*, const int etat);
inline void read_letter(automate_det*, const char);

/*void set_initial_states(automate_min*, const int);
void set_final_states(automate_min*, const int);*/

inline int map_letter(char, automate_det*);

#endif
