#make sure to load a rocm module and craype-accel-amd-gfx90a
FC=ftn
CC=CC

LIB_PINDUS_PREFIX ?=./libpindus
LIB_H_PREFIX ?=./lib_h

CXX_FLAGS = -O3 -I$(LIB_PINDUS_PREFIX) -I$(LIB_H_PREFIX) -DENABLE_PINDUS -DENABLE_HLIB
HIP_FLAGS = -I${ROCM_PATH}/include -fgpu-rdc --rocm-path=${ROCM_PATH} --offload-arch=gfx90a -x hip
FORTRAN_FLAGS=-h omp

LFLAGS=-fopenmp
HIP_LINK_FLAGS=-fopenmp -fgpu-rdc --hip-link --offload-arch=gfx90a --rocm-path=${ROCM_PATH} -L${ROCM_PATH}/lib -lamdhip64

runGraveyard: main.o libpindus.a libhlib.a 
	$(CC) -o $@ $(HIP_LINK_FLAGS) $^

main.o: main.cpp
	$(CC) $(CXX_FLAGS) $(HIP_FLAGS) -o $@ -c $<

.PHONY: libpindus.a libhlib.a
libpindus.a: export FC:=$(FC)
libpindus.a: export FORTRAN_FLAGS:= $(FORTRAN_FLAGS)
libpindus.a:
	make -C libpindus
	cp libpindus/libpindus.a .

libhlib.a: export CC:=$(CC)
libhlib.a: export CXX_FLAGS:=$(CXX_FLAGS)
libhlib.a: export HIP_FLAGS:=$(HIP_FLAGS)
libhlib.a:
	make -C lib_h
	cp lib_h/libhlib.a .

.PHONY: clean
clean:
	rm -rf *.o runGraveyard *core FOO.mod foo_mod_1.acc.s libhlib.a libpindus.a
	make -C libpindus clean
	make -C lib_h clean
