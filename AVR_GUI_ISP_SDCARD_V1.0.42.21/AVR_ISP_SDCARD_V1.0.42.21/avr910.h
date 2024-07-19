#ifndef AVR910_H
#define AVR910_H

#include "integer.h"

#define MAKEULONG(byte_e, byte_h, byte_l)    (ULONG)((ULONG)((ULONG)byte_e << 16) | (ULONG)((UINT)(byte_h << 8) | (UINT)byte_l))
#define MAKEUINT(byte_h, byte_l)    (UINT)((UINT)(byte_h << 8) | (UINT)byte_l)
#define HIBYTE(word)                (UCHAR)((UINT)word >> 8)
#define LOBYTE(word)				(UCHAR)((UINT)word & 0xff)

/*****************************************************************************
Определение выводов ISP
******************************************************************************/
#define LED_ERR_TGL()       LED_ERR_PORT.PIN_LED_ERR=~LED_ERR_PORT.PIN_LED_ERR
#define LED_ERR_ON()        LED_ERR_PORT.PIN_LED_ERR=0
#define LED_ERR_OFF()       LED_ERR_PORT.PIN_LED_ERR=1

#define LED_OK_TGL()       LED_OK_PORT.PIN_LED_OK=~LED_OK_PORT.PIN_LED_OK
#define LED_OK_ON()        LED_OK_PORT.PIN_LED_OK=0
#define LED_OK_OFF()       LED_OK_PORT.PIN_LED_OK=1


//#define TGL_LED_PASS()      TGL_LED_ERR()   //LED_PORT ^=     (1 << PIN_LED_PASS)
//#define SET_LED_PASS()      SET_LED_ERR()            //LED_PORT |=     (1 << PIN_LED_PASS)
//#define CLR_LED_PASS()      CLR_LED_ERR()            //LED_PORT &= ~(1 << PIN_LED_PASS)
#define LED_PASS_TGL()      LED_OK_TGL() 
#define LED_PASS_ON()       LED_OK_ON()
#define LED_PASS_OFF()      LED_OK_OFF()
//#define SET_LED_WR()        SET_LED_OK()                //LED_PORT |=     (1 << PIN_LED_WR)
//#define CLR_LED_WR()        CLR_LED_OK()                //LED_PORT &= ~(1 << PIN_LED_WR)
#define LED_WR_ON()         LED_OK_ON()
#define LED_WR_OFF()        LED_OK_OFF()
//#define SET_LED_RD()        SET_LED_OK()                //LED_PORT |=     (1 << PIN_LED_RD)
//#define CLR_LED_RD()        CLR_LED_OK()                //LED_PORT &= ~(1 << PIN_LED_RD)
#define LED_RD_ON()         LED_OK_ON()
#define LED_RD_OFF()        LED_OK_OFF()
#define SET_RESET()         ISP_RESET_PORT.PIN_ISP_RESET=0          //ISP_PORT |=     (1 << PIN_RESET)
#define CLR_RESET()         ISP_RESET_PORT.PIN_ISP_RESET=1          //ISP_PORT &= ~(1 << PIN_RESET)
#define SET_MOSI()          ISP_MOSI_PORT.PIN_ISP_MOSI=1            //ISP_PORT |=     (1 << PIN_MOSI)
#define CLR_MOSI()          ISP_MOSI_PORT.PIN_ISP_MOSI=0            //ISP_PORT &= ~(1 << PIN_MOSI)
#define SET_MISO()          ISP_MISO_PORT.PIN_ISP_MISO=1            //ISP_PORT |=     (1 << PIN_MISO)
#define CLR_MISO()          ISP_MISO_PORT.PIN_ISP_MISO=0            //ISP_PORT &= ~(1 << PIN_MISO)
#define READ_MISO()         ISP_MISO_PIN.PIN_ISP_MISO
#define SET_SCK()		    ISP_SCK_PORT.PIN_ISP_SCK=1              //ISP_PORT |=	 (1 << PIN_SCK)
#define CLR_SCK()		    ISP_SCK_PORT.PIN_ISP_SCK=0              //ISP_PORT &= ~(1 << PIN_SCK)
#define ISP_ON()            {ISP_RESET_INIT();ISP_MOSI_INIT();ISP_MISO_INIT();ISP_SCK_INIT();}
#define ISP_OFF()           {ISP_RESET_DEINIT();ISP_MOSI_DEINIT();ISP_MISO_DEINIT();ISP_SCK_DEINIT();}
#define LEDS_OFF()          {LED_OK_OFF();LED_ERR_OFF();}
/*SPI init*/
//#define ISP_PORT        PORTC
//#define ISP_DDR         DDRC
//#define ISP_PIN         PINC
//
//#define LED_PORT        PORTB
//#define LED_PIN         PINB
//#define LED_DDR         DDRB

//#define CLOCK_DDR       DDRB

//#define PIN_LOW_SCK     6 //Jumper to select low ISP programming frequency
//#define PIN_CLOCK     5 //PB5=OC1A 1Mhz clock if need external oscilator

/*MUX Select PIN definitions PD7 Output Low*/
//#define MUX_SEL_PORT      PORTD
//#define MUX_SEL_DDR       DDRD
//#define MUX_SEL_PIN       PIND
//#define PIN_MUX_SEL       7
//#define MUX_SEL_INIT()    {MUX_SEL_DDR.PIN_MUX_SEL=1; MUX_SEL_PORT.PIN_MUX_SEL=0;}
/*RESET PIN definitions PD5 Output Inverted High*/
#define ISP_RESET_PORT      PORTD
#define ISP_RESET_DDR       DDRD
#define ISP_RESET_PIN       PIND
#define PIN_ISP_RESET       5
#define ISP_RESET_INIT()    {ISP_RESET_DDR.PIN_ISP_RESET=1; SET_RESET();}
#define ISP_RESET_DEINIT()  ISP_RESET_INIT();//{ISP_RESET_DDR.PIN_ISP_RESET=1; SET_RESET();}
/*MOSI PIN definitions PD3 Output Low*/
#define ISP_MOSI_PORT      PORTD
#define ISP_MOSI_DDR       DDRD
#define ISP_MOSI_PIN       PIND
#define PIN_ISP_MOSI	   1// PD1 USART SPI MOSI. WAS PD3
#define ISP_MOSI_INIT()    {ISP_MOSI_DDR.PIN_ISP_MOSI=1; CLR_MOSI();}
#define ISP_MOSI_DEINIT()  {ISP_MOSI_DDR.PIN_ISP_MOSI=0; CLR_MOSI();}
/*MISO PIN definitions PD2 Input*/
#define ISP_MISO_PORT      PORTD
#define ISP_MISO_DDR       DDRD
#define ISP_MISO_PIN       PIND
#define PIN_ISP_MISO	   0// PD0 USART SPI MISO. WAS PD2
#define ISP_MISO_INIT()    {ISP_MISO_DDR.PIN_ISP_MISO=0; CLR_MISO();}
#define ISP_MISO_DEINIT()  {CLR_MISO();}
/*Clock PIN definitions PD7 Output Low*/
#define ISP_SCK_PORT      PORTD
#define ISP_SCK_DDR       DDRD
#define ISP_SCK_PIN       PIND
#define PIN_ISP_SCK		  4// PD4 USART SPI CLOCK. WAS PD7
#define ISP_SCK_INIT()    {ISP_SCK_DDR.PIN_ISP_SCK=1; CLR_SCK();}
#define ISP_SCK_DEINIT()  {ISP_SCK_DDR.PIN_ISP_SCK=0; CLR_SCK();}

//#define PIN_SS		    0

/***********  LEDs definitions   *************/
/*LED(OK)=Read/Write PIN definitions PB1 Output Inverted*/
#ifdef _MEGA64_INCLUDED_
#define LED_OK_PORT      PORTC
#define LED_OK_DDR       DDRC
#define LED_OK_PIN       PINC
#define PIN_LED_OK	     7
#else
#define LED_OK_PORT      PORTB
#define LED_OK_DDR       DDRB
#define LED_OK_PIN       PINB
#define PIN_LED_OK	     1
#endif


#define LED_OK_INIT()    {LED_OK_DDR.PIN_LED_OK=1; LED_OK_OFF();}
/*LED(Error)=Pass PIN definitions PB0 Output Inverted*/
#ifdef _MEGA64_INCLUDED_
#define LED_ERR_PORT      PORTC
#define LED_ERR_DDR       DDRC
#define LED_ERR_PIN       PINC
#define PIN_LED_ERR	      6
#else
#define LED_ERR_PORT      PORTB
#define LED_ERR_DDR       DDRB
#define LED_ERR_PIN       PINB
#define PIN_LED_ERR	      0
#endif

#define LED_ERR_INIT()    {LED_ERR_DDR.PIN_LED_ERR=1; LED_ERR_OFF();}

/*BUZZER PIN definitions PD6 Output*/
#define BUZ_PORT      PORTD
#define BUZ_DDR       DDRD
#define BUZ_PIN       PIND
#define PIN_BUZ	      6
#define BUZ_OFF()     BUZ_PORT.PIN_BUZ=0;
#define BUZ_TGL()     BUZ_PORT.PIN_BUZ=~BUZ_PORT.PIN_BUZ;
#define BUZ_INIT()    {BUZ_DDR.PIN_BUZ=1; BUZ_OFF();}


/****   Button definitions and initialization placed on main.c=ispUART.c  ****/
/*Start button definition ADC6 OFF=VCC ON=VCC/2  */
/*Up button definition ADC7 OFF=VCC ON=VCC/2  */
//#define BTN_PORT PORTB
//#define BTN_DDR  DDRB
//#define BTN_PIN  PINB
//#define PIN_BTN  7
//#define initBTN {BTN_DDR&=~(1<<PIN_BTN); BTN_PORT|=(1<<PIN_BTN);}//SET AS Input PULLUP
//#define getBTN  (BTN_PIN & (1<<PIN_BTN))

#define SOFTWARE_VER_1                'U'
#define SOFTWARE_VER_2                '2'
#define HARDWARE_VER_1                '0'
#define HARDWARE_VER_2                '2'
#define PROGRAMMER_TYPE                'S'     // Serial type
#define ACK                            0x0D     // 13D
#define NACK                        '?'        // 0x3F
#define YES                            'Y'
#define ESC                            0x1B
#define OPERATION_READ                0
#define OPERATION_WRITE                1
#define MEGA                        0 // mega/tiny AVR
#define CLASSIC                        1 // classic AVR
#define S89                            2 // 89Sxxx

#define AVR910_TX_DATA_SIZE            128
#define AVR910_REPORT_BLOCK_SIZE    128
#define AVR910_FIFO_SIZE			16
#define AVR910_FIFO_MASK			(AVR910_FIFO_SIZE - 1)
//#define puthexchars(i) putchar((i>>4)+(((i>>4)<0x0A)?'0':('A'-0x0A)));putchar((i&0x0f)+(((i&0x0f)<0x0A)?'0':('A'-0x0A)));
#define puthexchars(i) printf("0x%X",i);

extern UCHAR	g_txBuf[];
extern UCHAR    g_txRD, g_txWR;
//extern byte pageBuffer[];
extern UINT 	g_Addr;
extern ULONG	g_BlockSize;
extern UCHAR	g_Operation;
extern BOOL	g_BlockMode;
extern UCHAR	g_DeviceType;
extern UCHAR    g_DevicePollCode;
extern UCHAR	g_MemType;

void FIFO_Reset(void);
//void FIFO_Write(UCHAR data);
UCHAR FIFO_Read(void);

void AVR910_Init(void);
//void  AVR910_Command(UCHAR *data, UCHAR len);
void  AVR910_Command(void);
UCHAR AVR910_IsRxData(UCHAR **data);
void AVR910_ClearTXData(UCHAR bytesRead);
void FIFO_Write(UCHAR data);
void AVR910_ResetTXData(void);
//BOOL MyFlashPage(void);
void StandalonProg(void);
void AVR910_Standalone(flash char *commands);
void AVR910_SendRxToUart(void);
char getchar(void);
void putchar(char c);
UCHAR WriteBlockBin(void);
#endif//AVR910_H
