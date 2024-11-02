cmake_minimum_required( VERSION 3.24.0)

set(PROJECT_NAME Graveyard CACHE STRING "Project Name")
set(GRAVEYARD_VERSION "1.0" CACHE STRING "Project Version")

project( "Graveyard"
        LANGUAGES HIP CXX Fortran
        VERSION ${GRAVEYARD_VERSION} )

# Needed to find all of the components of a ROCm installation
set(CMAKE_PREFIX_PATH "${GRAVEYARD_ROCM_PATH}/lib/cmake" CACHE PATH "")

# the targets hip::host and hip::device are provided by find_package(hip)
find_package(hip REQUIRED CONFIG PATHS ${GRAVEYARD_ROCM_PATH})

#-----------------------------#

add_executable( runGraveyard
                main.cpp)

# Set the fortran compiler to be used as the linker
set_property(TARGET runGraveyard PROPERTY LINKER_LANGUAGE Fortran)
# We've specialized here to use the Cray Fortran compiler as the linker and need flags so that everything is able to link
# In principle we can set some or all of these flags to be compiler and GPU Vendor agnostic, but extra logic would be needed
# Some of these link options are needed for Fortran OpenMP Offload
target_link_options(runGraveyard PUBLIC "--rocm-path=${GRAVEYARD_ROCM_PATH};-haccel=${CMAKE_HIP_PLATFORM}_${CMAKE_HIP_ARCHITECTURES};--offload-arch=${CMAKE_HIP_ARCHITECTURES}")

set_source_files_properties(SOURCE main.cpp PROPERTIES LANGUAGE HIP)

set_target_properties(runGraveyard PROPERTIES LINK_FLAGS "${GRAVEYARD_OMP_LINK_FLAGS} ${GRAVEYARD_HIP_LINK_FLAGS}")

# add libpindus as a TPL dependency and find the Pindus Package
option(ENABLE_PINDUS "Enable Pindus Library" OFF)
if (ENABLE_PINDUS)
 find_package(pindus
  CONFIG
  PATHS ${PINDUS_PREFIX}
  REQUIRED)
  if(${pindus_FOUND})
    message(STATUS "Package pindus was found")
    add_compile_definitions("GRAVEYARD_ENABLE_PINDUS")
    target_link_libraries(runGraveyard pindus)
    message(STATUS "Pindus imported module path: ${PINDUS_MODULE_PATH}")
    target_include_directories(runGraveyard AFTER PUBLIC "${PINDUS_INSTALL_PREFIX}/include")
  endif()
endif()

# Enable Libionian as a TPL and find the Ionian Package 
option(ENABLE_IONIAN "Enable Ionian Library" OFF)
if(ENABLE_IONIAN)
  find_package(ionian
    CONFIG
    PATHS ${IONIAN_PREFIX}
    REQUIRED)
  if(${ionian_FOUND})
    message(STATUS "Package ionian was found")
    add_compile_definitions("GRAVEYARD_ENABLE_IONIAN")
    target_link_libraries(runGraveyard ionian)
    target_include_directories(runGraveyard AFTER PUBLIC "${IONIAN_INSTALL_PREFIX}/include")
    if(${IONIAN_USE_RDC})
      # set_target_properties(runGraveyard PROPERTIES LINK_FLAGS APPEND "-fgpu-rdc -O3") # this replaces
      # need to use APPEND_STRING since cmake adds a bunch of semi-colons that I can't get rid of
      # the space is needed in front
      # set_property(TARGET runGraveyard APPEND_STRING PROPERTY LINK_FLAGS " -fgpu-rdc")
      target_link_options(runGraveyard PUBLIC "-fgpu-rdc;--hip-link")
      target_compile_options(runGraveyard PUBLIC $<$<COMPILE_LANGUAGE:HIP>:-fgpu-rdc> )
    endif()
  endif()
endif()

install(TARGETS runGraveyard DESTINATION bin)
