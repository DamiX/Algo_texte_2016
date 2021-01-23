#ifndef __H_CLI__
#define __H_CLI__

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

extern const char* shell_red_color;
extern const char* shell_green_color;
extern const char* shell_reset_color;

typedef struct _cli_args {
	bool	nFlag;
	bool	lFlag;
	bool	iFlag;
	bool	bFlag;
	char**	positional;
} cli_args;

cli_args parse_cli_args(int argc, char** argv, void(*print_usage)(const char*));

#endif
