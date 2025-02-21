#include "stdafx.h"
/*
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <avr/pgmspace.h>
#include "SPI.h"
*/
#ifndef _OPTILOADER_H
#define _OPTILOADER_H


#define MAX_FUSES   4
#define FUSE_PROT 0			/* memory protection */
#define FUSE_LOW 1			/* Low fuse */
#define FUSE_HIGH 2			/* High fuse */
#define FUSE_EXT 3			/* Extended fuse */

// You may want to tweak these based on whether your chip is
// using an internal low-speed crystal
#define CLOCKSPEED_FUSES   SPI_CLOCK_DIV128
#define CLOCKSPEED_FLASH   SPI_CLOCK_DIV8

#define LED_ERR 8
#define LED_PROGMODE A0

//typedef struct image {
//    char image_name[30];	       /* Ie "optiboot_diecimila.hex" */
//    char image_chipname[12];	       /* ie "atmega168" */
//    byte chipcode;                  /* device code ie m64 is 0x45 */
//    uint16_t image_chipsig;	       /* Low two bytes of signature */
//    byte image_progfuses[5];	       /* fuses to set during programming */
//    byte image_normfuses[5];	       /* fuses to set after programming */
//    byte fusemask[4];
//    uint32_t chipsize;
//    uint16_t image_pagesize;	       /* page size for flash programming */
//    byte image_hexcode[22];	       /* intel hex format image (text) */
//} image_t;
//
//typedef struct alias {
//  char image_chipname[12];
//  uint16_t image_chipsig;
//  image_t * alias_image;
//} alias_t;

//extern image_t *images[];
//extern uint8_t NUMIMAGES;
// Useful message printing definitions

//#define debug(string) // flashprint(PSTR(string));


//void pulse (int pin, int times);
//void flashprint (char *p,uint16_t length);


//uint16_t spi_transaction (uint8_t a, uint8_t b, uint8_t c, uint8_t d);
//image_t *findImage (uint16_t signature);
//byte pgm_read_byte(byte *addr);
//uint16_t pgm_read_word(uint16_t *addr);
//uint32_t pgm_read_dword(uint32_t *addr);

//uint16_t readSignature (void);
UCHAR programFuses (byte *fuses,byte *fusemask);
//void eraseChip(void);
//boolean verifyImage (byte *hextext);
//void busyWait(void);
UCHAR flashPage (byte *pagebuff, uint16_t pagesize);
//void flashWord (uint8_t hilo, uint16_t addr, uint8_t data);
byte hexton (byte h);
//byte * readImagePage (byte *hextext, uint32_t pageaddr, uint16_t pagesize, byte *page);
BOOL verifyFuses (byte *fuses, byte *fusemask);
//void error(char *string);
void error(const char *str);

#endif
