#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../lib/algo.h"

const int sz_buffer = 2048;

inline void print_usage(const char* appName) {
	printf("Usage: %s [options] motif fichier\n\nOptions:\n\t-n\tAffiche le nombre d'occurrence du motif\n\t-l\tAffiche le numéro de ligne des occurrences\n\t-i\tInsensible à la case\n", appName);
}

inline int without_tolower(FILE* f) {
	return fgetc(f);
}

inline int with_tolower(FILE* f) {
	return tolower(fgetc(f));
}

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	bool nFlag = false;
	bool lFlag = false;
	bool iFlag = false;
	char* motif = argv[argc-2];
	const char* fichier = argv[argc-1];
	const char* ansi_red_color = "\x1b[1;32m";
	const char* ansi_reset_color = "\x1b[0m";

	// Normalise l'entrée
	motif = normalize_string(motif);
	int sz_motif = (int)strlen(motif);

	// Prise en charge des options
	int o;
	while((o = getopt(argc, argv, "nli")) != -1)
	{
		switch(o)
		{
			case 'n':
				nFlag = true;
			break;

			case 'l':
				lFlag = true;
			break;

			case 'i':
				iFlag = true;
			break;

			case '?':
				print_usage(argv[0]);
				return 0;
			break;

			default:
				exit(1);
		}
	}

	//Case-sensitive ?
	int (*ptr_f)(FILE*);
	if(iFlag) {
		ptr_f = with_tolower;
		strtolower(motif);
	} else {
		ptr_f = without_tolower;
	}

	//Creons l'automate
	automate_det* sgrepAutomata = automate_localisation(motif);

	FILE *txt = fopen(fichier, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, fichier);
		free(motif);
		exit(1);
	}

	char c;
	char buffer[sz_buffer];
	memset(buffer, 0, sz_buffer);

	int occ = 0, tmpocc = 0, i = 0, line = 1;

	while((c = (*ptr_f)(txt)) != EOF)
	{
		buffer[i%sz_buffer] = c; //Avoid overflow
		i = i + 1;

		if(!is_valid_caractere((int)c))
			continue;

		if(c == '\n') {
			if(occ > tmpocc) {

				if(lFlag)
					printf("\x1b[1;36ml.%d\x1b[0m - ", line);

				buffer[i-1] = '\0';				
				printf("%s\n", buffer);
				tmpocc++;
			}
			
			i = 0;
			line = line + 1;
			memset(buffer, 0, sz_buffer);
		}

		read_letter(sgrepAutomata, c);
		if(is_final(sgrepAutomata)) {
			// Fancy color !
			buffer[i-sz_motif] = '\0';
			strcat(buffer, ansi_red_color);
			strcat(buffer, motif);
			strcat(buffer, ansi_reset_color);
			i += 11;
			occ = occ + 1;
		}

	}

	if(nFlag)
		printf("Nombre d'ocurrence : %d\n", occ);

	free_automate_det(sgrepAutomata);
	fclose(txt);
	free(motif);

	return 0;
}		
