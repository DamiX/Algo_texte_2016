#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "include/algorithm.h"
#include "include/cli.h"
#include "include/helper.h"

static inline void print_usage(const char* appName) {
	printf("Usage: %s fichier\n", appName);
}

typedef struct _socket {
	struct addrinfo* 	addr;
	int					fd;
} Socket;

Socket make_socket(const char port[]) {
	struct addrinfo* addr;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = -1, sock_fd = -1;
	if((status = getaddrinfo("localhost", port, &hints, &addr)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	if((sock_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) == -1) {
		fprintf(stderr, "socket() error%d\n", errno);
		exit(-1);
	}

	Socket sock = { addr, sock_fd };
	return sock;
}

void close_socket(Socket sock) {
	close(sock.fd);
}

void server(char** argv)
{
	const char* fichier = argv[2];

	words w = read_words(fichier, false);

	deterministic_automata* Dx = aho_corasick(w.values, w.n, (int)(sizeof(w.values) / sizeof(char*)));

	const char port[] = "2016";
	Socket sock = make_socket(port);

	if(bind(sock.fd, sock.addr->ai_addr, sock.addr->ai_addrlen) == -1) {
		fprintf(stderr, "bind() error: %d", errno);
		exit(1);
	}

	if(listen(sock.fd, 5) == -1) {
		fprintf(stderr, "listen() error");
		exit(1);
	}

	// accept
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);

	int sock_fd_client = -1;
	if((sock_fd_client = accept(sock.fd, (struct sockaddr*)&their_addr, &addr_size)) == -1) {
		fprintf(stderr, "accept() error %d\n", errno);
		exit(1);
	}

	Socket sock_client = { NULL, sock_fd_client };

	char buffer[1024];
	while(recv(sock_client.fd, buffer, 1024, 0) != 0)
	{
		for(int i = 0; i < 1024; ++i)
		{
			printf("%02x ", buffer[i]);

			// Convert byte -> int before processing
			for(int j = 1<<7; j > 0; j=j>>1) {
				read_letter(Dx, (buffer[i] & j ? '1' : '0'));
				if(is_final_automata(Dx)) {
					close(sock_client.fd);
					close(sock.fd);
					printf("Unsupported instruction found\n");
					exit(1);
				}
			}
		}
		printf("\n");
	}

	close_socket(sock_client);
	close_socket(sock);
	free_deterministic_automata(Dx);
	free_words(w);
}

void client(char** argv)
{
	const char* bin = argv[2];

	// int s = -1;
	const char* port = "2016";

	Socket sock = make_socket(port);
	connect(sock.fd, sock.addr->ai_addr, sock.addr->ai_addrlen);

	// Read by chunks and send it though socket
	const int bf_size = 1024; //1Ko
	char buffer[bf_size];

	int fdesc;
	if((fdesc = open(bin, O_RDONLY)) == -1) {
		fprintf(stderr, "%s: Cannot open %s (%d)", __FILE__, bin, errno);
		exit(1);
	}

	while(read(fdesc, buffer, (size_t)bf_size) != 0)
		send(sock.fd, buffer, bf_size, 0);

	close(fdesc);
	close_socket(sock);
}

int main(int argc, char** argv)
{
	if(argc < 3) {
		print_usage(argv[0]);
		exit(1);
	}

	const bool is_server = (strcmp(argv[1], "server") == 0);
	if (is_server)
		server(argv);
	else
		client(argv);

	return 0;
}
