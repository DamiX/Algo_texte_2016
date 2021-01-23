#include "include/cli.h"

const char* shell_red_color     = "\x1b[1;32m";
const char* shell_green_color   = "\x1b[1;32m";
const char* shell_reset_color   = "\x1b[0m";

cli_args parse_cli_args(int argc, char** argv, void(*print_usage)(const char*)) {
	if (argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	char** positional_args = (char**)calloc((size_t)argc, sizeof(char*));
	positional_args[0] = argv[argc-2];
	positional_args[1] = argv[argc-1];

	cli_args args = { false, false, false, false, positional_args };

	int o;
	while((o = getopt(argc, argv, "bnli")) != -1)
	{
		switch(o)
		{
			case 'b':
				args.bFlag = true;
			break;

			case 'n':
				args.nFlag = true;
			break;

			case 'l':
				args.lFlag = true;
			break;

			case 'i':
				args.iFlag = true;
			break;

			// case '?':
			// 	print_usage(argv[0]);
			// 	exit(0);
			// break;

			default:
				exit(1);
		}
	}

	return args;
}
