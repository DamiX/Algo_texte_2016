#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "include/algorithm.h"
#include "include/cli.h"
#include "include/helper.h"

// CONST VARIABLE
#define BUFFER_SIZE 1024

static inline void print_usage(const char* appName) {
	printf("Usage: %s [option] file directory\n \
Search for patterns in a directory\n \
	Options:\n \
	-b	Enable binary read mode\n", appName);
}

char* build_path(const char* directory, const char* filepath)
{
	char* path = (char*)malloc(sizeof(char) * (strlen(directory)+strlen(filepath)+1));
	strcat(path, directory);
	strcat(path, "/");
	strcat(path, filepath);

	return path;
}

void plain_text_search(char* filepath, deterministic_automata* Dx) {
	FILE* txt = fopen(filepath, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s: Cannot open \"%s\"\n", __FILE__, filepath);
		return;
	}

	int i = 0;
	int occ = 0, tmpocc = 0, sz_correspondance = 0, line = 1;
	char buffer[BUFFER_SIZE];
	char c;
	while((c = tolower(fgetc(txt))) != EOF)
	{
		buffer[i%BUFFER_SIZE] = c;
		i = i + 1;

		if(!is_valid_ascii_char(c))
			continue;

		if(c == '\n')
		{
			if(occ > tmpocc) {
				if(tmpocc == 0) {
					printf("=================\nFile: %s\n=================\n", filepath);
				}
				buffer[i-1] = '\0';
				printf("\x1b[1;36ml.%d\x1b[0m -  %s\n", line, buffer);
				tmpocc++;
			}

			i = 0;
			line = line + 1;
			memset(buffer, 0, BUFFER_SIZE);
		}

		read_letter(Dx, c);
		if(is_final_automata(Dx))
		{
			// Gibberish for pretty printing
			sz_correspondance = strlen(Dx->correspondance[Dx->cursor]);
			buffer[i-sz_correspondance] = '\0';
			strcat(buffer, shell_green_color);
			strcat(buffer, Dx->correspondance[Dx->cursor]);
			strcat(buffer, shell_reset_color);
			i += 11;
			occ = occ + 1;
		}
	}

	fclose(txt);
}

void binary_search(char* filepath, deterministic_automata* Dx) {
	int fdesc;
	if((fdesc = open(filepath, O_RDONLY)) == -1) {
		fprintf(stderr, "%s: Cannot open %s (%d)", __FILE__, filepath, errno);
		exit(1);
	}

	int i = 0, j = 0, occ = 0, offset = 0;
	char buffer[BUFFER_SIZE];
	while(read(fdesc, buffer, (size_t)BUFFER_SIZE) != 0)
	{
		for(i = 0; i < BUFFER_SIZE; ++i)
		{
			// Convert byte -> bit before processing
			for(j = 1<<7; j > 0; j=j>>1) {
				read_letter(Dx, (buffer[i] & j ? '1' : '0'));
				if(is_final_automata(Dx)) {
					if(occ == 0) {
						printf("================\n%s%s%s\n================\n", shell_green_color, filepath, shell_reset_color);
					}
					printf("Offset: %d - %s%s%s\n", offset, shell_red_color, Dx->correspondance[Dx->cursor], shell_reset_color);
				}
			}

			offset += 8;
		}
	}

	close(fdesc);
}

int main(int argc, char** argv)
{
	const cli_args args = parse_cli_args(argc, argv, print_usage);

	const char* filepath = args.positional[0];
	const char* directory = args.positional[1];

	words w = read_words(filepath, args.bFlag);

	deterministic_automata* Dx = aho_corasick(w.values, w.n, BUFFER_SIZE);

	DIR* cur_dir = NULL;
	struct dirent* cur_file = NULL;

	if((cur_dir = opendir(directory)) == NULL) {
		fprintf(stderr, "%s: Access to \"%s\" directory denied\n", __FILE__, directory);
		exit(1);
	}

	while((cur_file = readdir(cur_dir)) != NULL)
	{
		if(cur_file->d_type == DT_REG)
		{
			char* path = build_path(directory, cur_file->d_name);

			if (args.bFlag)
				binary_search(path, Dx);
			else
				plain_text_search(path, Dx);

			free(path);
		}
	}

	closedir(cur_dir);
	free_deterministic_automata(Dx);
	free_words(w);

	return 0;
}
