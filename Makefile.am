# GNU Automake config
SUBDIRS = data gctpc/source src test test_validation
INCLUDES = $(default_includes)

.PHONY = run_unit run_accept

run_unit:
	cd test;$(MAKE) run_unit

run_accept:
	cd test;$(MAKE) run_accept
