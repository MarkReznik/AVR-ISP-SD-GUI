/*****************************************************************************
*
* (C) 2010, HP InfoTech srl, www.hpinfotech.com
*
* File              : flash.h
* Compiler          : CodeVisionAVR V2.xx
* Revision          : $Revision: 1.0 $
* Date              : $Date: December 9, 2010 $
* Updated by        : $Author: HP InfoTech $
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description       : Flash operations for AVR109 Self-programming
****************************************************************************/

#ifndef __FLASH_H
#define __FLASH_H

#ifdef  __ATmega128__
#define	_ATMEGA128	// device select: _ATMEGAxxxx
#endif

#ifdef   __ATmega328__
#define	_ATMEGA328	// device select: _ATMEGAxxxx
#endif

#ifndef   _ATMEGA328
#define	_ATMEGA328	// device select: _ATMEGAxxxx
#endif

/* definitions for SPM control */		
#ifdef _ATMEGA128
#define	SPMCR_REG	SPMCSR
#asm
     #define WR_SPMCR_REG_R22 sts 0x68,r22
#endasm
#endif

#ifdef _ATMEGA328
#define	SPMCR_REG	SPMCSR
#asm
     #define WR_SPMCR_REG_R22 out 0x37,r22
#endasm
#endif

#define _GET_LOCK_BITS() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0001, 0x09 )
#define _GET_LOW_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0000, 0x09 )
#define _GET_HIGH_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0003, 0x09 )
#define _GET_EXTENDED_FUSES() __AddrToZByteToSPMCR_LPM( (void flash *) 0x0002, 0x09 )
#define _SET_LOCK_BITS(data) __DataToR0ByteToSPMCR_SPM( data, 0x09 )
#define _ENABLE_RWW_SECTION() __DataToR0ByteToSPMCR_SPM( 0x00, 0x11 )
//#define _ENABLE_RWW_SECTION() __DataToR0ByteToSPMCR_SPM()



#ifdef _ATMEGA128
#define _WAIT_FOR_SPM() while( SPMCR_REG & (1<<SPMEN) );
#define _WAIT_FOR_EEWE() while(EECR & (1<<EEWE));                // Wait if ongoing EEPROM write
#endif

#ifdef _ATMEGA328
#define _WAIT_FOR_SPM() while( SPMCR_REG & (1<<SELFPRGEN) );
#define _WAIT_FOR_EEWE() while(EECR & (1<<EEPE));                // Wait if ongoing EEPROM write
#endif

#define _LOAD_PROGRAM_MEMORY(addr) (*( (unsigned char flash *) (addr) ))


/*
void MY_FILL_TEMP_WORD(unsigned long addr,unsigned int data);
void MY_PAGE_ERASE(unsigned long addr);
void MY_PAGE_WRITE(unsigned long addr);
*/

unsigned char __AddrToZByteToSPMCR_LPM(void flash *addr, unsigned char ctrl);
//void __DataToR0ByteToSPMCR_SPM(unsigned char data, unsigned char ctrl);
//void __DataToR0ByteToSPMCR_SPM(void);
//void __AddrToZByteToSPMCR_SPM(void flash *addr, unsigned char ctrl);
//void __AddrToZWordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl);

#ifdef _ATMEGA328
//void __AddrToZByteToSPMCR_SPM_E(void flash *addr);
//void __AddrToZByteToSPMCR_SPM_W(void flash *addr);
void __AddrToZWordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data);
void __AddrToZByteToSPMCR_SPM_W(void flash *addr);
void __AddrToZByteToSPMCR_SPM_E(void flash *addr);
void __AddrToZByteToSPMCR_SPM_EW(void flash *addr);
#endif

#ifdef _ATMEGA128
void __AddrToZ24WordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl);
void __AddrToZ24ByteToSPMCR_SPM(void flash *addr, unsigned char ctrl);
void __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data);
//void (*__AddrToZ24WordToR1R0ByteToSPMCR_SPM_F_Test)(void flash *addr, unsigned int data)= (void(*)(void flash *, unsigned int)) 0x0F9C6;
void __AddrToZ24ByteToSPMCR_SPM_W(void flash *addr);
void __AddrToZ24ByteToSPMCR_SPM_E(void flash *addr);
void __AddrToZ24ByteToSPMCR_SPM_EW(void flash *addr);
//void (*__AddrToZ24ByteToSPMCR_SPM_W_Test)(void flash *addr)= (void(*)(void flash *)) 0x0F9D6;
#endif

#ifdef _MODEL_SMALL_
  //#define _FILL_TEMP_WORD(addr,data) __AddrToZWordToR1R0ByteToSPMCR_SPM( (void flash *) (addr), data, 0x01 )
  //#define _PAGE_ERASE(addr) __AddrToZByteToSPMCR_SPM( (void flash*) (addr), 0x03 )
  //#define _PAGE_WRITE(addr) __AddrToZByteToSPMCR_SPM( (void flash*) (addr), 0x05 )
  #define _FILL_TEMP_WORD(addr,data) __AddrToZWordToR1R0ByteToSPMCR_SPM_F( (void flash *) (addr), data )
  #define _PAGE_ERASE(addr) __AddrToZByteToSPMCR_SPM_E( (void flash *) (addr) )
  #define _PAGE_WRITE(addr) __AddrToZByteToSPMCR_SPM_W( (void flash *) (addr) )
  //#define _PAGE_WRITE(addr) __AddrToZByteToSPMCR_SPM_EW( (void flash *) (addr) )
#else
  //#define _FILL_TEMP_WORD(addr,data) __AddrToZ24WordToR1R0ByteToSPMCR_SPM( (void flash *) (addr), data, 0x01 )
  //#define _FILL_TEMP_WORD(addr,data) __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F_Test( (void flash *) (addr), (unsigned int) data )
  #define _FILL_TEMP_WORD(addr,data) __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F( (void flash *) (addr), (unsigned int) data )
  #define _PAGE_ERASE(addr) __AddrToZ24ByteToSPMCR_SPM( (void flash *) (addr), 0x03 )
  //#define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM( (void flash *) (addr), 0x05 )
  #define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM_W( (void flash *) (addr) )
  #define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM_E( (void flash *) (addr) )
  //#define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM_W_Test( (void flash *) (addr) )
  //#define _PAGE_WRITE(addr) __AddrToZ24ByteToSPMCR_SPM_EW( (void flash *) (addr) )
#endif
//void dospm(void);

#endif