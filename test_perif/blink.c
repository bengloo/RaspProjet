#include <stdio.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	BTN	6
#define	BUZZER	1
#define 	APPUYE LOW

int main (int argc, char ** argv) {
	int ledPin = 0 ; 
	
	if (argc!=1) {
		ledPin = atoi(argv[1]);
	}
	
	printf ("blink pin %d\n",ledPin) ;

	wiringPiSetup () ;
	pinMode (BUZZER, OUTPUT) ;
	pinMode (ledPin, OUTPUT) ;
	pinMode (BTN, INPUT) ;
	
	// activation pullup sur patte BTN 
	/*
	void pullUpDnControl (int pin, int pud) ;
	This sets the pull-up or pull-down resistor mode on the given pin, which should be set as an input. Unlike the Arduino, the BCM2835 has both pull-up an down internal resistors. The parameter pud should be; PUD_OFF, (no pull up/down), PUD_DOWN (pull to ground) or PUD_UP (pull to 3.3v) The internal pull up/down resistors have a value of approximately 50KΩ on the Raspberry Pi.

	This function has no effect on the Raspberry Pi’s GPIO pins when in Sys mode. If you need to activate a pull-up/pull-down, then you can do it with the gpio program in a script before you start your program.
	*/	
	
	pullUpDnControl (BTN,PUD_UP);
	
	
	for (;;) {
		if (digitalRead(BTN) == APPUYE)   {
			digitalWrite (ledPin, HIGH) ;	// On
		 	digitalWrite (BUZZER, HIGH) ;	// SON
		 } else {
		 	digitalWrite (ledPin, LOW) ;	// On
		 	digitalWrite (BUZZER, LOW) ;	// SON
		 }
	}
	return 0 ;
}


