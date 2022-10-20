cmake_minimum_required( VERSION 3.24.0)

set(PROJECT_NAME Graveyard CACHE STRING "ProjectName")
set(GRAVEYARD_VERSION "1.0" CACHE STRING "Graveyard Version")


#set(MPI_C_COMPILER  "cc" CACHE PATH "")
#set(MPI_CXX_COMPILER  "CC" CACHE PATH "")
#set(MPI_Fortran_COMPILER "ftn" CACHE PATH "")

set(CMAKE_C_COMPILER "/opt/cray/pe/craype/2.7.17/bin/cc" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "/opt/cray/pe/craype/2.7.17/bin/CC" CACHE FILEPATH "")
set(CMAKE_Fortran_COMPILER "/opt/cray/pe/craype/2.7.17/bin/ftn" CACHE FILEPATH "")

#set(CMAKE_HIP_COMPILER "/g/g12/makrides/programs/ghoulash/build_2/amdcompiler" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
set(GRAVEYARD_ROCM_PATH "/opt/rocm-5.2.3" CACHE PATH "ROCm Path")
#set(CMAKE_HIP_COMPILER "${GRAVEYARD_ROCM_PATH}/bin/hipcc" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
#set(CMAKE_HIP_COMPILER "${GRAVEYARD_ROCM_PATH}/bin/amdclang++" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND

project( "Graveyard"
        LANGUAGES CXX Fortran
        VERSION ${GRAVEYARD_VERSION} )

#list(APPEND CMAKE_PREFIX_PATH ${GRAVEYARD_ROCM_PATH})
# the targets hip::host and hip::device are provided by find_package(hip)
#find_package(hip REQUIRED CONFIG PATHS ${HIP_PATH} ${ROCM_PATH})

message(STATUS "ROCM path:        ${ROCM_PATH}")
message(STATUS "HIP version:      ${hip_VERSION}")
set(HIP_ARCHITECTURES gfx90a CACHE STRING "")


set(CMAKE_Fortran_FLAGS "-ffree" CACHE PATH "")

set(CMAKE_Fortran_FLAGS_RELEASE "-O3 -g -ef" CACHE PATH "")
set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO "-O3 -g -ef" CACHE PATH "")
set(CMAKE_Fortran_FLAGS_DEBUG "-O0 -g -ef" CACHE PATH "")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g" CACHE PATH "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g" CACHE PATH "")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE PATH "")

set(GRAVEYARD_OMP_FORTRAN_FLAGS "-homp" CACHE STRING "Flags needed for fortran OpenMP compiling")
set(GRAVEYARD_OMP_CXX_FLAGS "-fopenmp" CACHE STRING "Flags needed for C++ OpenMP  compiling")

set(GRAVEYARD_OMP_LINK_FLAGS "-fopenmp" CACHE STRING "Options needed to link in OpenMP")

set(HIP_COMPILE_FLAGS "-I${GRAVEYARD_ROCM_PATH}/include -fgpu-rdc --rocm-path=${GRAVEYARD_ROCM_PATH} --offload-arch=gfx90a -x hip" CACHE STRING "Compiler options used to compile HIP code")
set(HIP_LINK_FLAGS "-fgpu-rdc --hip-link --offload-arch=gfx90a --rocm-path=${GRAVEYARD_ROCM_PATH} -L${GRAVEYARD_ROCM_PATH}/lib -lamdhip64" CACHE STRING "Flags needed to link HIP code with RDC")

#-----------------------------#

add_executable( runGraveyard
                main.cpp)

set_source_files_properties(SOURCE main.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})

set_target_properties(runGraveyard PROPERTIES LINK_FLAGS "${GRAVEYARD_OMP_LINK_FLAGS} ${HIP_LINK_FLAGS}")

# add flib as a link dependency
option(ENABLE_LIBPINDUS "Enable and build the FLIB library" ON)
if (ENABLE_LIBPINDUS)
  add_compile_definitions( "ENABLE_LIBPINDUS" )
  add_library( pindus STATIC)
  add_subdirectory(libpindus)
  set_source_files_properties(SOURCE libpindus/foo_mod.f90 PROPERTY COMPILE_FLAGS ${GRAVEYARD_OMP_FORTRAN_FLAGS})
  target_include_directories( runGraveyard PRIVATE include libpindus)
  target_link_libraries( runGraveyard
                         pindus)
endif()

# add hlib as a link dependency
option(ENABLE_HLIB "Enable and build the HLIB library" ON)
if (ENABLE_HLIB)
  add_compile_definitions( "ENABLE_HLIB" )
  add_library( hlib STATIC)
  add_subdirectory(lib_h)
  #set_source_files_properties(SOURCE lib_h/suskevi.cpp PROPERTIES LANGUAGE HIP)
  set_source_files_properties(SOURCE lib_h/suskevi.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})
  target_include_directories( runGraveyard PRIVATE include lib_h)
  target_link_libraries( runGraveyard
                         hlib)
endif()
