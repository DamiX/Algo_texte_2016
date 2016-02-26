#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int status = 0;

	struct addrinfo *res;
	struct addrinfo hints;
	const char* port = "2016";

	memset(&hints, 0, sizeof(hints)); //Set all bytes to 0

	//Init hints
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo("localhost", port, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int s;
	if((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		fprintf(stderr, "socker() error");
		exit(-1);
	}

	connect(s, res->ai_addr, res->ai_addrlen);

	//Read the file by chunks and send it though socket

	const char* file = "myTest";
	const int bf_size = 1024; //1Ko
	char buffer[bf_size];

	int fdesc;
	if((fdesc = open(file, O_RDONLY)) == -1) {
		fprintf(stderr, "%s: Impossible d'ouvrir l'executable %s. Error : %d", __FILE__,
		file, errno);
		exit(1);
	}

	while(read(fdesc, buffer, (size_t)bf_size) != 0)
	{
		send(s, buffer, bf_size, 0);
	}

	close(fdesc);
	close(s);

	return 0;
}
