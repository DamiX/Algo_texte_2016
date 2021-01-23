#include "file.h"

file_int init_file()
{
	file_int F;
	F.tete = 0;
	F.queue = 0;
	F.n = 0;
	F.taille = 1024;

	F.tab = (int*)malloc(sizeof(int) * 1024); //4 Mo
	if(F.tab == NULL) {
		fprintf(stderr, "%s: Allocation impossible", __FILE__);
		exit(1);
	}

	return F;
}

void free_file(file_int* F)
{
	free(F->tab);
}

//Accesseur
void enfiler(const int x, file_int* F)
{
	if(F->n == F->taille) //la file est remplie -> realloc
	{
		int new_size = F->taille*2;

		int* new_tab = (int*)malloc(sizeof(int) * new_size);
		if(new_tab == NULL) {
			fprintf(stderr, "%s: Allocation impossible", __FILE__);
			exit(1);
		}

		memcpy((void*)new_tab, (void*)F->tab, sizeof(int)*F->taille);
		free(F->tab);
		F->tab = new_tab;
		F->taille = new_size;
	}
	
	F->tab[F->queue] = x;
	F->n += 1;
	if(F->queue == F->taille-1)
		F->queue = 1;
	else
		F->queue += 1;
}

int defiler(file_int* F)
{
	int x = F->tab[F->tete];
	F->n -= 1;
	if(F->tete == F->taille)
		F->tete = 1;
	else
		F->tete += 1;

	return x;
}

inline int est_vide(file_int* F) {
	return (F->n == 0) ? 1 : 0;
}
