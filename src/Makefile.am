# Autoconf/automake file

lib_LIBRARIES = libvxapi_cvmhvbn.a libcvmhvbn.a 
bin_PROGRAMS = vx_lite_cvmhvbn vx_cvmhvbn
include_HEADERS = vx_sub_cvmhvbn.h cvmhvbn.h
 
# General compiler/linker flags
AM_CFLAGS = -Wall -O3 -std=c99 -D_LARGEFILE_SOURCE \
            -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -fPIC
AM_LDFLAGS = -L../gctpc/source -lgctpc -lm

# Dist sources
libcvmhvbn_a_SOURCES = vx_sub_cvmhvbn.c vx_io.c 
vx_lite_cvmhvbn_SOURCES = vx_lite_cvmhvbn.c
vx_cvmhvbn_SOURCES = cvmhvbn.c vx_cvmhvbn.c

TARGETS = vx_lite_cvmhvbn vx_cvmhvbn libvxapi_cvmhvbn.a libcvmhvbn.a libcvmhvbn.so

all: $(TARGETS)

install:
	mkdir -p ${prefix}
	mkdir -p ${prefix}/lib
	mkdir -p ${prefix}/include
	cp libcvmhvbn.so ${prefix}/lib
	cp libcvmhvbn.a ${prefix}/lib
	cp cvmhvbn.h ${prefix}/include

cvmhvbn.h: ../cvmhbn/src/cvmhbn.h vx_sub_cvmhvbn.h
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/cvmhbn.h > cvmhvbn.h

cvmhvbn.c: ../cvmhbn/src/cvmhbn.c cvmhvbn.h
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/cvmhbn.c > cvmhvbn.c

vx_lite_cvmhvbn.c: ../cvmhbn/src/vx_lite_cvmhbn.c vx_sub_cvmhvbn.h
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/vx_lite_cvmhbn.c > vx_lite_cvmhvbn.c

vx_cvmhvbn.c: ../cvmhbn/src/vx_cvmhbn.c cvmhvbn.h
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/vx_cvmhbn.c > vx_cvmhvbn.c

vx_sub_cvmhvbn.c: ../cvmhbn/src/vx_sub_cvmhbn.c vx_sub_cvmhvbn.h
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/vx_sub_cvmhbn.c > vx_sub_cvmhvbn.c

vx_sub_cvmhvbn.h: ../cvmhbn/src/vx_sub_cvmhbn.h 
	sed -f ../cvmhbn/setup/cvmhvbn_sed_cmd ../cvmhbn/src/vx_sub_cvmhbn.h > vx_sub_cvmhvbn.h

libcvmhvbn.a: vx_sub_cvmhvbn.o vx_io.o utils.o cvmhvbn_static.o 
	$(AR) rcs $@ $^

cvmhvbn_static.o: cvmhvbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

libcvmhvbn.so: vx_sub_cvmhvbn.o vx_io.o utils.o cvmhvbn.o
	$(CC) -shared $(AM_CFLAGS) -o libcvmhvbn.so $^ $(AM_LDFLAGS)

libvxapi_cvmhvbn.a: vx_sub_cvmhvbn.o vx_io.o utils.o *.h
	$(AR) rcs $@ $^

cvmhvbn.o: cvmhvbn.c
	$(CC) -fPIC -DDYNAMIC_LIBRARY -o $@ -c $^ $(AM_CFLAGS)

vx_lite_cvmhvbn.o : vx_lite_cvmhvbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

vx_lite_cvmhvbn : vx_lite_cvmhvbn.o libvxapi_cvmhvbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

vx_cvmhvbn.o : vx_cvmhvbn.c
	$(CC) -o $@ -c $^ $(AM_CFLAGS)

vx_cvmhvbn : vx_cvmhvbn.o libcvmhvbn.a
	$(CC) -o $@ $^ $(AM_LDFLAGS)

clean:
	rm -rf $(TARGETS)
	rm -rf *.o 
	rm -rf cvmhvbn.h cvmhvbn.c vx_lite_cvmhvbn.c vx_cvmhvbn.c vx_sub_cvmhvbn.c vx_sub_cvmhvbn.h
