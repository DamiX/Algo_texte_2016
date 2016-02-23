#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

typedef int bool;

void print_usage(char*);

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	bool nFlag = FALSE;
	bool lFlag = FALSE;
	const char* motif = argv[argc-2];
	const char* fichier = argv[argc-1];

	int c;
	while((c = getopt(argc, argv, "nl")) != -1)
	{
		switch(c)
		{
			case 'n':
				nFlag = TRUE;
			break;

			case 'l':
				lFlag = TRUE;
			break;

			case '?':
				print_usage(argv[0]);
				return 1;
			break;

			default:
				exit(1);
		}
	}

	#ifdef _DEBUG
	printf("nflag = %d\t lflag = %d\n", nFlag, lFlag);
	#endif

	/*FILE *txt = fopen(fichier, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier introuvable\n", __FILE__);
		exit(1);
	}

	

	fclose(txt);*/

	return 0;
}

void print_usage(char* appName) {
	printf("Usage: %s [options] motif fichier\n\n", appName);
	printf("Options:\n\t-n\tAffiche le nombre d'occurrence du motif\n");
	printf("\t-l\tAffiche le numéro de ligne des occurrences\n");
}
