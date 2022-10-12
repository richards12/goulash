target_sources( flib PRIVATE
              foo_mod.f90)

# set_source_files_properties(SOURCE foo_mod.f90 PROPERTY COMPILE_FLAGS ${GRAVEYARD_OMP_FORTRAN_FLAGS}) # this doesn't work, these properties cannot be set in add_subdirectory(...)
