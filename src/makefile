CC=clang
remove=rm
DEPENDENCIES=
DEBFLGS=-g -Wall  -pedantic 
OPFLGS=-O3  
PREFIX ?= /usr/local



SRC_DIR=.
INC_DIRS = $(shell find $(SRC_DIR) -type d)
INCLUDE_FLAGS = $(addprefix -I, $(INC_DIRS))


SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
FILES= $(SRC_FILES) $(INCLUDE_FLAGS)

cerrt_testing :
	$(CC) $(DEPENDENCIES) $(FILES) -o $@

cerrt_testing_opt :
	$(CC) $(OPFLGS) $(DEPENDENCIES) $(FILES) -o $@
cerrt_testing_dbg :
	$(CC)  $(DEBFLGS) $(DEPENDENCIES) $(FILES) -o $@ 
clean :
	$(remove) cerrt_testing*
