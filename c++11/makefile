CC=clang++
CFLAGS=-std=c++11 -m64 -O3 -Wall

# Setup directories
MAKE_DIR=$(CURDIR)
SRC_DIR=src/
HEADER_DIR=include/

# Executable
EXEC=sgrep

all: setup $(EXEC) clean

setup:
	mkdir -p bin
	mkdir -p .build
	cp -R $(SRC_DIR) .build/
	cp -R $(HEADER_DIR) .build/

# Link executable
sgrep:
	cd .build && \
	$(CC) $(CFLAGS) $^ sgrep.cpp -o $@ && \
	mv $@ "$(MAKE_DIR)/bin/"

# Compile cpp
# shellColors.o:
# 	$(CC) $(CFLAGS) -c .build/shellColors.cpp -o .build/$@

clean:
	rm -rf .build

mrproper: clean
	rm -rf bin
