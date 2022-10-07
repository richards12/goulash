FC=ftn
CC=CC

CXX_FLAGS=-O3
HIP_FLAGS=-I${ROCM_PATH}/include -fgpu-rdc --rocm-path=${ROCM_PATH} --offload-arch=gfx90a -x hip
FORTRAN_FLAGS=-h omp

LFLAGS=-fopenmp
HIP_LINK_FLAGS=-fopenmp -fgpu-rdc --hip-link --offload-arch=gfx90a --rocm-path=${ROCM_PATH} -L${ROCM_PATH}/lib -lamdhip64

runGhoul: main.o foo_mod.o suskevi.o
	$(CC) -o $@ $(HIP_LINK_FLAGS) $^

main.o: main.cpp
	$(CC) $(CXX_FLAGS) $(HIP_FLAGS) -o $@ -c $<

suskevi.o: suskevi.cpp
	$(CC) $(CXX_FLAGS) $(HIP_FLAGS) -o $@ -c $<

foo_mod.o: foo_mod.f90
	$(FC) $(FORTRAN_FLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o runGhoul *core FOO.mod foo_mod_1.acc.s
