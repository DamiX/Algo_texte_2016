#include "automate_det.h"

inline void print_error(char* err) {
	fprintf(stderr, err);
	exit(1);
}

automate_det* init_automate(const int nEtats, const int nAlphabet, const int etatInit, int* etatsFinals, int nEF, int* map_alphabet)
{
	automate_det* M = (automate_det*)malloc(sizeof(automate_det));
	if(M == NULL)
		print_error("Impossible d'allouer de la mémoire");

	//Init matrice de transition
	M->matTransition = (int**)malloc(sizeof(int*) * nEtats);
	if(M->matTransition == NULL)
		print_error("Impossible d'allouer de la mémoire");

	int i;
	for(i = 0; i < nEtats; ++i)
	{
		M->matTransition[i] = (int*)malloc(sizeof(int) * nAlphabet);
		if(M->matTransition[i] == NULL)
			print_error("Impossible d'allouer de la mémoire");

		memset(M->matTransition[i], 0, nAlphabet);
	}

	//Init etatInitial
	M->qInit = etatInit;

	//Init etatFinals
	M->nbEtatsFinals = nEF;
	M->qFinals = (int*)malloc(sizeof(int) * nEF);
	for(i = 0; i < nEF; ++i)
		M->qFinals[i] = etatsFinals[i];

	M->nbEtats = nEtats;
	M->curseur = etatInit;
	M->n_alphabet = nAlphabet;
	M->map_alphabet = map_alphabet;

	return M;
}

void free_automate_det(automate_det* M)
{
	int i;
	for(i = 0; i < M->nbEtats; ++i)
		free(M->matTransition[i]);
	free(M->matTransition);
	free(M->qFinals);
	free(M->map_alphabet);
	free(M);
}

inline int map_letter(char c, automate_det* M) {
	if(c >= 'a' && c <= 'z')
		return M->map_alphabet[(int)c-97];
	else if(c >= '0' && c <= '9')
		return M->map_alphabet[(int)c-22];
	else
		return -1;
}

int is_final(const automate_det* M)
{
	int i;
	for(i = 0; i < M->nbEtatsFinals; ++i)
	{
		if(M->curseur == M->qFinals[i]) {
			return 1;
		}
	}
	return 0;
}

int etat_is_final(const automate_det* M, const int etat)
{
	int i;
	for(i = 0; i < M->nbEtatsFinals; ++i)
	{
		if(etat == M->qFinals[i]) {
			return 1;
		}
	}
	return 0;
}

inline void read_letter(automate_det* M, const char c) {
	int key = map_letter(c, M);
	//printf("%c = %d\n", c, key);
	if(key == -1)
		M->curseur = 0;
	else
		M->curseur = M->matTransition[M->curseur][key];
}

// (p,c,q)
// TO DO : Implement checking
inline void set_transition(automate_det* M, const int p, const char c, const int q) {
	M->matTransition[p][map_letter(c, M)] = q;
}
