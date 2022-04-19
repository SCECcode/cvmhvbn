# Autoconf/automake file

bin_PROGRAMS = cvmhvbn_api_validate cvmhvbn_vxlite_validate

# General compiler/linker flags
AM_CFLAGS = -DDYNAMIC_LIBRARY -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
        -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -I../src
AM_LDFLAGS = -L../src -lcvmhvbn -L../gctpc/source -lgctpc -lm -ldl

# Dist sources
cvmhvbn_api_validate_SOURCES = cvmhvbn_api_validate.c
cvmhvbn_vxlite_validate_SOURCES = cvmhvbn_vxlite_validate.c

.PHONY = run_validate run_validate_with_ucvm

all: $(bin_PROGRAMS)

############################################
# Executables
############################################

cvmhvbn_vxlite_validate : cvmhvbn_vxlite_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

cvmhvbn_api_validate : cvmhvbn_api_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

if UCVM
cvmhvbn_ucvm_validate.o : cvmhvbn_ucvm_validate.c 
	$(CC) -c -o $@ $^ $(AM_CFLAGS) $(CFLAGS)

cvmhvbn_ucvm_validate : cvmhvbn_ucvm_validate.o 
	$(CC) -o $@ $^  $(LDFLAGS) $(AM_LDFLAGS)
endif

############################################
# Special build targets
############################################

run_validate: cvmhvbn_vxlite_validate cvmhvbn_api_validate
	./run_vxlite_validate.sh
	./run_api_validate.sh

run_validate_with_ucvm: cvmhvbn_ucvm_validate
	./run_ucvm_validate.sh

clean:
	rm -rf *~ *.o $(bin_PROGRAMS) cvmhvbn_ucvm_validate *.txt

