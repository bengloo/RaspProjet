#! /bin/bash
#à copier et executer directement dans le dossier ncuses.6.3
#tar -xf ncurses_programs.tar.gz
tar -xf ncurses.tar.gz
export PATH_PROJECT=$(pwd)/..
export TARGET_PI=$PATH_PROJECT/target_pi
export PATH_CC=$PATH_PROJECT/../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
if [ -e $(PATH_CC) ]
then
    export CC=$PATH_CC/arm-linux-gnueabihf-gcc 
	export CXX=$PATH_CC/arm-linux-gnueabihf-g++
	export BUILD_CC=gcc 
	cd ncurses-6.3 && ./configure --prefix=$TARGET_PI --with-shared --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf --disable-stripping 
	make -j9 
	make install
	cd .. && rm -r ncurses-6.3
else
    echo "Le dossier tools-master n'est pas situer à coté du projet, ajouté le ou edité le chemin dans g3.sh"
fi



