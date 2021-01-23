CFLAGS = -Wall
CC = gcc
EXEC = sgrep acgrep acbgrep server client

all: $(EXEC) clean

sgrep: automate.o file.o algo.o src/simple_grep_source.c
	$(CC) $^ -o $@

acgrep: automate.o file.o algo.o src/acgrep_source.c
	$(CC) $^ -o $@

acbgrep: automate.o file.o algo.o src/acbgrep_source.c
	$(CC) $^ -o $@

server: automate.o file.o algo.o src/server.c
	$(CC) $^ -o $@

client: src/client.c
	$(CC) $^ -o $@

algo.o: lib/automate_det.h lib/file.h lib/algo.c
	$(CC) -c lib/algo.c $(CFLAGS) -o algo.o

automate.o: lib/automate_det.c
	$(CC) -c lib/automate_det.c $(CFLAGS) -o automate.o

file.o: lib/file.c
	$(CC) -c lib/file.c $(CFLAGS) -o file.o

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
