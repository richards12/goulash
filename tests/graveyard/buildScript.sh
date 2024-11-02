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
    rzadams*)
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
    HOSTCONFIG_FILE=lc-rzadams.cmake
    
fi


# build libpindus
mkdir libpindus/build libpindus/install
pushd libpindus/build

cmake -C ../../host_configs/lc-rzadams.cmake -DCMAKE_INSTALL_PREFIX=../install ../src

cmake --build .
cmake --install .

popd

# build libionian
mkdir libionian/build libionian/install
pushd libionian/build

cmake -C ../../host_configs/lc-rzadams.cmake -DCMAKE_INSTALL_PREFIX=../install ../src
cmake --build .
cmake --install .

popd

# build driver
mkdir build install
pushd build

cmake -C ../host_configs/lc-rzadams.cmake -DCMAKE_INSTALL_PREFIX=../install -DENABLE_PINDUS=ON -DENABLE_IONIAN=ON -DPINDUS_PREFIX=../libpindus/install -DIONIAN_PREFIX=../libionian/install ../src
cmake --build .
cmake --install .
popd
