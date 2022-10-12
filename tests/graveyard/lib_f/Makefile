FC ?= f77 
FORTRAN_FLAGS ?= -h omp


libflib.a: foo_mod.o
	ar rcs $@ $^

foo_mod.o: foo_mod.f90
	$(FC) $(FORTRAN_FLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf libflib.a foo_mod.o *.mod *.s foo_mod*.acc.o
