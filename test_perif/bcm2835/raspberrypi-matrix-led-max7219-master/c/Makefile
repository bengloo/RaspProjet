# sample makefile
CC = /home/tom/Bureau/enseignements/enseignements/IG2I/CCR/2022/S3_prepa/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
CFLAGS = -Wall -std=c11
OUTFILE = led-max7219-text
OBJS = text.o
SRCS = text.c

# cd rep lib bcm2835
# export CC=cheminduCCC
# ./configure --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf --prefix=$PATH_TARGETBCM
#make 
#make install 


PATH_BCM=/home/tom/Bureau/enseignements/enseignements/IG2I/CCR/2022/S3_prepa/target_bcm2835
INC=-I$(PATH_BCM)/include
LIB=-L$(PATH_BCM)/lib

$(OUTFILE): $(OBJS)
	$(CC) $(LIB) $(CFLAGS) -o $(OUTFILE) $(OBJS) -lbcm2835
$(OBJS): $(SRCS)
	$(CC) $(INC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY : all
all : $(OUTFILE)
