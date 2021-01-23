# Directories
SRC_DIR		:= src
BUILD_DIR	:= build
OBJ_DIR		:= $(BUILD_DIR)/obj
BINARY_DIR	:= $(BUILD_DIR)/bin

# SRC := $(wildcard $(SRC_DIR)/*.c)
SRC := $(filter-out src/simple_grep.c src/acgrep.c src/client.c src/server.c, $(wildcard $(SRC_DIR)/*.c))
OBJ	:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Compiler & linker args
CC			:= gcc
CPPFLAGS	:= -Iinclude -MMD -MP
CFLAGS		:= -Wall -g
LDFLAGS 	:=
LDLIBS		:=

EXEC := sgrep acgrep server client

.PHONY: all

all: setup $(EXEC)

rebuild: clean all

setup:
	mkdir -p $(BINARY_DIR)
	mkdir -p $(OBJ_DIR)

sgrep: $(OBJ) $(OBJ_DIR)/simple_grep.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_DIR)/$@

acgrep: $(OBJ) $(OBJ_DIR)/acgrep.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_DIR)/$@

server: $(OBJ) $(OBJ_DIR)/server.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_DIR)/$@

client: $(OBJ) $(OBJ_DIR)/server.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(OBJ:.o=.d)
