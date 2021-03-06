#include "include/automata.h"

int** init_transition(const int nEtats, const int nAlphabet)
{
	int** matrice = (int**)malloc(sizeof(int*) * nEtats);
	if(matrice == NULL) {
		fprintf(stderr, "%s: Impossible d'allouer de la mémoire\n%s\n", __FILE__, strerror(errno));
		exit(1);
	}

	int i;
	size_t sz_int_nEtats = sizeof(int) * nAlphabet;

	for(i = 0; i < nEtats; ++i)
	{
		*(matrice+i) = (int*)malloc(sz_int_nEtats);
		if(*(matrice+i) == NULL) {
			fprintf(stderr, "%s: Impossible d'allouer de la mémoire\n%s\n", __FILE__, strerror(errno));
			exit(1);
		}

		memset(*(matrice+i), -1, sz_int_nEtats);
	}

	return matrice; //!! NE PAS OUBLIER DE LIBERER LA MEMOIRE !!
}

void free_deterministic_automata(deterministic_automata* M)
{
	int i;
	for(i = 0; i < M->sizeAllocate; ++i) {
		free(M->matTransition[i]);
	}

	if(M->correspondance != NULL) {
		for(i = 0; i < M->sizeAllocate; ++i)
		free(M->correspondance[i]);
	}
	free(M->matTransition);
	free(M->qFinals);
	free(M->mapAlphabet);
	free(M);
}

// Affichage
// TODO: PRETTY PRINTING!!
void print_automata(deterministic_automata* M)
{
	int i, j;

	for(i = 0; i < M->nStates; ++i)
	{
		printf("Etat %d:\t", i);
		for(j = 0; j < M->nAlphabet; ++j)
		{
			printf("%d\t", M->matTransition[i][j]);
		}
		printf("\n");
	}
}

bool is_final_automata(const deterministic_automata* M)
{
	return is_automata_state_final(M, M->cursor);
}

bool is_automata_state_final(const deterministic_automata* M, const int state)
{
	int i;
	for(i = 0; i < M->nFinalStates; ++i)
	{
		if(state == M->qFinals[i])
			return true;
	}

	return false;
}

void read_letter(deterministic_automata* M, const char c)
{
	int x = M->mapAlphabet[(int)c];
	if(x == -1)
		M->cursor = 0;
	else
		M->cursor = M->matTransition[M->cursor][x];
}

int map_letter(const char c, deterministic_automata* M) {
	return M->mapAlphabet[(int)c];
}
