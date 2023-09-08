cmake_minimum_required( VERSION 3.24.0)

set(PINDUS_VERSION "1.0" CACHE STRING "Pindus Version")

project( "Pindus"
        LANGUAGES Fortran
        VERSION ${PINDUS_VERSION} )


set( PINDUS_FORTRAN_FLAGS "" CACHE STRING "Flags passed to Fortran compiler, including for OpenMP")

add_library( pindus STATIC)

set_source_files_properties(SOURCE pindus_mod.f90 PROPERTIES COMPILE_OPTIONS "${PINDUS_FORTRAN_FLAGS}")

target_sources( pindus PRIVATE
              pindus_mod.f90)

install( TARGETS pindus DESTINATION lib EXPORT PindusTargets)
install( FILES pindus_mod.h DESTINATION include/pindus )
install( EXPORT PindusTargets DESTINATION lib/cmake)
