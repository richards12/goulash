cmake_minimum_required( VERSION 3.24.0)

set(IONIAN_VERSION "1.0" CACHE STRING "Ionian Version")

project( "Ionian"
        LANGUAGES CXX HIP
        VERSION ${IONIAN_VERSION} )

find_package(HIP)

set( IONIAN_HIP_FLAGS "" CACHE STRING "Flags passed to HIP compiler")
set( IONIAN_USE_RDC "TRUE" CACHE BOOL "Ionian use RDC" )

add_library( ionian STATIC)

target_compile_options(ionian PUBLIC
                       $<$<COMPILE_LANGUAGE:HIP>:-fgpu-rdc> )
target_link_options(ionian PUBLIC
                       "-fgpu-rdc" )

set_source_files_properties(SOURCE ionian.cpp PROPERTIES LANGUAGE HIP)
set_source_files_properties(SOURCE ionian.cpp PROPERTIES COMPILE_OPTIONS "${IONIAN_HIP_FLAGS}")

target_sources( ionian PRIVATE
              ionian.cpp)

install( TARGETS ionian DESTINATION lib EXPORT ionian-targets)
install( FILES ionian.h DESTINATION include/ionian)

#include(CMakeDependentOption)
include(CMakePackageConfigHelpers) # needed for configure_package_config_file 
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/ionian-config.cmake.in"
  "${PROJECT_BINARY_DIR}/ionian-config.cmake"
  PATH_VARS CMAKE_INSTALL_PREFIX
  INSTALL_DESTINATION lib/cmake/ionian)

install(FILES
  ${PROJECT_BINARY_DIR}/ionian-config.cmake
  DESTINATION lib/cmake/ionian)

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/ionian-config-version.cmake
  COMPATIBILITY SameMajorVersion)

install(FILES 
  "${PROJECT_BINARY_DIR}/ionian-config-version.cmake"
  DESTINATION lib/cmake/ionian)

install( EXPORT ionian-targets DESTINATION lib/cmake/ionian)

