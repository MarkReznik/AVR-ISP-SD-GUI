#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H

#include <io.h>

//#define	_ATMEGA128
#ifndef	_ATMEGA328
#define	_ATMEGA328
#endif

//#define PRINT_DEBUG 1

#ifdef PRINT_DEBUG
//UART DEBUG
// ATmega328 clock [Hz]
#define FCLK 16000000
// Baud rate
#define BAUD 9600
// Calculate the UBRR setting
#define UBRR 103//for 16Mhz 9600
//#define UBRR ((long) FCLK/(16*BAUD)-1)
#endif

// pin definitions
#define DDR_SPI         DDRB
#define PORT_SPI        PORTB
#define PIN_SPI         PINB

#ifdef _ATMEGA128
#define CS              PINB0
#define MOSI            PINB2
#define MISO            PINB3
#define SCK             PINB1
#endif

#ifdef _ATMEGA328
#define CS              PINB2
#define MOSI            PINB3
#define MISO            PINB4
#define SCK             PINB5
#endif

// macros
#define CS_ENABLE()     PORT_SPI &= ~(1 << CS)
#define CS_DISABLE()    PORT_SPI |= (1 << CS)

#define SD_SUCCESS  0
#define SD_ERROR    1
#define SD_READY    0

#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94
#define CMD8        8
#define CMD8_ARG    0x0000001AA
#define CMD8_CRC    0x86 //(1000011 << 1)
#define CMD58       58
#define CMD58_ARG   0x00000000
#define CMD58_CRC   0x00
#define CMD55       55
#define CMD55_ARG   0x00000000
#define CMD55_CRC   0x00

#define ACMD41      41
#define ACMD41_ARG  0x40000000
#define ACMD41_CRC  0x00

void SPI_init();
unsigned char SPI_transfer(unsigned char data);
void SD_powerUpSeq();
unsigned char SD_command(unsigned char cmd, unsigned long arg, unsigned char crc);

unsigned char SD_readRes1();
unsigned char SD_goIdleState();
void SD_readRes7(unsigned char *res);
void SD_sendIfCond(unsigned char *res);
//void SD_readRes3_7(unsigned char *res);
void SD_readOCR(unsigned char *res);
unsigned char SD_sendApp();
unsigned char SD_sendOpCond();
unsigned char SD_init();
unsigned char SD_readSingleBlock(unsigned long addr, unsigned char *buf, unsigned char *token);

#ifdef PRINT_DEBUG
void putchar(char c);
void USART_Flush( void );
//unsigned char recchar(void);
void putstr(flash char *str);
void printhex(flash char *str,unsigned long var,unsigned char len);
#endif

#endif