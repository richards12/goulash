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

#if(ENABLE_PINDUS)
#  set(GRAVEYARD_OMP_FORTRAN_FLAGS "-homp" CACHE STRING "Flags needed for fortran OpenMP compiling")
#  set(GRAVEYARD_OMP_CXX_FLAGS "-fopenmp" CACHE STRING "Flags needed for C++ OpenMP  compiling")
#  set(GRAVEYARD_OMP_LINK_FLAGS "-fopenmp" CACHE STRING "Options needed to link in OpenMP")
#endif()

#set(HIP_COMPILE_FLAGS "-I${GRAVEYARD_ROCM_PATH}/include -fgpu-rdc --rocm-path=${GRAVEYARD_ROCM_PATH} --offload-arch=gfx90a -x hip" CACHE STRING "Compiler options used to compile HIP code")
#set(HIP_LINK_FLAGS "-fgpu-rdc --hip-link --rocm-path=${GRAVEYARD_ROCM_PATH} -L${GRAVEYARD_ROCM_PATH}/lib -lamdhip64" CACHE STRING "Flags needed to link HIP code with RDC")

#-----------------------------#

add_executable( runGraveyard
                main.cpp)

#set_source_files_properties(SOURCE main.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})
set_source_files_properties(SOURCE main.cpp PROPERTIES LANGUAGE HIP)

set_target_properties(runGraveyard PROPERTIES LINK_FLAGS "${GRAVEYARD_OMP_LINK_FLAGS} ${GRAVEYARD_HIP_LINK_FLAGS}")

# add flib as a link dependency
option(ENABLE_PINDUS "Enable Pindus Library" OFF)
if (ENABLE_PINDUS)
 find_package(pindus
  CONFIG
  PATHS ${PINDUS_PREFIX}
  REQUIRED)
  if(${pindus_FOUND})
    message(STATUS "Package pindus was found")
    add_compile_definitions("GRAVEYARD_ENABLE_PINDUS")
    target_link_libraries(runGraveyard ionian)
    message(STATUS "Pindus imported module path: ${PINDUS_MODULE_PATH}")
    target_include_directories(runGraveyard AFTER PUBLIC "${PINDUS_INSTALL_PREFIX}/include")
  endif()
endif()

# Enable Libionian and find the package
option(ENABLE_IONIAN "Enable Ionian Library" ON)
if(ENABLE_IONIAN)
  find_package(ionian
    CONFIG
    PATHS ${IONIAN_PREFIX}
    REQUIRED)
  if(${ionian_FOUND})
    message(STATUS "Package ionian was found")
    add_compile_definitions("GRAVEYARD_ENABLE_IONIAN")
#    set_target_properties(ionian PROPERTIES IMPORTED_GLOBAL TRUE)
    target_link_libraries(runGraveyard ionian)
    target_include_directories(runGraveyard AFTER PUBLIC "${IONIAN_INSTALL_PREFIX}/include")
    if(${IONIAN_USE_RDC})
#      set_target_properties(runGraveyard PROPERTIES LINK_FLAGS APPEND "-fgpu-rdc -O3") # this replaces
      # need to use APPEND_STRING since cmake adds a bunch of semi-colons that I can't get rid of
      # the space is needed in front
#      set_property(TARGET runGraveyard APPEND_STRING PROPERTY LINK_FLAGS " -fgpu-rdc")
      target_link_options(runGraveyard PUBLIC "-fgpu-rdc")
      target_compile_options(runGraveyard PUBLIC $<$<COMPILE_LANGUAGE:HIP>:-fgpu-rdc> )
    endif()
  endif()
endif()
