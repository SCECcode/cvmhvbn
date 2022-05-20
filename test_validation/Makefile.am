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
cvmhvbn_vxlite_validate.c : ../cvmhbn/test_validation/cvmhbn_vxlite_validate.c
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_vxlite_validate.c > cvmhvbn_vxlite_validate.c

cvmhvbn_api_validate.c : ../cvmhbn/test_validation/cvmhbn_api_validate.c
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_api_validate.c > cvmhvbn_api_validate.c

cvmhvbn_vxlite_validate : cvmhvbn_vxlite_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

cvmhvbn_api_validate : cvmhvbn_api_validate.o 
	$(CC) -o $@ $^ $(AM_LDFLAGS)

### only run after ucvm is built and installed
if UCVM

CVMH_LDFLAGS = -L$(UCVM_INSTALL_PATH)/model/cvmh/lib -lvxapi -L$(UCVM_INSTALL_PATH)/model/cvmh/lib -lgeo 
CENCAL_LDFLAGS = -L$(UCVM_INSTALL_PATH)/model/cencal/lib -lcencalvm -lstdc++

CVMH_EXISTS := $(shell [[ -e $(UCVM_INSTALL_PATH)/model/cvmh/include/vx_sub.h ]] && echo 1 || echo 0 )
CENCAL_EXISTS := $(shell [[ -e $(UCVM_INSTALL_PATH)/model/cencal/include/cencalvm/query/cvmerror.h ]] && echo 1 || echo 0 )

cvmhvbn_ucvm_validate.c : ../cvmhbn/test_validation/cvmhbn_ucvm_validate.c
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_validate.c > cvmhvbn_ucvm_validate.c

cvmhvbn_ucvm_rerun.c : ../cvmhbn/test_validation/cvmhbn_ucvm_rerun.c
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_rerun.c > cvmhvbn_ucvm_rerun.c

cvmhvbn_ucvm_retry.c : ../cvmhbn/test_validation/cvmhbn_ucvm_retry.c
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/test_validation/cvmhbn_ucvm_retry.c > cvmhvbn_ucvm_retry.c

cvmhvbn_ucvm_validate.o : cvmhvbn_ucvm_validate.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhvbn_ucvm_validate : cvmhvbn_ucvm_validate.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

cvmhvbn_ucvm_rerun.o : cvmhvbn_ucvm_rerun.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhvbn_ucvm_rerun : cvmhvbn_ucvm_rerun.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

cvmhvbn_ucvm_retry.o : cvmhvbn_ucvm_retry.c
	$(CC) -c -o $@ $^ $(UCVM_CFLAGS) $(AM_CFLAGS) $(CFLAGS)

cvmhvbn_ucvm_retry : cvmhvbn_ucvm_retry.o 
	$(CC) -o $@ $^ $(UCVM_LDFLAGS) $(AM_LDFLAGS)

endif

############################################
# Special build targets
############################################

run_validate: cvmhvbn_api_validate cvmhvbn_vxlite_validate
	./run_vxlite_validate.sh
	./run_api_validate.sh

run_validate_with_ucvm: cvmhvbn_ucvm_validate cvmhvbn_ucvm_rerun cvmhvbn_ucvm_retry
	./run_ucvm_validate.sh

clean:
	rm -rf *~ *.o $(bin_PROGRAMS) cvmhvbn_ucvm_validate cvmhvbn_ucvm_rerun cvmhvbn_ucvm_retry *.txt
	rm -rf cvmhvbn_ucvm_rerun.c cvmhvbn_ucvm_validate.c cvmhvbn_vxlite_validate.c cvmhvbn_api_validate.c cvmhvbn_ucvm_retry.c


