#include <stdio.h> 
#include <unistd.h> 
#include <stdint.h> 
#include <wiringPi.h> 
#include <wiringPiI2C.h> 

#include <sys/time.h> 

#include <stdbool.h> 

#define HT16K33_BLINK_CMD 0x80 
#define HT16K33_BLINK_DISPLAYON 0x01 
#define HT16K33_BLINK_OFF 0 
#define HT16K33_BLINK_2HZ  1 
#define HT16K33_BLINK_1HZ  2 
#define HT16K33_BLINK_HALFHZ  3 

#define HT16K33_CMD_BRIGHTNESS 0xE0 

#define SEVENSEG_DIGITS 5 

// déclaration globale 
uint16_t nombre[4]; 

static const uint8_t chiffre[] = { 
	0x3F, /* 0 */ 
	0x06, /* 1 */ 
	0x5B, /* 2 */ 
	0x4F, /* 3 */ 
	0x66, /* 4 */ 
	0x6D, /* 5 */ 
	0x7D, /* 6 */ 
	0x07, /* 7 */ 
	0x7F, /* 8 */ 
	0x6F, /* 9 */ 
	0xBF, /* 0.*/ 
	0x86, /* 1.*/ 
	0xDB, /* 2.*/ 
	0xCF, /* 3.*/ 
	0xE6, /* 4.*/ 
	0xED, /* 5.*/ 
	0xFD, /* 6.*/ 
	0x87, /* 7.*/ 
	0xFF, /* 8.*/ 
	0xEF, /* 9.*/ 
	0x00, // rien du tout

}; 

void setBrightness(int fd, uint8_t b) { 
  if (b > 15) b = 15; 
  wiringPiI2CWrite(fd, HT16K33_CMD_BRIGHTNESS | b); 
} 

void blinkRate(int fd, uint8_t b) { 
  if (b > 3) b = 0; // turn off if not sure 
  wiringPiI2CWrite(fd, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)); 
} 

void begin(int fd) { 
  wiringPiI2CWrite(fd, 0x21); 
  blinkRate(fd, HT16K33_BLINK_OFF); 
  setBrightness(fd, 15); // max brightness 
} 

void writeDisplay(int fd) { 
  uint8_t addr = (uint8_t) 0x00;	 
  uint8_t i=0; 
  for(i=0 ; i<4 ; i++) { 
	if(i==2) // 2 points 
	    addr+=2;
        wiringPiI2CWriteReg8(fd, addr++, chiffre[nombre[i]] & 0xFF); 
        wiringPiI2CWriteReg8(fd, addr++, chiffre[nombre[i]] >> 8);  
  }  
} 


void initNombre(){
int i;

for (i=0;i<4;i++)
	nombre[i] = 0;

}

void decomposer(int nb){
int i,masque =1000;

initNombre();

if (nb > 9999) nb =9999;

	for (i=0;i<4;i++){
		while(nb >= masque)
		{
		nombre[i]++;
		nb = nb - masque;
		}
	masque = masque /10;
	}

}

int main() 
{ 
	// setup 
	int fda = wiringPiI2CSetup(0x70); 
	int timeCentieme = 1000;
	printf("fda : %d \n", fda); 

	// begin 
	begin(fda); 



        // loop 
	int num = 0; 
	while( timeCentieme-- != 0) { 

	decomposer(timeCentieme);
          
		writeDisplay(fda); 
		 
		// sleep 
		usleep(10); // second 
		 
		  
	} 
	return 1; 
} 
