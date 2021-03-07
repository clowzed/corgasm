libs = $(subst ./, ,$(wildcard ./*lib))
$(info Found libs:)
$(foreach lib, $(libs), $(info ................ $(lib)))

define \n


endef

all: clean

clean: clean_targets clean_objects

clean_targets:
	$(info $(\n)Cleaning from executable targets)
	$(foreach lib, $(libs), $(\n)rm -f ./$(lib)/**/*.exe)

clean_objects:
	$(info $(\n)Cleaning from object files)
	$(foreach lib, $(libs), $(\n)rm -f ./$(lib)/**/*.o)


merge:
	git add .
	git commit -am "commit before merge"
	git checkout dev
	git merge $(libs)
