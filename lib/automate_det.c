#include "automate_det.h"

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

void free_automate_det(automate_det* M)
{
	int i;
	for(i = 0; i < M->sizeAllocate; ++i)
		free(M->matTransition[i]);
	free(M->matTransition);
	free(M->qFinals);
	free(M->mapAlphabet);
	free(M);
}

// Affichage
void afficher_matTransition(automate_det* M)
{
	int i, j;
	int pad = M->nAlphabet;

	char buffer[1024];
	memset(buffer, 0, 1024);

	strcat(buffer, "+");
	for(i = 0; i < pad; ++i)
		strcat(buffer, "=====");
	strcat(buffer, "+\n+\t\t||\t");

	/*for(i = 0; i < M->nAlphabet; ++i) {
		strcat(buffer, M-);
		strcat(buffer, '\t');
	}*/

	strcat(buffer, "\n+");
	for(i = 0; i < pad; ++i)
		strcat(buffer, "-----");
	strcat(buffer, "+\n||\t||");


	/*for(i = 0; i < M->nbEtats; ++i)
	{
		strcat(buffer, "||
		for(j = 0; j < M->nAlphabet; ++j)
		{
	*/		
	
	printf("%s\n", buffer);
}

int est_final(const automate_det* M)
{
	int i;
	for(i = 0; i < M->nbEtatsFinals; ++i) {
		if(M->curseur == M->qFinals[i])
			return 1;
	}
	return 0;
}

int etat_est_final(const automate_det* M, const int etat)
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

void read_letter(automate_det* M, const char c)
{
	int x = M->mapAlphabet[(int)c];
	if(x == -1)
		M->curseur = 0;
	else
		M->curseur = M->matTransition[M->curseur][x];
}

inline int map_letter(const char c, automate_det* M) {
	return M->mapAlphabet[(int)c];
}
