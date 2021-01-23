#ifndef __H_FILE__
#define __H_FILE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _file{
	int tete;
	int queue;
	int n;
	int taille;
	int* tab;
} file_int;

//Constructeur & Destructeur
file_int init_file();
void free_file(file_int*);

//Accesseur
void enfiler(const int, file_int*);
int defiler(file_int*);

inline int est_vide(file_int*);

#endif
