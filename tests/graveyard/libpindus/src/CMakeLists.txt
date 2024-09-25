cmake_minimum_required( VERSION 3.24.0)

set(PINDUS_VERSION "1.0" CACHE STRING "Pindus Version")

project( "Pindus"
        LANGUAGES Fortran
        VERSION ${PINDUS_VERSION} )


set( PINDUS_FORTRAN_FLAGS "${GRAVEYARD_FORTRAN_OMPOL_FLAGS}" CACHE STRING "Flags passed to Fortran compiler, including for OpenMP")
set( PINDUS_MODULE_PATH "${PROJECT_BINARY_DIR}/mods" CACHE PATH "")

add_library( pindus STATIC)

set_source_files_properties(SOURCE pindus_mod.f90 PROPERTIES COMPILE_OPTIONS "${PINDUS_FORTRAN_FLAGS}")
set_target_properties(pindus PROPERTIES Fortran_MODULE_DIRECTORY "${PINDUS_MODULE_PATH}" )


message(STATUS "Fortran module dir: ${CMAKE_Fortran_MODULE_DIRECTORY}")
target_sources( pindus PRIVATE
              pindus_mod.f90)

install( TARGETS pindus DESTINATION lib EXPORT pindus-targets)
install( FILES pindus_mod.h DESTINATION include/pindus )
install( DIRECTORY ${PINDUS_MODULE_PATH} DESTINATION include/pindus FILES_MATCHING PATTERN "*.mod")

include(CMakePackageConfigHelpers) # needed for configure_package_config_file
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/pindus-config.cmake.in"
  "${PROJECT_BINARY_DIR}/pindus-config.cmake"
  PATH_VARS CMAKE_INSTALL_PREFIX
  INSTALL_DESTINATION lib/cmake/pindus)

install(FILES
  ${PROJECT_BINARY_DIR}/pindus-config.cmake
  DESTINATION lib/cmake/pindus)


write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/pindus-config-version.cmake
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${PROJECT_BINARY_DIR}/pindus-config-version.cmake"
  DESTINATION lib/cmake/pindus)

install( EXPORT pindus-targets DESTINATION lib/cmake/pindus)
