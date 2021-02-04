COMPILER = gcc
COMPILER_FLAGS  = -Werror -pedantic-errors -Wall -Wextra -Wpedantic
ADDITIONAL_LIBS = ../listlib/listlib.c ../stringlib/stringlib.c
SOURCE_FILES    = ./*.c

EXECUTABLE_NAME = argparselib.exe

all: clean build run

clean:
	rm -f *.exe
	rm -f *.o

build: clean
	$(COMPILER) $(COMPILER_FLAGS) $(SOURCE_FILES) $(ADDITIONAL_LIBS) -o $(EXECUTABLE_NAME)

run: build
	./$(EXECUTABLE_NAME) -n DIMA
