/*****************************************************************************
Файл: stdafx.h
Creted by PROTTOSS
Mail to PROTTOSS@mail.ru
Общий заголовочный файл
08.07.2006
******************************************************************************/

#ifndef STDAFX_H
#define STDAFX_H

/*****************************************************************************
uCU definitions
******************************************************************************/
#define CPU_CLOCK			8000000 	// freq in Hz

/*****************************************************************************
Types definitions
******************************************************************************/
#define PROGMEM flash
#define MMC 1
#ifndef MMC

#define UCHAR	unsigned char
#define SCHAR	signed char
#define UINT	unsigned int
#define SINT	signed int
#define ULONG	unsigned long int
#define SLONG	signed long int

#define FALSE	0
#define TRUE	1
#define NULL	0

#endif

#define BOOL	UCHAR
#define uint32_t	unsigned long int
#define uint16_t	unsigned int
#define byte	unsigned char
#define uint8_t	unsigned char
#define boolean BOOL

#define MAKEULONG(byte_e, byte_h, byte_l)	(ULONG)((ULONG)((ULONG)byte_e << 16) | (ULONG)((UINT)(byte_h << 8) | (UINT)byte_l))
#define MAKEUINT(byte_h, byte_l)	(UINT)((UINT)(byte_h << 8) | (UINT)byte_l)
#define HIBYTE(word)				(UCHAR)((UINT)word >> 8)
#define LOBYTE(word)				(UCHAR)((UINT)word & 0xff)

#if defined __IAR_SYSTEMS_ICC__ || defined __IAR_SYSTEMS_ASM__

/* Enable bit definitions */
#ifndef ENABLE_BIT_DEFINITIONS
#   define ENABLE_BIT_DEFINITIONS	1
#endif

/* Include IAR headers */
#include <ioavr.h>
#ifndef __IAR_SYSTEMS_ASM__
#   include <inavr.h>
#endif

#define __attribute__(arg)
#define IAR_SECTION(section)    @ section

#ifndef USB_BUFFER_SECTION
#   define  USB_BUFFER_SECTION  "TINY_Z"    /* if user has not selected a named section */
#endif

#ifdef __IAR_SYSTEMS_ASM__
#   define __ASSEMBLER__
#endif

#ifdef __HAS_ELPM__
#   define PROGMEM __farflash
#else
#   define PROGMEM __flash
#endif

#define PRG_RDB(addr)   (*(PROGMEM char *)(addr))

#define _NOP()		__no_operation()
#define CLI()       __disable_interrupt()
#define SEI()       __enable_interrupt()
#define WDT_RESET() __watchdog_reset()

#endif  /* defined __IAR_SYSTEMS_ICC__ || defined __IAR_SYSTEMS_ASM__ */

#endif//STDAFX_H

