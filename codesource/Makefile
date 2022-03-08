MAIN=temple_run
EXEC=$(MAIN)_
op=#-Wall -Wextra #opertion et retoure de la compilation

TARGET_PI=../target_pi
CFLAGS_PI=-I$(TARGET_PI)/include
LDFLAGS_PI=-L$(TARGET_PI)/lib
GCC_PI=../../tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

all:PC PI

debug: op += -DDEBUG
debug: all

#compilation de la verssion PC.
PC:
	gcc $(MAIN).c -o $(EXEC)pc $(op) -lX11 -lm

#compilation de la version PI compilé en croisé.
PI:
	$(GCC_PI) $(CFLAGS_PI) $(LDFLAGS_PI) $(MAIN).c -o $(op) -D'PI' $(EXEC)pi -lX11 -lm -lwinringPi

#compilation de la version PI non croisé
PI_sur_PI:
	gcc $(MAIN).c -o $(EXEC)pi $(op) -DPI -lX11 -lm -lwinringPi


clean:
	rm $(EXEC)*
	rm *.o