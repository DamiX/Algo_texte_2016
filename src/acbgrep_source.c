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
const char *ansi_green_color = "\x1b[1;32m";
const char *ansi_red_color = "\x1b[1;31m";
const char *ansi_reset_color = "\x1b[0m";

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

	// Normalise le repertoire
	strcat(argv[2], "/");
	const char* repertoire = argv[2];

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
	while((c = fgetc(txt)) != EOF)
	{
		if(c != '0' && c != '1' && c != '\n')
			continue;

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
		fprintf(stderr, "%s: Impossible d'acceder au repertoire \"%s\"\n", __FILE__, repertoire);
		exit(1);
	}

	// Parcours du répertoire
	char path[buf_size];
	int occ = 0, offset = 0;
	while((cur_file = readdir(cur_dir)) != NULL)
	{
		if(cur_file->d_type == DT_REG)
		{
			// Construit le chemin du fichier
			memset(path, 0, buf_size);
			strcat(path, repertoire);
			strcat(path, cur_file->d_name);

			int fdesc;
			if((fdesc = open(path, O_RDONLY)) == -1) {
				fprintf(stderr, "%s: Impossible d'ouvrir l'executable %s. Error : %d", __FILE__,
				path, errno);
				exit(1);
			}

			i = 0, occ = 0, offset = 0;	
			while(read(fdesc, buffer, (size_t)buf_size) != 0)
			{
				for(i = 0; i < buf_size; ++i)
				{
					//Converti les octets en bits avant de lire dans l'automate
					for(k = 1<<7; k > 0; k=k>>1) {
						read_letter(Dx, (buffer[i] & k ? '1' : '0'));
						if(is_final(Dx)) {
							if(occ == 0) {
								printf("================\n%s%s%s\n================\n", ansi_green_color, cur_file->d_name, ansi_reset_color);
							}
							printf("Offset: %d - %s%s%s\n", offset, ansi_red_color, Dx->correspondance[Dx->cursor], ansi_reset_color);
						}
					}

					offset += 8;
				}
			}

			close(fdesc);
		}
	}

	//Libère la mémoire
	free_automate_det(Dx);

	for(i = 0; i < j; ++i)
		free(list_words[i]);

	return 0;
}
