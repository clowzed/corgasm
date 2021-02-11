target       = libexecutable
unit_target  = unit_libexecutable

compiler = gcc
$(info setted compiler: $(compiler))

build_compiler_flags           = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99
$(info compiler build flags:)
$(info .......... $(build_compiler_flags))
$(info )


unit_test_build_compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99
$(info unit test build flags:)
$(info .......... $(unit_test_build_compiler_flags))
$(info )

ifeq ($(MAKECMDGOALS), test)
	object_compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99 -DLIB_TEST
else
	object_compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99 -DLIB_BUILD
endif

$(info object files compiling flags:)
$(info .......... $(object_compiler_flags))
$(info )



.phony: clean, building, run, test

all: clean building run test

building: $(target)


dependencies = listlib stringlib

ifneq ($(dependencies), )
$(info dependencies: $(dependencies))
$(info  )
endif

dependencies_c_files = $(foreach dependency, $(dependencies), $(wildcard ../$(dependency)/*.c))

ifneq ($(dependencies_c_files), )
$(info dependencies source files: $(dependencies_c_files))
$(info  )
endif


lib_c_files = $(wildcard *.c) $(dependencies_c_files)

ifneq ($(lib_c_files), )
$(info library source files:)
$(foreach lib_c_file, $(lib_c_files), $(info .......... $(lib_c_files)))
$(info  )
endif

unit_c_files = $(dependencies_c_files) $(wildcard ./unit/*.c) $(lib_c_files)

ifneq ($(wildcard ./unit/*.c), )
$(info library unit tests source files:)
$(foreach unit_c_file, $(unit_c_files), $(info .......... $(unit_c_file)))
$(info  )
endif



object_files = $(addsuffix .o, $(basename $(lib_c_files)))

ifneq ($(object_files), )
$(info library object files that will be compiled: $(object_files))
$(foreach object_file, $(object_files), $(info .......... $(object_file)))
$(info )
endif


unit_object_files = $(addsuffix .o, $(basename $(unit_c_files)))
ifneq ($(unit_object_files), )
$(info unit object files that will be compiled:)
$(foreach unit_object_file, $(unit_object_files), $(info .......... $(unit_object_file)))
$(info  )
endif



%.o: %.c
	$(info )
	$(info Compiling object file: $@)
	$(compiler) $(object_compiler_flags) -c $< -o $@


$(target) : $(object_files)
	$(info )
	$(info building target: $(target))
	$(compiler) $(build_compiler_flags) $(object_files) -o $@

run:
	$(info )
	$(info running target: $(target))
	./$(target)
	$(info)

clean:
	$(info )
	$(info cleaning from old files...)
	rm -f $(object_files)
	rm -f $(unit_object_files)
	rm -f $(unit_target)
	rm -f $(target)
	rm -f $(unit_target)
	$(info)

test: clean $(unit_target) run_unit


run_unit:
	$(info )
	$(info running unit tests: $(unit_target))
	./$(unit_target)

$(unit_target) : $(unit_object_files)
	$(info )
	$(info building unit target: $(unit_target))
	$(compiler) $(unit_test_build_compiler_flags) $(unit_object_files) -o $@
