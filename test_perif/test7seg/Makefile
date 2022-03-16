MAIN=chrono
EXEC=$(MAIN)
op=-Wall -Wextra #opertion et retoure de la compilation

TARGET_PI=../../target_pi
CFLAGS_PI=-I$(TARGET_PI)/include
LDFLAGS_PI=-L$(TARGET_PI)/lib
GCC_PI=../../../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc


all:
	@echo "generation de l'executable pour le raspberriPi en cross compilation depuis le pc"
	$(GCC_PI) $(CFLAGS_PI) $(LDFLAGS_PI) $(MAIN).c -o $(EXEC) $(op) -lm -lwiringPi -std=c99
	@echo ""

