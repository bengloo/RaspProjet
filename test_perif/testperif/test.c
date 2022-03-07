//#include "/home/bengloo/Documents/LE3/rasp/wiringpi/wiringPi-36fb7f1/wiringPi/wiringPi.h"
#include <wiringPi.h>
#define BTN 6
#define BUZZER 1

int main (void)
{
  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  for (;;)
  {
    digitalWrite (BUZZER, HIGH) ; delay (500) ;
    digitalWrite (BUZZER,  LOW) ; delay (500) ;
  }
  return 0 ;
}