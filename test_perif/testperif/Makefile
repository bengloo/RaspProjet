TARGET_PI=../target_pi
CFLAGS_PI=-I$(TARGET_PI)/include
LDFLAGS_PI=-L$(TARGET_PI)/lib
c_PI=../../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
all:
	$(c_PI) $(CFLAGS_PI) $(LDFLAGS_PI) -o test test.c -lwiringPi