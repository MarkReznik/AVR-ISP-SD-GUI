// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* - File              : Self_programming.h
* - Compiler          : IAR EWAVR 2.28a / 3.10c and newer
*
* - Support mail      : avr@atmel.com
*
* - Supported devices : All devices with bootloaders support.
*
* - AppNote           : AVR106 - C functions for reading and writing
*                       to flash memory.
*
* - Description       : The file contains functions for easy reading and writing
*                       of Flash memory on parts having the "Self-programming"
*                       feature. The user functions are as follows:
*
*                       ReadFlashByte()	
*                       ReadFlashPage()	
*                       WriteFlashByte()
*                       WriteFlashPage()	
*                       RecoverFlash()	
*
*                       The remaining functions contained in this file are used
*                       by the functions listet above.
*
* $Revision: 2.0 $
* $Date: Wednesday, January 18, 2006 15:18:54 UTC $
*
*****************************************************************************/
//#include <ioavr.h>

#ifndef __SELF_PROGRAMMING_H
#define __SELF_PROGRAMMING_H

//void dospm(void);

/******************************************************************************
* Defines for configuration
******************************************************************************/
//#define __ATmega128__
#define __ATmega328__

#ifdef  __ATmega128__
#define FLASHEND 0x1FFFFUL    // 64K Words
//#define __FLASH_RECOVER       // This define enables FLASH buffering to avoid
                              // data loss if reset or power break when writing
#define ADR_FLASH_BUFFER 0x1000UL    // Defines the FLASH page address used for buffering
                              // if enabeled. Will use Flash page 0 for buffering
#define ADR_LIMIT_LOW   0x0UL     // Defines the low address limit for FLASH write

#define BOOTSECTORSIZE 0x1000U // 2048 words 0x2000=4096 words
#define ADR_LIMIT_HIGH  FLASHEND-BOOTSECTORSIZE// Defines the high address limit for FLASH write

#endif

#ifdef  __ATmega328__
#define FLASHEND 0x7FFFUL    // 32K Bytes, 16K Words
//#define __FLASH_RECOVER       // This define enables FLASH buffering to avoid
                              // data loss if reset or power break when writing
#define ADR_FLASH_BUFFER 0x1000UL    // Defines the FLASH page address used for buffering
                              // if enabeled. Will use Flash page 0 for buffering
#define ADR_LIMIT_LOW   0x0UL     // Defines the low address limit for FLASH write

#define BOOTSECTORSIZE 0x1000U // 2048 words 0x2000=4096 words
#define ADR_LIMIT_HIGH  FLASHEND-BOOTSECTORSIZE// Defines the high address limit for FLASH write

#ifndef PRINT_DEBUG
/*ADC defines*/
#define MAX_BUTTONS     4
#define ADC_VREF_TYPE   0x20


enum
{   BUTTONS_RELEASED=0,
    BUTTON_START,
    BUTTON_STOP,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTONS_PRESSED
};//Buttons;
#endif

#define LED_OK_PORT      PORTB
#define LED_OK_DDR       DDRB
#define LED_OK_PIN       PINB
#define PIN_LED_OK	     1
#define LED_OK_TGL()       LED_OK_PORT.PIN_LED_OK=~LED_OK_PORT.PIN_LED_OK
#define LED_OK_ON()        LED_OK_PORT.PIN_LED_OK=0
#define LED_OK_OFF()       LED_OK_PORT.PIN_LED_OK=1
#define LED_ERR_PORT      PORTB
#define LED_ERR_DDR       DDRB
#define LED_ERR_PIN       PINB
#define PIN_LED_ERR	      0
#define LED_ERR_TGL()       LED_ERR_PORT.PIN_LED_ERR=~LED_ERR_PORT.PIN_LED_ERR
#define LED_ERR_ON()        LED_ERR_PORT.PIN_LED_ERR=0
#define LED_ERR_OFF()       LED_ERR_PORT.PIN_LED_ERR=1

#endif

/******************************************************************************
* Definition of PAGESIZE
******************************************************************************/

#if defined(__ATtiny13__)   || defined(__AT90Tiny13__)   || \
    defined(__ATtiny2313__) || defined(__AT90Tiny2313__)
#define PAGESIZE 32
#endif

#if defined(__ATmega88__)   || defined(__AT90Mega88__)   || \
    defined(__ATmega48__)   || defined(__AT90Mega48__)   || \
    defined(__ATmega8__)    || defined(__AT90Mega8__)    || \
    defined(__ATmega8515__) || defined(__AT90Mega8515__) || \
    defined(__ATmega8535__) || defined(__AT90Mega8535__)
#define PAGESIZE 64
#endif

#if defined(__ATmega162__)  || defined(__AT90Mega162__)  || \
    defined(__ATmega163__)  || defined(__AT90Mega163__)  || \
    defined(__ATmega168__)  || defined(__AT90Mega168__)  || \
    defined(__ATmega169__)  || defined(__AT90Mega169__)  || \
    defined(__ATmega161__)  || defined(__AT90Mega161__)  || \
    defined(__ATmega16__)   || defined(__AT90Mega16__)   || \
    defined(__ATmega32__)   || defined(__AT90Mega32__)   || \
    defined(__ATmega323__)  || defined(__AT90Mega323__)  || \
    defined(__ATmega328__)  || defined(__AT90Mega329__)  || \
    defined(__ATmega329__)  || defined(__AT90Mega329__)  || \
    defined(__ATmega3290__) || defined(__AT90Mega3290__)
#define PAGESIZE 128
#endif

#if defined(__ATmega64__)   || defined(__AT90Mega64__)   || \
    defined(__ATmega640__)  || defined(__AT90Mega640__)  || \
    defined(__ATmega649__)  || defined(__AT90Mega649__)  || \
    defined(__ATmega6490__) || defined(__AT90Mega6490__) || \
    defined(__ATmega128__)  || defined(__AT90Mega128__)  || \
    defined(__ATmega1280__) || defined(__AT90Mega1280__) || \
    defined(__ATmega1281__) || defined(__AT90Mega1281__) || \
    defined(__ATmega2560__) || defined(__AT90Mega2560__) || \
    defined(__ATmega2561__) || defined(__AT90Mega2561__)
#define PAGESIZE 256
#define	LARGE_MEMORY
#define	__HAS_ELPM__
#endif

/******************************************************************************
* Definition of datatypes
* All functions uses integer types for passing Flash addresses.
******************************************************************************/

#ifdef __HAS_ELPM__
typedef unsigned long MyAddressType;                  // Used for passing address to functions
typedef unsigned char __farflash* MyFlashCharPointer; // Used for reading Flash
typedef unsigned int __farflash* MyFlashIntPointer;  // Used for reading Flash
#else
typedef unsigned int  MyAddressType;                  // Used for passing address to functions
typedef unsigned char __flash* MyFlashCharPointer;    // Used for reading Flash
typedef unsigned int  __flash* MyFlashIntPointer;
#endif


/******************************************************************************
* Definition of SPM controll register. Labeled SPMCR on some devices, SPMSCR
* on others.
******************************************************************************/
/*
#if defined(__ATmega16__)   || defined(__AT90Mega16__)   || \
    defined(__ATmega161__)  || defined(__AT90Mega161__)  || \
    defined(__ATmega162__)  || defined(__AT90Mega162__)  || \
    defined(__ATmega163__)  || defined(__AT90Mega163__)  || \
    defined(__ATmega32__)   || defined(__AT90Mega32__)   || \
    defined(__ATmega323__)  || defined(__ATMega323__)    || \
    defined(__ATmega64__)   || defined(__AT90Mega64__)   || \
    defined(__ATmega8__)    || defined(__AT90Mega8__)    || \
    defined(__ATmega8535__) || defined(__AT90Mega8535__) || \
    defined(__ATmega8515__) || defined(__AT90Mega8515__)
#define SPMControllRegister SPMCR
#else
#define SPMControllRegister SPMCSR
#endif
*/
/******************************************************************************
* Other defines
******************************************************************************/

#define FLASH_BUFFER_FULL_ID  0xAA
#define TRUE 1
#define FALSE 0

/******************************************************************************
* Function prototypes
******************************************************************************/

/* User functions */
#ifndef PRINT_DEBUG
unsigned char TestADCs(void);
#endif
//void dospm(void);

unsigned char ReadFlashByte(MyAddressType flashStartAdr);
unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage);
unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data);
unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage);
unsigned char RecoverFlash();

/* Functions used by user functions */
unsigned char AddressCheck(MyAddressType flashAdr);

void WriteBufToFlash(MyAddressType flashStartAdr);
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data);

#endif /* End if __BOOT_FUNC_H */
