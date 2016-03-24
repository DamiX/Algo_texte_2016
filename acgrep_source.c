#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "lib/algo.h"

// CONST VARIABLE
const int buffer_size = 1024;

inline void print_usage(const char* appName) {
	printf("Usage: %s fichier repertoire\n", appName);
}

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	const char* fichier = argv[1];
	const char* repertoire = argv[2];

	FILE *txt = fopen(fichier, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, fichier);
		exit(1);
	}

	int i = 0, j = 0, k = 0;
	char c;
	char buffer[buffer_size];
	char* list_words[buffer_size];

	// Lit et stock la liste des mots dans un tableau
	while((c = tolower(fgetc(txt))) != EOF)
	{
		if(c == '\n') {
			buffer[i] = '\0';

			list_words[j] = (char*)malloc(sizeof(char) * i+1);
			memcpy(list_words[j++], buffer, i+1);

			i = 0;
		} else {
			buffer[i++] = (char)c;
			k += 1;
		}
	}
	
	//Libère la mémoire
	fclose(txt);

	//Creons l'automate
	automate_det* Dx = aho_corasick(list_words, j, k);

	// Lit et stock le nom des dossier dans le répertoire
	DIR* cur_dir = NULL;
	struct dirent* cur_file = NULL;

	if((cur_dir = opendir(repertoire)) == NULL) {
		fprintf(stderr, "%s: Impossible d'accer au repertoire \"%s\"", __FILE__, repertoire);
		exit(1);
	}

	// Parcours du répertoire
	while((cur_file = readdir(cur_dir)) != NULL)
	{
		if(cur_file->d_type == DT_REG)
		{
			txt = fopen(cur_file->d_name, "r");
			if(txt == NULL) {
				fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, cur_file->d_name);
				exit(1);
			}

			printf("=================\nFichier: %s\n=================\n", cur_file->d_name);
			while((c = tolower(fgetc(txt))) != EOF)
			{
				if(!is_valid_caractere((int)c))
					continue;

				read_letter(Dx, c);
				if(is_final(Dx)) {
					printf("Occurence trouvée: %s\n", Dx->correspondance[Dx->cursor]);
				}
			}

			fclose(txt);
		}
		printf("\n");
	}
	

	//Libération de la mémoire
	closedir(cur_dir);

	free_automate_det(Dx);

	for(i = 0; i < j; ++i)
		free(list_words[i]);

	return 0;
}
