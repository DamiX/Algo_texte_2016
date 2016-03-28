#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "../lib/algo.h"

// CONST VARIABLE
const int buf_size = 1024; //1Ko

inline void print_usage(const char*);

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	const char* fichier = argv[1];
	const char* executable = argv[2];

	FILE *txt = fopen(fichier, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, fichier);
		exit(1);
	}

	int i = 0, j = 0, k = 0;
	char c;
	char buffer[buf_size];
	char* list_words[buf_size];

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

	int fdesc;
	if((fdesc = open(executable, O_RDONLY)) == -1) {
		fprintf(stderr, "%s: Impossible d'ouvrir l'executable %s. Error : %d", __FILE__,
		executable, errno);
		exit(1);
	}
	
	while(read(fdesc, buffer, (size_t)buf_size) != 0)
	{
		for(i = 0; i < buf_size; ++i)
		{
			//Converti les octets en bits avant de lire dans l'automate
			for(k = 1<<7; k > 0; k=k>>1) {
				read_letter(Dx, (buffer[i] & k ? '1' : '0'));
				if(is_final(Dx)) {
					printf("Occurence trouvée: %s\n", Dx->correspondance[Dx->cursor]);
				}
			}
		}
	}

	//Libère la mémoire
	close(fdesc);
	free_automate_det(Dx);

	for(i = 0; i < j; ++i)
		free(list_words[i]);

	return 0;
}

inline void print_usage(const char* appName) {
	printf("Usage: %s fichier repertoire\n", appName);
}
