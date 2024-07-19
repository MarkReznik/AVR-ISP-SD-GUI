// Useful message printing definitions
#include "optiLoader.h"
#include "avr910.h"
#include <stdio.h>
#include <ctype.h>
/*
 * Low level support functions
 */

/*
 * flashprint
 * print a text string direct from flash memory to Serial
 */
//void flashprint (char *p,uint16_t length)
//{
//    byte c;
//    while(length--){
//     c=*p;
//     if(isalnum(c)!=1)return;
//     putchar(c);
//     p++;
//    }
//}

/*
 * hexton
 * Turn a Hex digit (0..9, A..F) into the equivalent binary value (0-16)
 */

//byte hextob (byte h,byte l)
//{
//  return (hexton(h)<<4)|hexton(l);
//}

void error(const char *str)
{
   puts("Error ");
   puts(str);
}
/*
 * pulse
 * turn a pin on and off a few times; indicates life via LED
 */
//#define PTIME 30
//void pulse (int pin, int times) {
//  do {
//    digitalWrite(pin, HIGH);
//    delay(PTIME);
//    digitalWrite(pin, LOW);
//    delay(PTIME);
//  }
//  while (times--);
//}

