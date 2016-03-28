#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include "../lib/algo.h"

inline void print_usage(const char*);

int main(int argc, char** argv)
{
	if(argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}

	const char* fichier = argv[1];

	// Etape #1 : Lire et stocker la liste des mots dans un tableau
	FILE *txt = fopen(fichier, "r");
	if(txt == NULL) {
		fprintf(stderr, "%s : Fichier \"%s\" introuvable\n", __FILE__, fichier);
		exit(1);
	}

	int i = 0, j = 0, k = 0;
	char c;
	const int buf_size = 1024; //1Ko
	char buffer[buf_size];
	char* list_words[buf_size];

	while((c = tolower(fgetc(txt))) != EOF)
	{
		if(c == '\n') {
			buffer[i] = '\0';

			list_words[j] = (char*)malloc(sizeof(char) * i+1);
			memcpy(list_words[j++], buffer, i+1);

			i = 0;
		} else {
			buffer[i++] = (char)c;
			k++;
		}
	}

	//Libère la mémoire
	fclose(txt);

	//Creation de l'automate
	automate_det* Dx = aho_corasick(list_words, j, k);

	// Etape #2 : Init les parametres pour la connexion en socket
	int status = -1, soc_connexion = -1, soc_client = -1;

	struct addrinfo *res;
	struct addrinfo hints;
	const char* port = "2016";

	memset(&hints, 0, sizeof(hints)); //Set all bytes to 0

	//Init hints
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	//getaddrinfo ! :o What kind of dark magic is that ?
	if((status = getaddrinfo("localhost", port, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	if((soc_connexion = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		fprintf(stderr, "socker() error%d\n", errno);
		exit(-1);
	}
	
	if(bind(soc_connexion, res->ai_addr, res->ai_addrlen) == -1) {
		fprintf(stderr, "bind() error: %d", errno);
		exit(1);
	}

	if(listen(soc_connexion, 5) == -1) {
		fprintf(stderr, "listen() error");
		exit(1);
	}

	// accept
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);

	if((soc_client = accept(soc_connexion, (struct sockaddr*)&their_addr, &addr_size)) == -1) {
		fprintf(stderr, "accept() error %d\n", errno);
		exit(1);
	}

	while(recv(soc_client, buffer, buf_size, 0) != 0)
	{
		for(i = 0; i < buf_size; ++i)
		{
			printf("%02x ", buffer[i]);

			//Converti les octets en bits avant de lire dans l'automate
			for(j = 1<<7; j > 0; j=j>>1) {
				read_letter(Dx, (buffer[i] & j ? '1' : '0'));
				if(is_final(Dx)) {
					//Envoyer stop
					close(soc_client);
					printf("Bits frauduleux trouvée\n");
					exit(1);
				}
			}
		}
		printf("\n");
	}

	//Libérer la mémoire
	free_automate_det(Dx);
	for(i = 0; i < j; ++i)
		free(list_words[i]);
	close(soc_client);
	close(soc_connexion);

	return 0;
}

inline void print_usage(const char* appName) {
	printf("Usage: %s fichier\n", appName);
}
