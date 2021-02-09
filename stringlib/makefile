target = libexecutable

compiler = gcc

compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99


.phony: clean, building, run

all: building run

building: clean $(target)

libs_c_files =
libs_h_files =

c_files      = $(wildcard *.c) $(libs_c_files)
header_files = $(wildcard *.h) $(libs_h_files)
object_files = $(addsuffix .o, $(basename $(c_files)))

$(info found .c files: $(c_files))
$(info found .h files: $(header_files))

%.o: %.c $(header_files)
	$(info building object_files: $(object_files))
	$(compiler) $(compiler_flags) -c $< -o $@

.precious: $(target) $(object_files)

$(target) : $(object_files)
	$(info building target: $(target))
	$(compiler) $(compiler_flags) $(object_files) -o $@

run:
	$(info running target: $(target))
	./$(target)

clean:
	$(info cleaning from old files...)
	rm -f $(object_files)
	rm -f $(target)
