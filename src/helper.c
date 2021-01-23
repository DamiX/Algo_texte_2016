#include "include/helper.h"

const int max_ascii_char = 0x80;

char* norm_ascii_str(const char* str) {
    const int sz = (int)strlen(str);

    int i = 0;
    char* buffer = (char*)malloc(sizeof(char) * sz);
    if (buffer != NULL) {
        while (*str != 0) {
            if (is_valid_ascii_char(*str))
                buffer[i++] = *str++;
        }

        buffer[i] = 0;
    }

    return buffer;
}

void free_words(words w) {
	for(int i = 0; i < w.n; ++i)
		free(w.values[i]);
}

words read_words(const char* filepath, bool is_binary_mode) {
	FILE *txt = fopen(filepath, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s: Cannot open \"%s\"\n", __FILE__, filepath);
		exit(1);
	}

	words result;
	int i = 0, j = 0, k = 0;
	char c;
	char buffer[BUFFER_SIZE];

	// Lit et stock la liste des mots dans un tableau
	while((c = tolower(fgetc(txt))) != EOF)
	{
		if (is_binary_mode) {
			if (c != '0' && c != '1' && c != '\n')
				continue;
		}

		if(c == '\n') {
			buffer[i] = '\0';

			result.values[j] = (char*)malloc(sizeof(char) * i+1);
			memcpy(result.values[j++], buffer, i+1);

			i = 0;
		} else {
			buffer[i++] = (char)c;
			k += 1;
		}
	}

	fclose(txt);

	result.n = j;
	return result;
}
