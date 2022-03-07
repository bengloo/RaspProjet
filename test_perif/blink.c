/*
 * blink.c:
 *	Standard "blink" program in wiringPi. Blinks an LED connected
 *	to the first GPIO pin.
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */
#include "/home/bengloo/Documents/LE3/rasp/wiringpi/wiringPi-36fb7f1/wiringPi/wiringPi.h"
#include <stdio.h>
//#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	BTN 6 
#define	BUZZER 1
#define appuyer LOW
 

int main (int argc, int **argv)
{
  
	int ledPin=0;

	if(argc!=1){
	ledPin=atoi(argv[1]);
	}

printf ("inesblink pin %d\n,ledPin") ;

  wiringPiSetup () ;
pinMode (ledPin, OUTPUT) ;
pinMode (BTN, OUTPUT) ;
pinMode (BUZZER, OUTPUT) ;

pullUpDnControl(BTN,PUD_UP);

  while(1)
  {  
	if(digitalRead==appuyer){
    	digitalWrite (ledPin, HIGH) ;	// On
      	digitalWrite (BUZZER, HIGH) ;
	}
	else{
	digitalWrite (ledPin, LOW) ;	// On
      	digitalWrite (BUZZER, LOW) ;	
	}

  }
	
  return 0 ;
}




