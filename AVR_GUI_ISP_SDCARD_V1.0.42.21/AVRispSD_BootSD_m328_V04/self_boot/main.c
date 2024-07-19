/*
 * self_boot.c
 *
 * Created: 13/01/2020 06:15:59
 * Author : mark.reznik
 */ 

#include <avr/io.h>
//#include "Self_programming.h"

int main(void)
{
    /* Replace with your application code */
    WriteFlashByte(0x8000,0x55);
	while (1) 
    {
    }
}

