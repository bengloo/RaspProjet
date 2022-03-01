#! /bin/bash
#à copier et executer directement dans le dossier ncuses.6.3
export TARGET_PI=/home/bengloo/Documents/LE3/rasp/RPI/target_pi
export PATH_CC=/home/bengloo/Documents/LE3/rasp/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
export CC=$PATH_CC/arm-linux-gnueabihf-gcc 
export CXX=$PATH_CC/arm-linux-gnueabihf-g++
export BUILD_CC=gcc 
./configure --prefix=$TARGET_PI --with-shared --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf --disable-stripping 
make -j9
make install

