/***************************************************************************** 
* 
* Atmel Corporation 
* 
* File              : Self_programming.h 
* Compiler          : IAR EWAAVR 2.28a / 3.10c 
* Revision          : $Revision: 1.6 $ 
* Date              : $Date: Monday, April 04, 2005 12:14:58 UTC $ 
* Updated by        : $Author: raapeland $ 
* 
* Support mail      : avr@atmel.com 
* 
* Supported devices : Any device having the "Self-programming" feature. 
* 
* AppNote           : AVR106: C functions for reading and writing to FLASH. 
* 
* Description       : The file contains functions prototypes and necessary 
*                     definitions for the functions contained in application 
*                     note "AVR106: C functions for reading and writing to 
*                     FLASH". These functions provide easy reading and writing 
*                     of Flash memory on parts having the "Self-programming" 
*                     feature. 
******************************************************************************/ 
//#include <ioavr.h> 
 
#ifndef __SELF_PROGRAMMING_H 
#define __SELF_PROGRAMMING_H 
 
 
/****************************************************************************** 
* Defines for configuration 
******************************************************************************/ 
#ifndef __ATmega128__
#define __ATmega128__ 
#endif
//#define __FLASH_RECOVER       // This define enables FLASH buffering to avoid 
                              // data loss if reset or power break when writing 
#define ADR_FLASH_BUFFER 0    // Defines the FLASH page address used for buffering 
                              // if enabeled. Will use Flash page 0 for buffering 
#define ADR_LIMIT_LOW   0     // Defines the low address limit for FLASH write 
 
#define BOOTSECTORSIZE 0x2000 // 4096 words 
#define ADR_LIMIT_HIGH  FLASHEND-BOOTSECTORSIZE// Defines the high address limit for FLASH write 
 
/****************************************************************************** 
* Definition of PAGESIZE 
******************************************************************************/ 
 
#if defined(__ATtiny13__) | defined(__AT90Tiny13__) |\
    defined(__ATtiny2313__) | defined(__AT90Tiny2313__) 
#define PAGESIZE 32 
#endif 
 
#if defined(__ATmega88__)   | defined(__AT90Mega88__) |\
    defined(__ATmega48__)  | defined(__AT90Mega48__) |\
    defined(__ATmega168__) | defined(__AT90Mega168__) |\
    defined(__ATmega8__)   | defined(__AT90Mega8__)   |\
    defined(__ATmega8515__)  | defined(__AT90Mega8515__) |\
    defined(__ATmega8535__) | defined(__AT90Mega8535__) 
#define PAGESIZE 64 
#endif 
 
#if defined(__ATmega162__) | defined(__AT90Mega162__) |\
    defined(__ATmega163__) | defined(__AT90Mega163__)  |\
    defined(__ATmega169__) | defined(__AT90Mega169__)  |\
    defined(__ATmega161__) | defined(__AT90Mega161__) |\
    defined(__ATmega16__) | defined(__AT90Mega16__) |\
    defined(__ATmega32__) | defined(__AT90Mega32__)  |\
    defined(__ATmega323__) | defined(__AT90Mega323__) 
#define PAGESIZE 128 
#endif 
 
#if defined(__ATmega128__) | defined(_CHIP_ATMEGA128_) | defined(__AT90Mega128__) |\
    defined(__ATmega64__) | defined(__AT90Mega64__) |\
    defined(__ATmega2560__) 
#define __ATmega128__
#define PAGESIZE 256 
#endif 
 
#ifndef PAGESIZE 
#error "PAGESIZE not defined" 
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
#if defined(__ATmega16__)   | defined(__AT90Mega16__) |\
    defined(__ATmega161__)  | defined(__AT90Mega161__) |\
    defined(__ATmega162__) | defined(__AT90Mega162__) |\
    defined(__ATmega163__)   | defined(__AT90Mega163__)   |\
    defined(__ATmega32__) | defined(__AT90Mega32__) |\
    defined(__ATmega323__)   | defined(__ATMega323__) |\
    defined(__ATmega64__)  | defined(__AT90Mega64__) |\
    defined(__ATmega8__) | defined(__AT90Mega8__) |\
    defined(__ATmega8535__) | defined(__AT90Mega8535__) |\
    defined(__ATmega8515__) | defined(__AT90Mega8515__) 
#define SPMControllRegister SPMCR 
#endif 
 
#if defined(__ATmega128__) | defined(__AT90Mega128__) |\
    defined(__ATmega168__) | defined(__AT90Mega168__)  |\
    defined(__ATmega169__)  | defined(__AT90Mega169__)  |\
    defined(__ATmega48__) | defined(__AT90Mega48__) |\
    defined(__ATmega88__) | defined(__AT90Mega88__) |\
    defined(__ATtiny13__) | defined(__AT90Tiny13__)  |\
    defined(__ATtiny2313__)  | defined(__AT90Tiny2313__) |\
    defined(__ATmega2560__) 
#define SPMControllRegister SPMCSR 
#endif 
 
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
unsigned char ReadFlashByte(MyAddressType flashAdr); 
unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage); 
unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data); 
unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage); 
unsigned char RecoverFlash(void); 
 
/* Functions used by user functions */ 
unsigned char AddressCheck(MyAddressType flashAdr); 
void WriteBufToFlash(MyAddressType flashStartAdr); 
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data); 
 
#endif /* End if __BOOT_FUNC_H */ 