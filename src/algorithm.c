#include "include/algorithm.h"
#include "include/queue.h"
#include "include/helper.h"

static const char SENTINEL_VALUE = -1;

int* table_bord(const char* mot)
{
	const size_t sz = strlen(mot)+1;

	int* bord = (int*)malloc(sizeof(int) * sz);
	if(bord != NULL) {
		memset(bord, 0, sizeof(int)*sz);

		bord[0] = -1;

		int i = 0, j = 0;
		for(i = 2; i < sz; ++i)
		{
			/* j peut être égal à -1, il est préférable d'utiliser la propriété de court
			circuit de l'opérateur && à bon escient */
			while(j >= 0 && mot[j] != mot[i-1]) //j can be -1
				j = bord[j];

			bord[i] = ++j;
		}
	}

	return bord;
}

deterministic_automata* automate_localisation(const char* mot)
{
	int sz = (int)strlen(mot);
	int* bord = table_bord(mot);

	//Determine l'alphabet du pattern
	int* alphabet = NULL;
	int n_alphabet = 0;
	get_alphabet(&mot, 1, &alphabet, &n_alphabet);

	//Creation de l'automate de localisation
	deterministic_automata* Du = (deterministic_automata*)malloc(sizeof(deterministic_automata));
	if(Du == NULL)
		return NULL;

	Du->qInit = 0;
	Du->nStates = sz+1;
	Du->nFinalStates = 1;
	Du->cursor = 0; //qInit
	Du->nAlphabet = n_alphabet;
	Du->mapAlphabet = alphabet;
	Du->sizeAllocate = sz+1;

	// Etat final
	Du->qFinals = (int*)malloc(sizeof(int));
	if(Du->qFinals == NULL)
		return NULL;

	Du->qFinals[0] = sz;

	//Correspondance
	Du->correspondance = NULL;

	//Transition
	Du->matTransition = init_transition(sz+1, n_alphabet);

	// Début de l'algorithme (Création des transitions)
	int cur_letter = map_letter(mot[0], Du);
	Du->matTransition[0][cur_letter] = 1;

	int i, j;
	for(i = 0; i < Du->nAlphabet; ++i) {
		if(i != cur_letter)
			Du->matTransition[0][i] = 0;
	}

	for(i = 1; i < sz; ++i)
	{
		cur_letter = map_letter(mot[i], Du);

		Du->matTransition[i][cur_letter] = i+1;
		for(j = 0; j < Du->nAlphabet; ++j) {
			if(j != cur_letter)
				Du->matTransition[i][j] = Du->matTransition[bord[i]][j];
		}
	}

	for(i = 0; i < Du->nAlphabet; ++i)
		Du->matTransition[sz][i] = Du->matTransition[bord[sz]][i];

	free(bord);

	return Du; //!! Ne pas oublier de libérer la mémoire
}

void get_alphabet(const char** liste_mot, const int nb_mot, int** alphabet, int* n_alphabet)
{
	*n_alphabet = 0;

	const int alphabet_size = sizeof(int) * max_ascii_char;
	*alphabet = (int*)malloc(alphabet_size);
	if(*alphabet == NULL)
		return;

	memset(*alphabet, SENTINEL_VALUE, alphabet_size);

	int i, j;
	int sz;
	int cur_letter;

	for(j = 0; j < nb_mot; ++j)
	{
		sz = (int)strlen(liste_mot[j]);

		for(i = 0; i < sz; ++i)
		{
			cur_letter = (int)liste_mot[j][i];
			if((*alphabet)[cur_letter] == -1) {
				(*alphabet)[cur_letter] = *n_alphabet;
				*n_alphabet += 1;
			}
		}
	}
}

deterministic_automata* build_trie(char** list, const int n_elem, const int sz_all_words)
{
	//Get the alphabet my friend !
	int* alphabet = NULL;
	int n_alphabet = 0;
	get_alphabet((const char**)list, n_elem, &alphabet, &n_alphabet);

	// Créer l'automate
	deterministic_automata* M = (deterministic_automata*)malloc(sizeof(deterministic_automata));
	if(M == NULL)
		return NULL;

	M->qInit = 0;
	M->nStates = 1;
	M->nFinalStates = 0;
	M->cursor = 0;
	M->nAlphabet = n_alphabet;
	M->mapAlphabet = alphabet;
	M->sizeAllocate = sz_all_words+1;

	//Init qFinals
	M->qFinals = (int*)malloc(sizeof(int) * (sz_all_words+1));
	if(M->qFinals == NULL)
		return NULL;

	//Correspondance
	M->correspondance = (char**)malloc(sizeof(char*) * (sz_all_words+1));
	if(M->correspondance == NULL)
		print_error("Unable to allocate memory for correspondance");

	int i;
	for(i = 0; i < sz_all_words+1; ++i) {
		M->correspondance[i] = (char*)malloc(sizeof(char) * 128);
		if(M->correspondance[i] == NULL)
			print_error("Unable to allocate memory for correspondance");

		memset(M->correspondance[i], 0, 128);
	}

	//Init matTransition
	//au pire des cas nEtats = |Prefix(X)|
	M->matTransition = init_transition((sz_all_words+1), n_alphabet);

	//Debut de l'algo !
	int j;
	int max = 1;
	int etat = 0;
	int sz_word = 0;
	int cur_letter;

	for(i = 0; i < n_elem; ++i)
	{
		etat = 0;
		sz_word = (int)strlen(list[i]);
		for(j = 0; j < sz_word; ++j)
		{
			cur_letter = map_letter(list[i][j], M);

			if(M->matTransition[etat][cur_letter] == -1) {
				M->nStates += 1;
				M->matTransition[etat][cur_letter] = max;
				max = max +1;
			}
			etat = M->matTransition[etat][cur_letter];
		}
		M->qFinals[M->nFinalStates] = etat;
		M->nFinalStates += 1;
		strcpy(M->correspondance[etat], list[i]);
	}

	return M;
}

deterministic_automata* aho_corasick(char** list, const int n_elem, const int sz_all_words)
{
	deterministic_automata* Dx = build_trie(list, n_elem, sz_all_words);

	int_queue Q = init_queue();
	int failure[Dx->nStates];
	memset(failure, 0, sizeof(int)*Dx->nStates);

	int i, j;
	int n_alphabet = Dx->nAlphabet;

	for(i = 0; i < n_alphabet; ++i)
	{
		j = Dx->matTransition[0][i];
		if(j == -1) {
			Dx->matTransition[0][i] = 0;
		} else {
			failure[j] = 0;
			push_queue(j, &Q);
		}
	}

	int tmp;
	while(!is_queue_empty(&Q))
	{
		tmp = pop_queue(&Q);
		if(is_automata_state_final(Dx, failure[tmp])) {
			Dx->qFinals[Dx->nFinalStates] = tmp;
			Dx->nFinalStates += 1;
			strcpy(Dx->correspondance[tmp], list[failure[tmp]]);
		}

		for(i = 0; i < n_alphabet; ++i)
		{
			j = Dx->matTransition[tmp][i];
			if(j == -1) {
				Dx->matTransition[tmp][i] = Dx->matTransition[failure[tmp]][i];
			} else {
				failure[j] = Dx->matTransition[failure[tmp]][i];
				push_queue(j, &Q);
			}
		}
	}

	free_queue(&Q);
	return Dx;
}
