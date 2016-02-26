#include "algo.h"

int* table_bord(const char* mot)
{
	size_t sz = strlen(mot)+1;

	int* bord = (int*)malloc(sizeof(int) * sz);
	if(bord == NULL) {
		fprintf(stderr, "Allocation mémoire impossible");
		exit(1);
	}

	memset(bord, 0, sizeof(int)*sz);

	bord[0] = -1;

	int i = 0, j = 0;
	for(i = 2; i < sz; ++i)
	{ 
		while(mot[j] != mot[i-1] && j >= 0)
			j = bord[j];

		bord[i] = ++j;
	}

	return bord;
}

automate_det* automate_localisation(const char* mot)
{
	int sz = (int)strlen(mot);
	int* bord = table_bord(mot);

	//Determine l'alphabet du motif
	int* alphabet = NULL;
	int n_alphabet = 26;
	get_alphabet(&mot, 1, &alphabet, &n_alphabet);

	//Creation de l'automate de localisation
	automate_det* Du = init_automate(sz+1, n_alphabet, 0, &sz, 1, alphabet);

	int cur_letter = map_letter(mot[0], Du);
	Du->matTransition[0][cur_letter] = 1;

	int i, j;
	for(i = 0; i < Du->n_alphabet; ++i) {
		if(i != cur_letter)
			Du->matTransition[0][i] = 0;
	}

	for(i = 1; i < sz; ++i)
	{
		cur_letter = map_letter(mot[i], Du);
		
		Du->matTransition[i][cur_letter] = i+1;
		for(j = 0; j < Du->n_alphabet; ++j) {
			if(j != cur_letter)
				Du->matTransition[i][j] = Du->matTransition[bord[i]][j];
		}
	}

	for(i = 0; i < Du->n_alphabet; ++i)
		Du->matTransition[sz][i] = Du->matTransition[bord[sz]][i];

	free(bord);

	/*for(i = 0; i < sz+1; ++i)
	{
		printf("etape %d: ", i);
		for(j = 0; j < n_alphabet; ++j)
		{ printf("%d\t", Du->matTransition[i][j]); }
		printf("\n");
	}*/

	return Du;
}

void get_alphabet(const char** liste_mot, const int nb_mot, int** alphabet, int* n_alphabet)
{
	*n_alphabet = 0;

	*alphabet = (int*)malloc(sizeof(int) * 36);
	if(*alphabet == NULL) {
		fprintf(stderr, "Impossible d'allouer la mémoire");
		exit(1);
	}

	memset(*alphabet, -1, sizeof(int)*36); //-1 valeur discriminante

	int i, j;
	int sz;
	int tmp = 0;

	for(j = 0; j < nb_mot; ++j)
	{
		sz = (int)strlen(liste_mot[j]);

		for(i = 0; i < sz; ++i)
		{
			if(liste_mot[j][i] >= 'a' && liste_mot[j][i] <= 'z')
				tmp = (int)liste_mot[j][i]-97;
			else if(liste_mot[j][i] >= '0' && liste_mot[j][i] <= '9')
				tmp = (int)liste_mot[j][i]-22;

			if((*alphabet)[tmp] == -1) {
				(*alphabet)[tmp] = *n_alphabet;
				*n_alphabet += 1;
			}
		}
	}
}

automate_det* build_trie(char** list, const int n_elem, const int sz_all_words)
{
	//Get the alphabet my friend !
	int* alphabet = NULL;
	int n_alphabet = 0;
	get_alphabet((const char**)list, n_elem, &alphabet, &n_alphabet);

	/*int i;
	for(i = 0; i < 26; ++i)
		printf("i: %d\t alphabet[%d]: %d\n", i, i, alphabet[i]);*/


	automate_det* M = (automate_det*)malloc(sizeof(automate_det));
	if(M == NULL) {
		fprintf(stderr, "%s: Impossible d'allouer de la mémoire", __FILE__);
		exit(1);
	}

	M->qInit = 0;
	M->nbEtats = 1;
	M->nbEtatsFinals = 0;
	M->curseur = 0;
	M->n_alphabet = n_alphabet;
	M->map_alphabet = alphabet;

	//Init qFinals
	M->qFinals = (int*)malloc(sizeof(int) * (sz_all_words+1));
	if(M->qFinals == NULL) {
		fprintf(stderr, "%s: Impossible d'allouer de la mémoire", __FILE__);
		exit(1);
	}

	//Init matTransition
	//au pire des cas nEtats = |Prefix(X)|
	M->matTransition = (int**)malloc(sizeof(int*) * (sz_all_words+1));
	if(M->matTransition == NULL) {
		fprintf(stderr, "%s: Impossible d'allouer de la mémoire", __FILE__);
		exit(1);
	}

	int i, j;
	for(i = 0; i < sz_all_words+1; ++i)
	{
		M->matTransition[i] = (int*)malloc(sizeof(int) * n_alphabet);
		if(M->matTransition[i] == NULL) {
			fprintf(stderr, "%s: Impossible d'allouer de la mémoire", __FILE__);
			exit(1);
		}

		memset(M->matTransition[i], -1, n_alphabet*sizeof(int));
	}

	//Debut de l'algo !
	int max = 1;
	int etat = 0;
	int sz_word = 0;

	for(i = 0; i < n_elem; ++i)
	{
		etat = 0;
		sz_word = (int)strlen(list[i]);
		for(j = 0; j < sz_word; ++j)
		{
			if(M->matTransition[etat][map_letter(list[i][j], M)] == -1) {
				M->nbEtats += 1;
				M->matTransition[etat][map_letter(list[i][j], M)] = max;
				max = max +1;
			}
			etat = M->matTransition[etat][map_letter(list[i][j], M)];
		}
		M->qFinals[M->nbEtatsFinals] = etat;
		M->nbEtatsFinals += 1;
	}

	/*for(i = 0; i < M->nbEtats; ++i)
	{
		printf("etape %d: ", i);
		for(j = 0; j < n_alphabet; ++j)
		{ printf("%d\t", M->matTransition[i][j]); }
		printf("\n");
	}

	printf("Etats Finals :\n");
	for(i = 0; i < M->nbEtatsFinals; ++i)
		printf("\t%d\t", M->qFinals[i]);
	printf("\n");*/

	return M;
}

automate_det* aho_corasick(char** list, const int n_elem, const int sz_all_words)
{
	automate_det* Dx = build_trie(list, n_elem, sz_all_words);

	file_int F = init_file();
	int failure[Dx->nbEtats];
	memset(failure, 0, sizeof(int)*Dx->nbEtats);

	int i, j;
	int n_alphabet = Dx->n_alphabet;

	for(i = 0; i < n_alphabet; ++i)
	{
		j = Dx->matTransition[0][i];
		if(j == -1) {
			Dx->matTransition[0][i] = 0;
		} else {
			failure[j] = 0;
			enfiler(j, &F);
		}
	}

	int tmp;
	while(!est_vide(&F))
	{
		tmp = defiler(&F);
		if(etat_is_final(Dx, failure[tmp])) {
			Dx->qFinals[Dx->nbEtatsFinals] = tmp;
			Dx->nbEtatsFinals += 1;
		}

		for(i = 0; i < n_alphabet; ++i)
		{
			j = Dx->matTransition[tmp][i];
			if(j == -1) {
				Dx->matTransition[tmp][i] = Dx->matTransition[failure[tmp]][i];
			} else {
				failure[j] = Dx->matTransition[failure[tmp]][i];
				enfiler(j, &F);
			}
		}
	}

	/*for(i = 0; i < Dx->nbEtats; ++i)
	{
		printf("etape %d: ", i);
		for(j = 0; j < Dx->n_alphabet; ++j)
		{ printf("%d\t", Dx->matTransition[i][j]); }
		printf("\n");
	}

	printf("Etats Finals :\n");
	for(i = 0; i < Dx->nbEtatsFinals; ++i)
		printf("\t%d\t", Dx->qFinals[i]);
	printf("\n");*/

	return Dx;
}
