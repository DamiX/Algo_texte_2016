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
	int	qInit;
	int	nbEtats;
	int	nbEtatsFinals;
	int	curseur;
	int	nAlphabet;
	int	sizeAllocate; //Prevent padding from OS
} automate_det;

// Constructeur & Destructeur
int** init_transition(const int, const int);
void free_automate_det(automate_det*);

// Affichage
void afficher_matTransition(automate_det*);

// Accessor
void set_transition(automate_det*, const int, const char, const int);

int est_final(const automate_det*);
int etat_est_final(const automate_det*, const int etat);
void read_letter(automate_det*, const char);

inline int map_letter(const char, automate_det*);

#endif
