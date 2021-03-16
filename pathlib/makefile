executable_target = $(notdir $(CURDIR))
unit_target       = unit_$(executable_target)


uppercase_exetuable_target =  $(shell echo $(executable_target) | tr a-z A-Z)



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
	object_compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99 -DLIB_TEST_$(uppercase_exetuable_target)
else
	object_compiler_flags = -g3 -O3 -Wall -Wextra -Werror -pedantic -std=c99 -DLIB_BUILD_$(uppercase_exetuable_target)
endif

$(info object files compiling flags:)
$(info .......... $(object_compiler_flags))
$(info )



.phony: clean, building, run, test

all: clean building run

building: $(executable_target)


dependencies = listlib stringlib


ifneq ($(dependencies), )
$(info dependencies:)
$(foreach dependency, $(dependencies) , $(info .......... $(dependency)))
$(info  )
endif

dependencies_c_files = $(foreach dependency, $(dependencies), $(wildcard ../$(dependency)/*.c))

ifneq ($(dependencies_c_files), )
$(info dependencies source files:)
$(foreach dependency_source_file, $(dependencies_c_files), $(info .......... $(dependency_source_file)))
$(info  )
endif


lib_c_files_ = $(wildcard *.c)
ifneq ($(lib_c_files), )
$(info library source files:)
$(foreach lib_c_file, $(lib_c_files_), $(info .......... $(lib_c_file)))
$(info )
endif


lib_c_files = $(lib_c_files_) $(dependencies_c_files)

ifneq ($(lib_c_files), )
$(info Sourse files that will be compiled:)
$(foreach lib_c_file, $(lib_c_files), $(info .......... $(lib_c_file)))
$(info  )
endif

unit_c_files = $(wildcard ./unit/*.c) $(lib_c_files)

ifneq ($(wildcard ./unit/*.c), )
$(info library unit tests source files:)
$(foreach unit_c_file, $(unit_c_files), $(info .......... $(unit_c_file)))
$(info  )
endif

object_files = $(addsuffix .o, $(basename $(lib_c_files)))

ifneq ($(object_files), )
$(info library object files that will be compiled:)
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


$(executable_target) : $(object_files)
	$(info )
	$(info building target: $(target))
	$(compiler) $(build_compiler_flags) $(object_files) -o $@

run:
	$(info )
	$(info running target: $(executable_target))
	./$(executable_target)
	$(info )

clean:
	$(info )
	$(info cleaning from old files...)
	rm -f $(object_files)
	rm -f $(subst $(object_files),,$(unit_object_files))
	rm -f $(unit_target)
	rm -f $(target)
	rm -f $(unit_target)
	$(info )

test: clean $(unit_target) run_unit


run_unit:
	$(info )
	$(info running unit tests: $(unit_target))
	./$(unit_target)
	$(info )

$(unit_target) : $(unit_object_files)
	$(info )
	$(info building unit target: $(unit_target))
	$(compiler) $(unit_test_build_compiler_flags) $(unit_object_files) -o $@
