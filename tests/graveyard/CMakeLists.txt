cmake_minimum_required( VERSION 3.24.0)

set(PROJECT_NAME GhoulLash CACHE STRING "ProjectName")
set(GHOUL_LASH_VERSION "1.0" CACHE STRING "GhoulLash Version")


#set(MPI_C_COMPILER  "cc" CACHE PATH "")
#set(MPI_CXX_COMPILER  "CC" CACHE PATH "")
#set(MPI_Fortran_COMPILER "ftn" CACHE PATH "")

set(CMAKE_C_COMPILER "/opt/cray/pe/craype/2.7.17/bin/cc" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "/opt/cray/pe/craype/2.7.17/bin/CC" CACHE FILEPATH "")
set(CMAKE_Fortran_COMPILER "/opt/cray/pe/craype/2.7.17/bin/ftn" CACHE FILEPATH "")

#set(CMAKE_HIP_COMPILER "/g/g12/makrides/programs/ghoulash/build_2/amdcompiler" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
set(GHOUL_ROCM_PATH "/opt/rocm-5.2.3" CACHE PATH "ROCm Path")
#set(CMAKE_HIP_COMPILER "${GHOUL_ROCM_PATH}/bin/hipcc" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
set(CMAKE_HIP_COMPILER "/g/g12/makrides/programs/ghoulash/amdwrapper" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
project( "GhoulLash"
        LANGUAGES CXX Fortran HIP
        VERSION ${GHOUL_LASH_VERSION} )

#set(CMAKE_HIP_COMPILER "${GHOUL_ROCM_PATH}/bin/amdclang++" CACHE FILEPATH "") # CMAKE doesn't like this, CLANGRT_BUILTINS-NOTFOUND
#list(APPEND CMAKE_PREFIX_PATH ${GHOUL_ROCM_PATH})
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

set(GHOUL_OMP_FORTRAN_FLAGS "-homp" CACHE STRING "Flags needed for fortran OpenMP compiling")
set(GHOUL_OMP_CXX_FLAGS "-fopenmp" CACHE STRING "Flags needed for C++ OpenMP  compiling")

set(GHOUL_OMP_LINK_FLAGS "-fopenmp" CACHE STRING "Options needed to link in OpenMP")

set(HIP_COMPILE_FLAGS "-I${GHOUL_ROCM_PATH}/include -fgpu-rdc --rocm-path=${GHOUL_ROCM_PATH} --offload-arch=gfx90a -x hip" CACHE STRING "Compiler options used to compile HIP code")
set(HIP_LINK_FLAGS "-fgpu-rdc --hip-link --offload-arch=gfx90a --rocm-path=${GHOUL_ROCM_PATH} -L${GHOUL_ROCM_PATH}/lib -lamdhip64" CACHE STRING "Flags needed to link HIP code with RDC")

#-----------------------------#

add_executable( runGhoul
                main.cpp
                foo_mod.f90
                suskevi.cpp)

set_source_files_properties(SOURCE foo_mod.f90 PROPERTY COMPILE_FLAGS ${GHOUL_OMP_FORTRAN_FLAGS})
set_source_files_properties(SOURCE suskevi.cpp PROPERTIES LANGUAGE HIP)
set_source_files_properties(SOURCE suskevi.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})
set_source_files_properties(SOURCE main.cpp PROPERTY COMPILE_FLAGS ${HIP_COMPILE_FLAGS})

set_target_properties(runGhoul PROPERTIES LINK_FLAGS "${GHOUL_OMP_LINK_FLAGS} ${HIP_LINK_FLAGS}")
