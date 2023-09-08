#!/bin/bash

case `hostname` in
    rztopaz*)
	ARCH=CTS1
	;;
    rzwhippet*)
	ARCH=CTS2
	;;
    rzvernal*)
	ARCH=EAS3
	;;
    lassen* | rzansel*)
        ARCH=ATS2
        ;;
esac

if [ $ARCH = EAS3 ]
then
    # For now, must load rocm module (version 5.5.1 for CCE 16). cmake
    # appears to be using either PATH or LD_LIBRARY_PATH for
    # something.
    CXX=/opt/cray/pe/cce/16.0.0/bin/craycxx
    F90=/opt/cray/pe/cce/16.0.0/bin/crayftn
    
fi



mkdir libpindus/build
pushd libpindus/build

# We have to manually supply the location of the gcc toolset, because
# CCE 16 is broken.
#
# Also we shouldn't have to pass in our own OpenMP flags.  We need to
# improve the CMakeLists.txt to do OpenMP the "right" way.
cmake ../src/CMakeLists.txt \
      -DCMAKE_INSTALL_PREFIX=../install \
      -DCMAKE_CXX_COMPILER=$CXX \
      -DCMAKE_Fortran_COMPILER=$F90 \
      -DCMAKE_Fortran_FLAGS=-ugcc_base=/opt/rh/gcc-toolset-10/root/usr \
      -DPINDUS_FORTRAN_FLAGS="-fopenmp;--rocm-path=$ROCM_PATH;-target-accel=amd_gfx90a" 

cmake --build .
cmake --install .

popd

mkdir libionian/build
pushd libionian/build

cmake ../src/CMakeLists.txt \
      -DCMAKE_INSTALL_PREFIX=../install \
      -DCMAKE_CXX_COMPILER=$CXX 
cmake --build .
cmake --install .

popd

