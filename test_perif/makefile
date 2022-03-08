DESTDIR?=/usr
PREFIX?=/local

ifneq ($V,1)
Q ?= @
endif

TARGET_CC=../../../../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
CC=$(TARGET_CC)/arm-linux-gnueabihf-gcc
TARGET_WP=target-wp

CFLAGS= -I$(TARGET_WP)/include
LDFLAGS= -L$(TARGET_WP)/lib
LIB= -lwiringPi

all:simple blink

simple:
	@echo "execution de la regle simple"

blink: blink.c
		$Q $(CC) $(CFLAGS) $(LDFLAGS) blink.c -o blink $(LIB)
