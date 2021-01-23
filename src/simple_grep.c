#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "include/algorithm.h"
#include "include/cli.h"
#include "include/helper.h"

const int sz_buffer = 2048;

static inline int without_tolower(FILE* f) {
	return fgetc(f);
}

static inline int with_tolower(FILE* f) {
	return tolower(fgetc(f));
}

static inline void print_usage(const char* appName) {
	printf("Usage: %s [options] pattern file\n \
Search for pattern in file.\n \
	Options:\n \
	-n	Print number of matches\n \
	-l	Print line number containing match\n \
	-i	Ignore case-sensitivity\n", appName);
}

int main(int argc, char** argv)
{
	cli_args args = parse_cli_args(argc, argv, print_usage);

	char* pattern = norm_ascii_str(args.positional[0]);
	char* input_file = args.positional[1];
	const int sz_motif = (int)strlen(pattern);

	//Case-sensitive ?
	int (*ptr_f)(FILE*);
	if(args.iFlag) {
		ptr_f = with_tolower;
		strtolower(pattern);
	} else {
		ptr_f = without_tolower;
	}

	deterministic_automata* sgrepAutomata = automate_localisation(pattern);

	FILE *txt = fopen(input_file, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, input_file);
		free(pattern);
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

		if(!is_valid_ascii_char(c))
			continue;

		if(c == '\n') {
			if(occ > tmpocc) {

				if(args.lFlag)
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
		if(is_final_automata(sgrepAutomata)) {
			buffer[i-sz_motif] = '\0';
			strcat(buffer, shell_red_color);
			strcat(buffer, pattern);
			strcat(buffer, shell_reset_color);
			i += 11;	///< shell_reset_color size
			occ = occ + 1;
		}

	}

	fclose(txt);
	free_deterministic_automata(sgrepAutomata);
	free(pattern);

	if(args.nFlag)
		printf("Found: %d occurrences\n", occ);

	return 0;
}
