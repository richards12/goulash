FC ?= f77 
FORTRAN_FLAGS ?= -h omp


libpindus.a: pindus_mod.o
	ar rcs $@ $^

pindus_mod.o: pindus_mod.f90
	$(FC) $(FORTRAN_FLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf libpindus.a pindus_mod.o *.mod *.s pindus_mod*.acc.o
