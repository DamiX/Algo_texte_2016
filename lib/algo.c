#include "algo.h"

const int ASCII = 128;

inline void print_error(const char* err) {
	fprintf(stderr, "%s: %s\n%s\n", __FILE__, err, strerror(errno));
	exit(1);
}

//Charset
inline bool is_valid_caractere(const int c) {
	return (c >= 0 && c <= ASCII) ? true : false;
}

inline void strtolower(char* str) {
	while((*str = tolower(*str)) != 0){++str;}
}

char* normalize_string(const char* myString)
{
	int i = 0;
	int sz = (int)strlen(myString);
	
	char* buffer = (char*)malloc(sizeof(char) * sz+1);
	if(buffer == NULL)
		print_error("Unable to allocate memory for normalize_string");

	while(*myString != 0)
	{
		if(is_valid_caractere((int)*myString))
			buffer[i++] = *myString++;
	}
	buffer[i] = '\0';

	return buffer; //!! DO NOT FORGET TO FREE !!
}

int* table_bord(const char* mot)
{
	size_t sz = strlen(mot)+1;

	int* bord = (int*)malloc(sizeof(int) * sz);
	if(bord == NULL)
		print_error("Unable to allocate memory for table_bord");

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

	return bord;
}

automate_det* automate_localisation(const char* mot)
{
	int sz = (int)strlen(mot);
	int* bord = table_bord(mot);

	//Determine l'alphabet du motif
	int* alphabet = NULL;
	int n_alphabet = 0;
	get_alphabet(&mot, 1, &alphabet, &n_alphabet);

	//Creation de l'automate de localisation
	automate_det* Du = (automate_det*)malloc(sizeof(automate_det));
	if(Du == NULL)
		print_error("Unable to allocate memory for the automata");

	Du->qInit = 0;
	Du->nbEtats = sz+1;
	Du->nbEtatsFinals = 1;
	Du->curseur = 0; //qInit
	Du->nAlphabet = n_alphabet;
	Du->mapAlphabet = alphabet;
	Du->sizeAllocate = sz+1;

	// Etat final
	Du->qFinals = (int*)malloc(sizeof(int));
	if(Du->qFinals == NULL)
		print_error("Unable to allocate memory for final states");

	Du->qFinals[0] = sz;

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

	*alphabet = (int*)malloc(sizeof(int) * ASCII);
	if(*alphabet == NULL)
		print_error("Unable to allocate memory the alphabet");

	memset(*alphabet, -1, sizeof(int)*ASCII); //-1 valeur discriminante

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

automate_det* build_trie(char** list, const int n_elem, const int sz_all_words)
{
	//Get the alphabet my friend !
	int* alphabet = NULL;
	int n_alphabet = 0;
	get_alphabet((const char**)list, n_elem, &alphabet, &n_alphabet);

	// Créer l'automate
	automate_det* M = (automate_det*)malloc(sizeof(automate_det));
	if(M == NULL)
		print_error("Unable to allocate memory for the trie");

	M->qInit = 0;
	M->nbEtats = 1;
	M->nbEtatsFinals = 0;
	M->curseur = 0;
	M->nAlphabet = n_alphabet;
	M->mapAlphabet = alphabet;
	M->sizeAllocate = sz_all_words+1;

	//Init qFinals
	M->qFinals = (int*)malloc(sizeof(int) * (sz_all_words+1));
	if(M->qFinals == NULL)
		print_error("Unable to allocate memory for final states");

	//Init matTransition
	//au pire des cas nEtats = |Prefix(X)|
	M->matTransition = init_transition((sz_all_words+1), n_alphabet);

	//Debut de l'algo !
	int i, j;
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
				M->nbEtats += 1;
				M->matTransition[etat][cur_letter] = max;
				max = max +1;
			}
			etat = M->matTransition[etat][cur_letter];
		}
		M->qFinals[M->nbEtatsFinals] = etat;
		M->nbEtatsFinals += 1;
	}

	return M;
}

automate_det* aho_corasick(char** list, const int n_elem, const int sz_all_words)
{
	automate_det* Dx = build_trie(list, n_elem, sz_all_words);

	file_int F = init_file();
	int failure[Dx->nbEtats];
	memset(failure, 0, sizeof(int)*Dx->nbEtats);

	int i, j;
	int n_alphabet = Dx->nAlphabet;

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
		if(etat_est_final(Dx, failure[tmp])) {
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

	return Dx;
}
