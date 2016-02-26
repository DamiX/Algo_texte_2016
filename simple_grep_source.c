#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "lib/algo.h"

#define true 1
#define false 0
#define _DEBUG

typedef int bool;

const int sz_buffer = 1024;

inline void print_usage(const char*);

// Charset checking
inline bool valid_caractere(const char);
inline void strtolower(char*);
char* normalize_string(char*);

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	bool nFlag = false;
	bool lFlag = false;
	char* motif = argv[argc-2];
	const char* fichier = argv[argc-1];

	// Normalise l'entrée
	strtolower(motif);
	motif = normalize_string(motif);

	// Prise en charge des options
	int o;
	while((o = getopt(argc, argv, "nl")) != -1)
	{
		switch(o)
		{
			case 'n':
				nFlag = true;
			break;

			case 'l':
				lFlag = true;
			break;

			case '?':
				print_usage(argv[0]);
				return 1;
			break;

			default:
				exit(1);
		}
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

	//for(i = 0; i < 26; ++i)
	//	printf("i: %d\t v: %d\n", i, sgrepAutomata->map_alphabet[i]);

	while((c = tolower(fgetc(txt))) != EOF)
	{
		//printf("%c", c);
		if(c == '\n') {
			line++;
			i = 0;

			if(occ > tmpocc) {
				if(lFlag)
					printf("l.%d - ", line-1);
				printf("%s\n", buffer);
				tmpocc++;
			}
				
			memset(buffer, 0, sz_buffer);
		} else {
			buffer[i++] = c;
		}

		if(!valid_caractere(c))
			continue;

		read_letter(sgrepAutomata, c);
		if(is_final(sgrepAutomata))
			occ = occ + 1;

	}

	if(nFlag)
		printf("Nombre d'ocurrence : %d\n", occ);

	free_automate_det(sgrepAutomata);
	fclose(txt);
	free(motif);

	return 0;
}

inline void print_usage(const char* appName) {
	printf("Usage: %s [options] motif fichier\n\nOptions:\n\t-n\tAffiche le nombre d'occurrence du motif\n\t-l\tAffiche le numéro de ligne des occurrences\n", appName);
}

inline bool valid_caractere(const char c) {
	return (c >= 'a' && c <= 'z' || c >= '0' && c <= '9') ? true : false;
}

inline void strtolower(char* str) {
	while((*str = tolower(*str)) != 0){++str;}
}

char* normalize_string(char* myString)
{
	int i, j = 0;
	int sz = (int)strlen(myString);
	
	char* buffer = malloc(sizeof(char) * sz+1);
	if(buffer == NULL) {
		fprintf(stderr, "Impossible d'allouer de la mémoire");
		exit(-1);
	}

	for(i = 0; i < sz; ++i) {
		if(valid_caractere(myString[i]))
			buffer[j++] = myString[i];
	}
	buffer[j] = '\0';

	return buffer;
}
			
