/*****************************************************************************
����: avr910.�

Creted by PROTTOSS
Mail to PROTTOSS@mail.ru

�������� AVR910

16.12.2006
******************************************************************************/
//#include "usbdrv.h"

#include "common.h"
#include "avr910.h"
#include "avr910devices.h"
#include "mycode.h"
//#include <stdlib.h>
#include <string.h>
#include <delay.h>
#include <stdio.h>
/*****************************************************************************
��������� �������������
******************************************************************************/



//#define LOW_SCK_DELEY()	delay_us(tmpimage.spidiv)//#asm("nop")//			//300 // ���������� ������ �� ��� ������� 20 ��� (25 + 25 ���)
#define FUSE_SCK_DELEY  delay_us(250)   // Fuse write clock low freq 100us+100us=200us 5kHz
//#define SPI_ON()	//SPCR = ((0 << SPR1) | (1 << SPR0) | (1 << SPE)| (1 << MSTR) )   //250kHz
//#define SPI_OFF()	//SPCR = 0

void LOW_SCK_DELEY()
{
	unsigned char us_cnt = spidiv;    
    while(1){       
        //delay_us(1);
        #asm("NOP")
        //#asm("NOP")
        //#asm("NOP")
        if(us_cnt!=0)
            break;
        us_cnt--;
    }
}


/*****************************************************************************
���������� ����������
******************************************************************************/
BOOL    g_IsFuseClock;
UCHAR	g_DeviceType;		// ��� ���������� ��

UCHAR	g_DeviceCode;
UCHAR	g_DevicePollCode;	// ��� �������� ��� ������ ������ ������� ����������
UINT 	g_Addr; 			// ����� ������ � ����������

// ��������� ������ ����������� �� USB ������
BOOL	g_BlockMode;		// ���� ������/������ � ������� ������
ULONG	g_BlockSize;		// ���������� ������ ����� //support 64k...
UCHAR	g_MemType;			// ��� ������ � ������� ������
UCHAR	g_WordCount;		// ���������� ����������� � �� ����/���� � ������� ������
UCHAR	g_Operation;		// ��� ����������� ������� ��������
UCHAR	g_Command;			// ����������� � ������� ������ �������
BOOL	g_NeedParams;		// ���� �������� ���������� �������

// ��������� FIFO ������ ������/������
UCHAR	g_FIFO[AVR910_FIFO_SIZE]; // ���������� �����
UCHAR	g_wrI;			// ������ ��� ������ � �����
UCHAR	g_rdI;			// ������ ��� ������ �� ������
UCHAR	g_FIFODataSize; // ������ ������ � ������

// ��������� ������������ � USB ������
UCHAR	g_txBuf[AVR910_TX_DATA_SIZE];	// ����� ������������ ������
UCHAR 	g_txWR;			// ������ ������ ������������ ������
UCHAR	g_txRD;			// ������ ������ �������� ������

// ������ ��������������
flash UCHAR SwID[] =
{
  	'A', 'V', 'R', ' ', 'I', 'S', 'P', 0 ,
};

/*****************************************************************************
��������� �������
******************************************************************************/

// ������� SPI-���������� AVR
#define AVR_L_BYTE					0x00
#define AVR_H_BYTE					0x08
#define AVR_RD_BYTE					0x20
#define AVR_WR_BYTE					0x40
#define AVR_PROG_EN()				Write123(0xAC, 0x53, 0x00)
#define AVR_CHIP_ERASE()			Write1234(0xAC, 0x80, 0x00, 0x00)
#define AVR_POLL_READY()			Write1234(0xF0, 0x00, 0x00, 0x00)
#define AVR_RD_FLASH_LB(addr)		Write1234(0x20, HIBYTE(addr), LOBYTE(addr), 0x00)
#define AVR_RD_FLASH_HB(addr)		Write1234(0x28, HIBYTE(addr), LOBYTE(addr), 0x00)
#define AVR_WR_FLASH_LB(addr, data)	Write1234(0x40, HIBYTE(addr), LOBYTE(addr), data)
#define AVR_WR_FLASH_HB(addr, data)	Write1234(0x48, HIBYTE(addr), LOBYTE(addr), data)
#define AVR_WR_FLASH_PAGE(addr)		Write1234(0x4C, HIBYTE(addr), LOBYTE(addr), 0x00)
#define AVR_RD_EEPROM(addr)			Write1234(0xA0, HIBYTE(addr), LOBYTE(addr), 0x00)
#define AVR_WR_EEPROM(addr, data)	Write1234(0xC0, HIBYTE(addr), LOBYTE(addr), data)
#define AVR_RD_LOCK()				Write1234(0x58, 0x00, 0x00, 0x00)
#define AVR_WR_LOCK(data)			Write1234(0xAC, data | 0xF9, 0x00, data)
#define AVR_RD_SIGN(addr)			Write1234(0x30, 0x00, addr, 0x00)
#define AVR_RD_FUSE()				Write1234(0x50, 0x00, 0x00, 0x00)
#define AVR_WR_FUSE(data)			Write1234(0xAC, 0xA0, 0x00, data)
#define AVR_RD_FUSE_H()				Write1234(0x58, 0x08, 0x00, 0x00)
#define AVR_WR_FUSE_H(data)			Write1234(0xAC, 0xA8, 0x00, data)
#define AVR_RD_FUSE_EXT()			Write1234(0x50, 0x08, 0x00, 0x00)
#define AVR_WR_FUSE_EXT(data)		Write1234(0xAC, 0xA4, 0x00, data)
#define AVR_RD_CALIBR(addr)			Write1234(0x38, 0xA8, addr, 0x00)

// ������� SPI-���������� 89S
#define S89_ADDR_HBYTE(h_addr)		((h_addr << 3) | ((h_addr >> 3) & 0x04))
#define S89_PROG_EN()				Write123(0xAC, 0x53, 0x00)
#define S89_CHIP_ERASE()			Write123(0xAC, 0x04, 0x00)
#define S89_RD_FLASH(addr)			Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x01, LOBYTE(addr), 0x00)
#define S89_WR_FLASH(addr, data)	Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x02, LOBYTE(addr), data)
#define S89_RD_EEPROM(addr)			Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x05, LOBYTE(addr), 0x00)
#define S89_WR_EEPROM(addr, data)	Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x06, LOBYTE(addr), 0x00)
#define S89_WR_LOCK(data)			Write123(0xAC, (data & 0xE0) | 0x07, 0x00)

// �������� ��� �������� ������ AVR
#define AVR_DELAY_RESET()		delay_ms(20)//__delay_cycles(300000) // > 20 ms
#define AVR_DELAY_WR_FUSE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define AVR_DELAY_WR_FLASH()	delay_ms(5)//__delay_cycles(150000)	// > 5 ms
#define AVR_DELAY_WR_EEPROM()	delay_ms(10)//__delay_cycles(150000)	// > 10 m�
#define AVR_DELAY_ERASE()		delay_ms(20)//__delay_cycles(300000)	// > 20 m�

// �������� ��� �������� ������ 89S
#define S89_DELAY_RESET()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define S89_DELAY_WR_FUSE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define S89_DELAY_WR_FLASH()	delay_us(2500)//__delay_cycles(30000)	// > 2,5 ms
#define S89_DELAY_WR_EEPROM()	delay_us(2500)//__delay_cycles(30000)	// > 2,5 ms
#define S89_DELAY_ERASE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms

#define DELAY_RESET()			delay_ms(20)//__delay_cycles(300000)  // > 20 ms
#define DELAY_WR_FUSE()			delay_ms(10)//__delay_cycles(150000) 	// > 10 ms
#define DELAY_WR_FLASH()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define DELAY_WR_EEPROM()		delay_ms(10)//__delay_cycles(150000)	// > 10 m�
#define DELAY_ERASE()			delay_ms(50)//__delay_cycles(300000)	// > 20 m�

//void AVR910_Init(void);
//void AVR910_Command(UCHAR *data, UCHAR len);
//UCHAR AVR910_IsRxData(UCHAR **data);
//void AVR910_ClearTXData(UCHAR bytesRead);
UCHAR ExSPI(UCHAR wr_data);
void CatchPorts(void);
void ReleasePorts(void);
UCHAR GetOneChar(void);
void PutOneChar(UCHAR data);
UCHAR Write123(UCHAR byte1, UCHAR byte2, UCHAR byte3);
UCHAR Write1234(UCHAR byte1, UCHAR byte2, UCHAR byte3, UCHAR byte4);
void PollData(UCHAR command, UCHAR data);



void CheckCommParams(UCHAR req_bytes);

void SetProgMode(void);
void GetDevCodes(void);
void SetDevType(void);
void SetLED(void);
void ClrLED(void);
void tx123rx4(void);
void tx1234rx4(void);
void GetBlockMode(void);
void GetSWID(void);
void GetSWVer(void);
void GetHWVer(void);
void GetAutoIncAddr(void);
void SetAddr(void);
void GetPROGType(void);
void ChipErase(void);
void WriteFuse(void);
void WriteHFuse(void);
void WriteEXTFuse(void);
void WriteLock(void);
void ReadLock(void);
void ReadFuse(void);
void ReadHFuse(void);
void ReadEXTFuse(void);
void ReadSignature(void);
void ReadCalibrationByte(void);
void LeaveProgMode(void);

void WriteProgLB(void);
void WriteProgHB(void);
void WriteProgOneLB(void);
void WriteProgOneHB(void);
void WritePage(void);
void ReadProg(UCHAR com_byte1, UCHAR com_byte2);
void WriteData(void);
void WriteOneData(void);
void ReadData(void);
void ReadBlockCommand(void);
void WriteBlockCommand(void);
void ReadBlock(void);
void WriteBlock(void);

/**********************
Standalone addons
***********************/
void ReBoot(void);

void AVR910_Standalone(flash char *commands)
{
   UCHAR i;
//   #ifdef PRINT_DEBUG
//   printf("\r\n Commands is: %p",commands);
//   while(tx_counter);
//   #endif
   AVR910_SendRxToUart();//clear send to uart tx buffer
   FIFO_Reset();
   for(i=0;i<strlenf(commands);i++)
        FIFO_Write(commands[i]);
   AVR910_Command();
//   #ifdef PRINT_DEBUG
//   printf("\r\n Response is: %c",g_txBuf[0]);
//   while(tx_counter);
//   #endif
}
/*****************************************************************************
�������������
******************************************************************************/
void AVR910_Init(void)
{
  	// ������������� ������ �����������
  	//LED_DDR 	|= (1 << PIN_LED_WR) | (1 << PIN_LED_RD)| (1 << PIN_LED_PASS);
    //LED_PORT	&= ~((1 << PIN_LED_WR) | (1 << PIN_LED_RD)| (1 << PIN_LED_PASS));
    LED_OK_INIT();
    LED_ERR_INIT();
    //MUX_SEL_INIT();
    ReleasePorts();
    // ������������� ���� low_sck
    //ISP_PORT	|= (1 << PIN_LOW_SCK); // ����������� �� ����� ������
    //ISP_DDR		&= ~(1 << PIN_LOW_SCK);	// ������������� ��� ����
    //ISP_PORT	|= (1 << PIN_SS); // ����������� �� ����� ������
    //ISP_DDR		|= (1 << PIN_SS);	// ������������� ��� ����
    // ������������� ������-������� TC1
//    TCCR1A = (0 << WGM11)| (0 << WGM10)|			   // CTC
//      		 (0 << COM1A1) | (1 << COM1A0);			   // Toggle OC1A on Compare Match
//	TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10) | // clkIO /1 (No prescaling)
//      		 (0 << WGM13)| (1 << WGM12);			   // CTC
//    OCR1A  = (UINT)5;
//
//    // ������� ������
//	//ISP_DDR |= (1 << PIN_CLOCK);
//    CLOCK_DDR |= (1 << PIN_CLOCK);
    // ������������� SPI ����
    //SPSR = 0x00; // ������� SPI2X

    // �������������� AVR910 ���������� ����������
  	g_Addr = 0;
    g_BlockMode = FALSE;
    g_NeedParams = FALSE;
    FIFO_Reset();
    g_IsFuseClock=FALSE;
    //ReleasePorts();
    //initBTN;
}

/*****************************************************************************
����� ���������
******************************************************************************/
//void AVR910_Command(UCHAR *data, UCHAR len)
void AVR910_Command(void)
{
	g_txWR = g_txRD = 0;	// ���������� ����� ��������

    // �������� ������ �� ������ USB � FIFO �������������
    //while(len--)
    	//FIFO_Write(*data++);


    // ��������� �������� �� USB ������
	while(0 < g_FIFODataSize)
    {
      	// �������� �������� ������ ������
      	if(TRUE == g_BlockMode)
    	{
          	// �������� �������� (������, ������)
          	if(OPERATION_WRITE == g_Operation)
            {
              	// ������� �������� ������
              	WriteBlock();
            	continue;
            }
            else
            {
              	// �������� ������ ��� �� �����������
              	// � �� �������� ������ - ��������� �������� ������ � ������� �����
              	g_BlockMode = FALSE;
            }
   		}

        // ���������, ������������ �� �� �������
        if(FALSE == g_NeedParams)
        	g_Command = GetOneChar();


      	// ����������� �������
      	switch(g_Command)
    	{
        	case ESC:
            break;
			case 'P': SetProgMode();
      		break;
			case 'a': GetAutoIncAddr();
			break;
			case 'A': SetAddr();
			break;
			case 'S': GetSWID();
        	break;
			case 'V': GetSWVer();
        	break;
			case 'v': GetHWVer();
        	break;
			case 'p': GetPROGType();
        	break;
            case 'T': SetDevType();
        	break;
            case 't': GetDevCodes();
        	break;
            case 'x': SetLED();
        	break;
			case 'y': ClrLED();
        	break;
			case ':': tx123rx4();
        	break;
			case '.': tx1234rx4();
        	break;
			case 'b': GetBlockMode();
			break;
        	case 'e': ChipErase();
        	break;
            case 'E': ReBoot(); // External commands
        	break;
            case 'L': LeaveProgMode();
        	break;
        	case 'l': WriteLock();
        	break;
			case 'f': WriteFuse();
        	break;
			case 'Q': ReadLock();
       		break;
            case 'F': ReadFuse();
       		break;
			case 'n': WriteHFuse();
       		break;
            case 'N': ReadHFuse();
       		break;
            case 'o': WriteEXTFuse();
       		break;
            case 'O': ReadEXTFuse();
       		break;
            case 's': ReadSignature();
        	break;
            case 'W': ReadCalibrationByte();
        	break;
            case 'd': ReadData();
        	break;
			case 'g': ReadBlockCommand();
			break;
			case 'B': WriteBlockCommand();
			break;
            case 'R': ReadProg(0x28, 0x20);
        	break;
        	case 'c': WriteProgOneLB();
            break;
			case 'C': WriteProgOneHB();
            break;
			case 'm': WritePage(); PutOneChar(ACK);// OK
        	break;
			case 'D': WriteOneData();
       		break;
			default: PutOneChar(NACK);
          	break;
    	}

        // ���� ���� ���������� ������� �� �������, ����
        if(TRUE == g_NeedParams)
          	break;
    }
}
void ReBoot(void)
{
    CHAR param;
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

    param = GetOneChar();
    switch(param){
       case 'D':PrintImage();
            break;
       //case 'R':SetValueReady("ready",'0');   
       //         Call_GetDir();
            break;
       case '.':
       {
           #asm("WDR")
            WDTCSR=0x18;
            WDTCSR=0x08|0x07;//2sec
            #ifdef  PRINT_DEBUG
            if(debug)printf("\r\nRebooting to bootlader...");
            #endif
            //delay_ms(1000);
            //((void(*)(void))(char *)0x3C00)(); // jump to 0x0000
       }
            break;
       default:
            break;
    }

}
/*****************************************************************************
���������� � ���������� data ����� ������ ������ � ���������� � ����������
g_txLen ���������� ������
******************************************************************************/
UCHAR AVR910_IsRxData(UCHAR **data)
{
  	*data = &g_txBuf[g_txRD]; // ������������� ����� ������ �������� ������
  	return (g_txWR - g_txRD); // ���������� ����� ������
}

/*****************************************************************************
����������� ����� ������ �� ���������� � USB ������
� ���������� bytesRead ���������� ���������� ����������� ������ �� ������
������.
******************************************************************************/
void AVR910_ResetTXData(void)
{
   g_txRD = g_txWR = 0;
}
void AVR910_ClearTXData(UCHAR bytesRead)
{
  	// ������� ����� �� ����������� ������ �
    // ��������� ����� ��������� ������
    g_txRD += bytesRead;

    // ���������, ���� �� �����
    if(g_txRD == g_txWR)
    {
      	// ���� ��, �� ���������� ����� � ���� �
    	// ��������� ������� ����� ������ � �������� ������
    	g_txRD = g_txWR = 0;
		if((TRUE == g_BlockMode) &&
           (OPERATION_READ == g_Operation))
    		ReadBlock();
    }
}
void AVR910_SendRxToUart(void)
{
    UCHAR *data;
    UCHAR bytesRead;
    while((bytesRead = AVR910_IsRxData(&data)) > 0){
       AVR910_ClearTXData(bytesRead);
       while(bytesRead--)
            #ifndef  USART_SPI 
            putchar(*data++);
            #else
            *data++;
            #endif
    }
}
/*****************************************************************************
����� ������� �� SPI
���������� � SPI ���� ������ wr_data � ���������� ����������� ��� ������ ������
******************************************************************************/
UCHAR ExSPI(UCHAR wr_data)
{
  	   UCHAR ddd;
    
    // ��������� ������� low_sck
//  	if(ISP_PIN & (1 << PIN_LOW_SCK)) // �� �������, ���������� SPI
//	{
//        SPI_ON();	// �������� SPI
//        SPDR = wr_data;	// ��������� SPI �������
//        while(!(SPSR & (1<<SPIF))){};	// ���� ��������� ��������
//		return SPDR;	// ���������� �������� ������
//    }
//    else // ����� ���������� SPI
    {
      	#ifdef USART_SPI
        {
          /* Wait for empty transmit buffer */
        while ( ( UCSR0A & (1<<UDRE0))==0 );
        /* Clear RXC0 before sends the data */
        ddd = UDR0;
        /* Put data into buffer, sends the data */
        UDR0 = wr_data;       
        /* Wait for data to be received */
        while ( (UCSR0A & (1<<RXC0))==0 );
        /* Get and return received data from buffer */
        //g_MyUDR=UDR0;
        //return g_MyUDR;
        return UDR0;
        }
        #else
        //SPI_OFF();	// ��������� ���������� SPI
      	UCHAR i;
        UCHAR rd_data=0;
        for(i = 0; i < 8; i++)
        {
          	// ���������� ������� ��� ������������� �����
          	if(0 != (wr_data & 0x80))
            	SET_MOSI();//ISP_PORT |= (1 << PIN_MOSI); // = 1
            else
            	CLR_MOSI();//ISP_PORT &= ~(1 << PIN_MOSI);	// = 0

            // �������������� ��������� ���
        	wr_data <<=1;

            // ��������� ��������� (������� �� ��������) ���
            // ������������ �����
            rd_data <<= 1;
            if(0 != READ_MISO())//(ISP_PIN & (1 << PIN_MISO)))
            	rd_data++; // ������������� ��������� ���, ���� ������� ����������� �������

			// ��������� ������� �� ������ SCK ��
            SET_SCK();//ISP_PORT |= (1 << PIN_SCK);	// �������
            if(g_IsFuseClock==TRUE)
                FUSE_SCK_DELEY;
            else
                LOW_SCK_DELEY(); // ��������
            CLR_SCK();//ISP_PORT &= ~(1 << PIN_SCK);	// ������
            if(g_IsFuseClock==TRUE)
                FUSE_SCK_DELEY;
            else
                LOW_SCK_DELEY(); // ��������
    	}
        return rd_data;  
        #endif
    }
}

/*****************************************************************************
���������� ����� � ������� ���������
******************************************************************************/
void CatchPorts(void)
{
  	// ���������� ������ SPI � ISP �������
  	//ISP_DDR  |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_RESET);

    // ����������� ������ � ������ MISO
    //ISP_PORT |= (1 << PIN_MISO);
    
    if(g_DeviceType == S89)
      	SET_RESET();
    else{
        #asm("nop");
        //delay_ms(30);
    }
    
    /* ReSet MSPI mode of operation and SPI data mode 0. */
    UCSR0C =0;
    /* Disable receiver and transmitter. */
    UCSR0B =0;
    UBRR0H = 0;
    //UBRR0L = 0;
    /* Setting the XCKn port pin as output, enables master
    mode. */
    DDRD.4=1;
    PORTD.4=1;//spi sck HIGH before reset HIGH
    delay_ms(1);
    DDRD.5=1;//set reset as output
    PORTD.5=0;//spi reset high
    delay_ms(1);
    PORTD.4=0;//spi sck low before reset low
    delay_ms(1);
    DDRD.1=1;//MOSI as out
    DDRD.0=0;//MISO as in
    PORTD.0=1;
    delay_ms(1);
    PORTD.0=0;
    /* Set MSPI mode of operation and SPI data mode 0. */
    UCSR0C =
    (1<<UMSEL01)|(1<<UMSEL00)|(0<<UCPHA0)|(0<<UCPOL0);
    /* Enable receiver and transmitter. */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set baud rate. */
    /* IMPORTANT: The Baud Rate must be set after the
    transmitter is enabled */
    //UBRR0L = 12;//baud;
    //UBRR0L = 255;//baud;
    g_MyUDR=UDR0;
    
    PORTD.5=1;//spi reset low
    //ISP_ON();
    
    // ��������� SPI � ������������� ��������
    //SPI_ON();

    // ��������� RESET � �������� ���������
    if(g_DeviceType == S89)
      	SET_RESET();
   //else
      	//CLR_RESET();
    AVR_DELAY_RESET();	// ��������

}

/*****************************************************************************
��������� �����
******************************************************************************/
void ReleasePorts(void)
{
  	//CatchPorts();
    // ��������� SPI
    //ISP_OFF();    
    // ��������� ������ SPI �� ISP �������
  	//ISP_DDR &=  ~((1 << PIN_SCK) | (1 << PIN_MISO) | (1 << PIN_MOSI) | (1 << PIN_RESET));
    //ISP_PORT &= ~((1 << PIN_SCK) | (1 << PIN_MISO) | (1 << PIN_MOSI) | (1 << PIN_RESET));
}

/*****************************************************************************
���������� ������ �� ������ ��������� USB
******************************************************************************/
UCHAR GetOneChar(void)
{
  	/*if(g_rxLen > 0)
    {
      	g_rxLen--;
    	return *g_rxBuf++;
    }

    return 0;*/
 	return FIFO_Read();
}

/*****************************************************************************
���������� ������ � ����� ����������� USB
******************************************************************************/

void PutOneChar(UCHAR data)
{
  	if(g_txWR < AVR910_TX_DATA_SIZE)
      	g_txBuf[g_txWR++] = data;
}

/*****************************************************************************
���������� ��������������� 3 ����� � SPI � ���������� ��������� �� ������ 3-��
******************************************************************************/
UCHAR Write123(UCHAR byte1, UCHAR byte2, UCHAR byte3)
{
  	ExSPI(byte1);
    ExSPI(byte2);    
    #ifdef USART_SPI
    g_MyUDR=ExSPI(byte3);
    return g_MyUDR;
    #endif        
    return ExSPI(byte3);
}

/*****************************************************************************
���������� ��������������� 4 ����� � SPI � ���������� ��������� �� ������ 4-��
******************************************************************************/
UCHAR Write1234(UCHAR byte1, UCHAR byte2, UCHAR byte3, UCHAR byte4)
{
  	ExSPI(byte1);
    ExSPI(byte2);
    ExSPI(byte3);
    #ifdef USART_SPI
    g_MyUDR=ExSPI(byte4);
    return g_MyUDR;
    #endif
    return ExSPI(byte4);
}

/*****************************************************************************
���� � ����� ����������������
******************************************************************************/
void SetProgMode(void)
{
    UCHAR progOK=0;
    UCHAR c;
    UCHAR err = ACK;
    UCHAR ctrl_byte;
    LED_RD_ON();
    LED_WR_ON();
    g_IsFuseClock=TRUE;
    ReleasePorts(); 	// ��������� �����
    // ��������� ��� ����������
    if(S89 == g_DeviceType) //89S
    {
      	//ReleasePorts(); // ��������� �����
		CatchPorts();	// ���������� ����� � ISP
   		S89_DELAY_RESET();	// ��������

        // �������� ������� ����� � ����� ����������������
        S89_PROG_EN();
        progOK=1;
    }
    else
    {
      	// ���������������� � ������������ �����
        #ifdef PRINT_LCD
        lcd_putsf_row(3,"Echo53:");
        #endif
        for(c = 0; c < 3; c++)
        {
            
            CatchPorts();		// ���������� ����� � ISP
            //AVR_DELAY_RESET();	// ��������

            // 32 ����� ������� �������������������� � ��������������� ��
        
           	// �������� ������� ��� � ����� ����������������
			// ��� ������� 3-�� ����� � SPI ��� ������ ������� ��� ���������� �������
          	ctrl_byte = AVR_PROG_EN();
            ExSPI(0x00);
            #ifdef PRINT_LCD
            lcd_puts_hex(g_MyUDR);
            #endif
            // �������� ������ ����������� ���� - ������������� ���������
            if(0x53 == ctrl_byte){
           		progOK=1;
                break;//goto m1;
           	}
            // ���� ��� ��������� ��������, �� �������� ��� ������ ������� �����
          	/*
            SET_SCK();
    		delay_us(50);//__delay_cycles(600); // �������� 50 ���
			CLR_SCK();
    		delay_us(50);//__delay_cycles(600);
            */
        }
        // ���� �� �� ��������������������,
        // �������, ��� ���������� �� �������
        if(progOK==0){
            ReleasePorts(); // ��������� �����
            err = NACK; // �������� ��� ������
            if(CanShow)
                delay_ms(500);
        }
        else
            delay_ms(100);
    }
//:m1
    PutOneChar(err);
    if(progOK==0){
        LED_WR_OFF();//CLR_LED_WR();
        LED_RD_OFF();//CLR_LED_RD();
    }
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
���������� ���� �������������� ���������
******************************************************************************/
void GetDevCodes(void)
{
  	UCHAR code, i = 0;
    do
    {
      	code = DeviceDesc[i];
      	PutOneChar(code);
      	i += 2;
	}
    while(0!= code);
}

/*****************************************************************************
����� ���� ������������� ����������������
������������� ������ � ������� ���������� ���������, ���� ���������� ���
����������� �� ����� ������
******************************************************************************/
void SetDevType(void)
{
  	UCHAR code, err = NACK, i = 0, request;

    // ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

    request = GetOneChar();

    // ���������� ��� ���� �� � ������� ��������� � ����������
    // ���� ��� �� �� ����� 0
	while(0 != (code = DeviceDesc[i]))
    {
      	// ���� ��� ������, ���� �������������� ����������,
      	// ��������� ������ ��� ����������
      	if(request == code)
        {
          	// ������������� ��� ��
          	g_DeviceCode = DeviceDesc[i];
          	g_DevicePollCode = DeviceDesc[i + 1];
            err = ACK;
            if(0x7F < g_DeviceCode)
            	g_DeviceType = S89;
            else
              	if((g_DevicePollCode == 0x7F) ||
                   (g_DevicePollCode == 0xFF))
                	g_DeviceType = CLASSIC;
                else
                  	g_DeviceType = MEGA;
        }

        i += 2;
    }

    PutOneChar(err);
}

/*****************************************************************************
������ ��������� ����������������
������� ������������
******************************************************************************/
void SetLED(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	GetOneChar();
	PutOneChar(ACK);
}

/*****************************************************************************
�������� ��������� ����������������
������� ������������
******************************************************************************/
void ClrLED(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	GetOneChar();
	PutOneChar(ACK);
}

/*****************************************************************************
������������� �������: ����� � SPI 3 ����� ������ � ���������� � �����
�������� ��������� �� ������ 4-�� (0x00)
******************************************************************************/
void tx123rx4(void)
{
  	UCHAR b1,b2,b3;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(3);
    if(TRUE == g_NeedParams)
      	return;
    
    LED_RD_ON();
    LED_WR_ON();
  	
    b1 = GetOneChar();
    b2 = GetOneChar();
    b3 = GetOneChar();
  	PutOneChar(Write1234(b1, b2, b3, 0x00));
    PutOneChar(ACK);
    DELAY_WR_FLASH();
    
    LED_RD_OFF();
    LED_WR_OFF();    
}

/*****************************************************************************
������������� �������: ����� � SPI 3 ����� ������ � ���������� � �����
�������� ��������� �� ������ 4-�� (d4)
******************************************************************************/
void tx1234rx4(void)
{
  	UCHAR b1,b2,b3,b4;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(4);
    if(TRUE == g_NeedParams)
      	return;
    
    LED_RD_ON();
    LED_WR_ON();
  	
    b1 = GetOneChar();
    b2 = GetOneChar();
    b3 = GetOneChar();
    b4 = GetOneChar();
    PutOneChar(Write1234(b1, b2, b3, b4));
  	PutOneChar(ACK);
    DELAY_WR_FLASH();

    LED_RD_OFF();
    LED_WR_OFF();
}

/*****************************************************************************
������ � ������ ��������, ������� ����
******************************************************************************/
void WriteProgLB(void)
{
  	UCHAR data;
    // �������� ��������� ������
  	LED_WR_ON();

  	// �������� ���� ������
	data = GetOneChar();

    // ����� ����
    if(g_DeviceType == S89)
    	S89_WR_FLASH(g_Addr, data);
    else
    	AVR_WR_FLASH_LB(g_Addr, data);

    // ��������� ������ �����
	PollData(0x20, data);

    // �������������� �����, ���� 89S
    if(S89 == g_DeviceType)
      	g_Addr++;

    // ����� ��������� ������
    LED_WR_OFF();
}

/*****************************************************************************
������ � ������ ��������, ������� ����
******************************************************************************/
void WriteProgHB(void)
{
  	UCHAR data;
    if(S89 != g_DeviceType)
    {
      	// �������� ��������� ������
  		LED_WR_ON();

      	// �������� ������� ���� ������
		data = GetOneChar();

      	// ����� � SPI ������� ����
        AVR_WR_FLASH_HB(g_Addr, data);

    	// ��������� ������ �����
		PollData(0x28, data);

    	// �������������� �����
    	g_Addr++;

        // ����� ��������� ������
    	LED_WR_OFF();
    }
}

/*****************************************************************************
��������� ������ � ������ ��������, ������� ����
******************************************************************************/
void WriteProgOneLB(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteProgLB();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
��������� ������ � ������ ��������, ������� ����
******************************************************************************/
void WriteProgOneHB(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteProgHB();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
��������� ������ � ������ ������
******************************************************************************/
void WriteOneData(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteData();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
�������� �������� � ����������� ������ ����������������
******************************************************************************/
void PollData(UCHAR command, UCHAR data)
{
  	// ��������� �������
  	if(g_DevicePollCode & 0x0F)
    {
      	// ���� ������ ��������� � ����� ��������, �� �� ����� ��������� ������
      	if(g_DevicePollCode == data)
        	DELAY_WR_FLASH();

        else
        {
          	// 256 ������ ��������
          	UCHAR cycle = 0xFF;
          	do
            {
              	// ������ ����������
              	if(S89 == g_DeviceType) // 89S
                {
                  	if(data == S89_RD_FLASH(g_Addr))
                    	break; // OK
                }
                else // AVR
                {
                  	if(data == Write1234(command, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00))
                		break; // OK
                }
            }
            while(--cycle);

            // ���� �������� ��� ��� � �� ������� (((
            if(0 == cycle)
            	DELAY_WR_FLASH();
        }
    }
}

/*****************************************************************************
������ �������� � ������ ��������
******************************************************************************/
void WritePage(void)
{
  	// ��������� ������� Write Programm Memory Page
  	if(MEGA == g_DeviceType) //mega/tiny AVR
    {
      	UINT page_addr = g_Addr;
        if(TRUE == g_BlockMode)
        	page_addr--;
  		Write1234(0x4C, HIBYTE(page_addr), LOBYTE(page_addr), 0x00);
		DELAY_WR_FLASH();
    }
}

/*****************************************************************************
������ ������ ��������
******************************************************************************/
void ReadProg(UCHAR com_byte1, UCHAR com_byte2)
{
  	// �������� ��������� ������
  	LED_RD_ON();

  	if(S89 != g_DeviceType)// AVR - ������ �����
    {
      	PutOneChar(Write1234(com_byte1, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
        PutOneChar(Write1234(com_byte2, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
    }
    else//S89 - ������ ����
    {
      	PutOneChar(S89_RD_FLASH(g_Addr));
    }

    // ������������� �����
	g_Addr++;

    // ����� ��������� ������
  	LED_RD_OFF();
}

/*****************************************************************************
������ � ������ ������ (EEPROM)
******************************************************************************/
void WriteData(void)
{
  	UCHAR data;
    // �������� ��������� ������
  	LED_WR_ON();

	// �������� ���� ������
	data = GetOneChar();

    // ��������� ��� ����������
    if(S89 != g_DeviceType) // AVR
    	AVR_WR_EEPROM(g_Addr, data);
    else // 89S
    	if(0x86 == g_DeviceCode)// EEPROM ���� ������ � 8252
           	S89_WR_EEPROM(g_Addr, data);

    DELAY_WR_EEPROM();

    // �������������� �����
    g_Addr++;

    // ����� ��������� ������
    LED_WR_OFF();
}

/*****************************************************************************
������ ������ ������ (EEPROM)
******************************************************************************/
void ReadData(void)
{
  	// �������� ��������� ������
  	LED_RD_ON();

  	// ��������� ��� ����������
    if(S89 != g_DeviceType)// AVR
    	PutOneChar(Write1234(0xA0, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
	else
    	if(0x86 == g_DeviceCode)//S89 EEPROM ������� ������ � 8252
        	PutOneChar(Write123(((HIBYTE(g_Addr) << 3) | 0x05), LOBYTE(g_Addr), 0x00));

    // ������������� �����
	g_Addr++;

    // ����� ��������� ������
  	LED_RD_OFF();
}

/*****************************************************************************
������� ������ �������� � ������ (EEPROM) ����
******************************************************************************/
void ChipErase(void)
{
  	// �������� ��������� ������
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
  	// �������� ������� Chip Erase, �������� � OK
  	if(g_DeviceType == S89)
      	S89_CHIP_ERASE();
    else
      	AVR_CHIP_ERASE();
	DELAY_ERASE();
    PutOneChar(ACK);

    // ����� ��������� ������
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
������ ����� ����������
******************************************************************************/
void WriteLock(void)
{
  	UCHAR data;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// �������� ��������� ������
  	LED_WR_ON();
    g_IsFuseClock=TRUE;

	// �������� ���� ������, ����� lock bits, �������� � OK
	data = GetOneChar();
	if(g_DeviceType == S89)
      	S89_WR_LOCK(data);
    else
      	AVR_WR_LOCK(data);
	DELAY_WR_FUSE();
    PutOneChar(ACK);

    // ����� ��������� ������
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
������ ��� ����������
******************************************************************************/
void WriteFuse(void)
{
  	UCHAR err,data;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// �������� ��������� ������
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// �������� ���� ������, ����� fuse bits, �������� � OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // ����� ��������� ������
    LED_WR_OFF(); 
    g_IsFuseClock=FALSE;
}
void WriteHFuse(void)
{
  	UCHAR err,data;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// �������� ��������� ������
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// �������� ���� ������, ����� fuse bits, �������� � OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE_H(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // ����� ��������� ������
    LED_WR_OFF(); 
    g_IsFuseClock=FALSE;
}
void WriteEXTFuse(void)
{
  	UCHAR err,data;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// �������� ��������� ������
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// �������� ���� ������, ����� fuse bits, �������� � OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE_EXT(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // ����� ��������� ������
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
������ ����� ���������� � ���������� (������� ����)
******************************************************************************/
void ReadFuse(void)
{
  	// �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE());

    // ����� ��������� ������
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
������ ����� ���������� � ���������� (������� ����)
******************************************************************************/
void ReadLock(void)
{
  	// �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_LOCK());

    // ����� ��������� ������
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
������ ����� ���������� � ���������� (������� ����)
******************************************************************************/
void ReadHFuse(void)
{
  	// �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE_H());

    // ����� ��������� ������
  	LED_RD_OFF(); 
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
������ ����� ���������� � ���������� (������� ����)
******************************************************************************/
void ReadEXTFuse(void)
{
  	// �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE_EXT());

    // ����� ��������� ������
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
������ ��������� ����
******************************************************************************/
void ReadSignature(void)
{
  	UCHAR addr;
    // �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������ ��������������� Vendor, Family, Number
    addr = 0;
    #ifdef PRINT_LCD
    lcd_putsf_row(3,"Sign:0x");
    #endif
    do
    {
    	//PutOneChar((g_DeviceType == S89)?0xFF:(AVR_RD_SIGN(addr)));
        PutOneChar(AVR_RD_SIGN(addr));//g_txBuf[g_txWR++] = data;
        #ifdef PRINT_LCD
        lcd_puts_hex(g_MyUDR);
        #endif
        addr++;
    }
    while(addr<=2);
    if(CanShow)
        delay_ms(200);
    // ����� ��������� ������
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}

void ReadCalibrationByte(void)
{
  	UCHAR addr;
    // �������� ��������� ������
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // ������ ��������������� Vendor, Family, Number
    addr = 0;// case when 4 calibration bytes 
    do
    {
    	//PutOneChar((g_DeviceType == S89)?0xFF:(AVR_RD_SIGN(addr)));
        PutOneChar(AVR_RD_CALIBR(addr));//g_txBuf[g_txWR++] = data;
        addr++;
    }
    while(addr<=3); // case when 4 calibration bytes
    // ����� ��������� ������
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
����� �� ������ ����������������
******************************************************************************/
void LeaveProgMode(void)
{
    ReleasePorts();
  	PutOneChar(ACK);
}

/*****************************************************************************
�������� ��������� �������� ������ ������/������
******************************************************************************/
void GetBlockMode(void)
{
  	PutOneChar(YES);
    PutOneChar((UCHAR)(AVR910_REPORT_BLOCK_SIZE >> 8));
    PutOneChar(AVR910_REPORT_BLOCK_SIZE & 0xFF);
}

/*****************************************************************************
���������� ������������� ��
******************************************************************************/
void GetSWID(void)
{

    UCHAR flash *ptr = SwID;
  	while(0 != *ptr)
      	PutOneChar(*ptr++);
}

/*****************************************************************************
���������� ������ ��
******************************************************************************/
void GetSWVer(void)
{
  	PutOneChar(SOFTWARE_VER_1);
    PutOneChar(SOFTWARE_VER_2);
}

/*****************************************************************************
���������� ������ �����
******************************************************************************/
void GetHWVer(void)
{
  	PutOneChar(HARDWARE_VER_1);
    PutOneChar(HARDWARE_VER_2);
}

/*****************************************************************************
���������� ��������� �������������� ������
******************************************************************************/
void GetAutoIncAddr(void)
{
  	PutOneChar(ACK);
}

/*****************************************************************************
���������� ��� �������������
******************************************************************************/
void GetPROGType(void)
{
  	PutOneChar(PROGRAMMER_TYPE);
}

/*****************************************************************************
���������� �����
******************************************************************************/
void SetAddr(void)
{
  	// ���������, ���� �� ������ ��� �������
    CheckCommParams(2);
    if(TRUE == g_NeedParams)
      	return;

  	g_Addr = MAKEUINT(GetOneChar(), GetOneChar());
    PutOneChar(ACK);
}

/*****************************************************************************
��������� ������� �������� ������
******************************************************************************/
void ReadBlockCommand(void)
{
  	UCHAR size_h,size_l;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(3);
    if(TRUE == g_NeedParams)
      	return;

  	// �������� ������ ����� � ��� ������
  	size_h = GetOneChar();
    size_l = GetOneChar();
    g_BlockSize = ((UINT)(size_h << 8)) | ((UINT)(size_l));
	g_MemType = GetOneChar();

    g_Operation = OPERATION_READ;
    g_BlockMode = TRUE;

    // ��������� ��� ����������
    if(S89 != g_DeviceType)
       	if('F' == g_MemType)// � ������ �������� AVR ����� �������
      		g_BlockSize /= 2;
	ReadBlock();
}

/*****************************************************************************
������� ������ ������
******************************************************************************/
void ReadBlock(void)
{
  	// ��������� ��� ������
    if('F' == g_MemType)//FLASH
    {
      	// ��������� ����� �������
      	while(g_txWR < AVR910_TX_DATA_SIZE)
        {
          	ReadProg(0x20, 0x28);
            if(0 == --g_BlockSize)
    		{
      			g_BlockMode = FALSE;
                break;
    		}
        }

    }
    else// EEPROM
    {
      	// ��������� ����� �������
      	while(g_txWR < AVR910_TX_DATA_SIZE)
        {
          	ReadData();
            if(0 == --g_BlockSize)
    		{
      			g_BlockMode = FALSE;
                break;
    		}
        }
    }
}

/*****************************************************************************
��������� ������� ������� ������
******************************************************************************/
void WriteBlockCommand(void)
{
  	UCHAR size_h,size_l,size_e=1;
    // ���������, ���� �� ������ ��� �������
    CheckCommParams(4);
    if(TRUE == g_NeedParams){// if less than 4 bytes waiting==true -> for big size
      CheckCommParams(3);
      size_e=0;
      if(TRUE == g_NeedParams)
        return;
    }

  	// �������� ������ ����� � ��� ������
  	if(size_e)
        size_e = GetOneChar();
    size_h	= GetOneChar();
    size_l	= GetOneChar();
    g_BlockSize		= ((ULONG)((ULONG)size_e << 16)) | ((UINT)(size_h << 8)) | ((UINT)(size_l));
    g_MemType		= GetOneChar();
	g_BlockMode		= TRUE;
    g_Operation		= OPERATION_WRITE;
    g_WordCount 	= 0;

    // ��������� ��� ����������
    if(S89 != g_DeviceType)
       	if('F' == g_MemType)// � ������ �������� AVR ����� �������*/
      		g_BlockSize /= 2;
}

/*****************************************************************************
������� ����� ������ ������
******************************************************************************/
void WriteBlock(void)
{
  	// ��������� ��� ������
  	if('E' == g_MemType)// EEPROM
    {
    	//while(g_rxLen)
      	while(0 < g_FIFODataSize)
    	{
    		WriteData();
            g_BlockSize--;
		}
	}

	if('F' == g_MemType)//Flash
    {
       	//while(g_rxLen)
      	while(0 < g_FIFODataSize)
       	{
       		WriteProgLB();
     		WriteProgHB();
            g_BlockSize--;

            // ��������� ������������� ��������
            if(MEGA == g_DeviceType)
            {
               	g_WordCount++;
              	if(g_WordCount == g_DevicePollCode)
                {
                  	WritePage();
                    g_WordCount = 0;
                }
            }
		}
	}

    // ���� ���� ��������
    if(0 == g_BlockSize)
    {
      	// � �������� �� ����������
      	if(g_WordCount != 0)

          	// ������ ����� ������ ������,
          	// ���� ������ � ������ �������� �� ��������� ��������
          	if('F' == g_MemType)//Flash
      			WritePage();

      	// ��������, ��� ���� ������
      	g_BlockMode = FALSE;
    	PutOneChar(ACK);
    }
}
UCHAR WriteBlockBin(void)
{
        UCHAR chk_data,next_data,WordCount,adr_ext,adr_hi,adr_lo,adr_ext_b,adr_hi_b,adr_lo_b,readCounter,boot=0;
        UINT bc=0,bootPages,appCS,bootCS;
        
        //copy intel hex to page buffer
        LastOffset=2000L; //16bit addresses and 16bit data counter, coded from 1000->1023 24bytes
        if(fileOpen()==FALSE)
            return FALSE;
        if(SetBinFileOffset(LastOffset)==FALSE)//if(SetFlashArea()==0)
            return FALSE;
        if(fileRead()==FALSE)
            return FALSE;
        bc=0;
        adr_ext = buffer[bc++];
        adr_hi = buffer[bc++];
        adr_lo = buffer[bc++];
        g_BlockSize = buffer[bc++];
        g_BlockSize = (g_BlockSize<<8) | buffer[bc++];//app pages counter
        appCS = buffer[bc++];
        appCS = (appCS<<8) | buffer[bc++];   
        adr_ext_b = buffer[bc++];
        adr_hi_b = buffer[bc++];
        adr_lo_b = buffer[bc++];
        bootPages =buffer[bc++];
        bootPages = (bootPages<<8) | buffer[bc++];//app pages counter
        if(bootPages!=0)
            boot=1;
        bootCS = buffer[bc++];
        bootCS = (bootCS<<8) | buffer[bc++];
        bc++; 
        g_DevicePollCode=buffer[bc++];
        WordCount=g_DevicePollCode;
        if(g_BlockSize==0){
            appCS=bootCS;
            bootCS=0;
            g_Addr=0;
            g_BlockSize=bootPages;
            bootPages=0;
            adr_ext=adr_ext_b;
            adr_hi=adr_hi_b;
            adr_lo=adr_lo_b;
            boot=0;
        }
        /*start debug
        #ifdef PRINT_LCD
        lcd_putsf_row(1,"");
        lcd_puts_hex(adr_hi); 
        lcd_puts_hex(adr_lo);
        lcd_puts_hex(g_BlockSize>>8);
        lcd_puts_hex(g_BlockSize);
        lcd_puts_hex(appCS>>8);
        lcd_puts_hex(appCS);
        lcd_puts_hex(g_DevicePollCode);
        #endif
        end debug*/   
        bc=0; 
        LastOffset=2048L;
        g_Addr=0;
        if(readPageBin((UINT)g_DevicePollCode<<1,pageBuffer)==FALSE)
            return FALSE;
        //#asm("cli");
        while(g_BlockSize>0)
       	{
       		if(boot!=0&&bootPages==0)
            {
                adr_ext=adr_ext_b;
                adr_hi=adr_hi_b;
                adr_lo=adr_lo_b;
                boot=0; 
                /*
                #ifdef PRINT_LCD
                lcd_putsf_row(2,"");
                lcd_puts_hex(adr_hi); 
                lcd_puts_hex(adr_lo);
                lcd_puts_hex(g_BlockSize>>8);
                lcd_puts_hex(g_BlockSize);
                lcd_puts_hex(appCS>>8);
                lcd_puts_hex(appCS);
                lcd_puts_hex(g_DevicePollCode);
                #endif
                */
            }
            //WriteProgLB();
     		//AVR_WR_FLASH_LB(g_Addr, buffer[nbytesCounter++]);
            /* Wait for empty transmit buffer */
            while ( ( UCSR0A & (0x20))==0 );
            UDR0 = 0x40;
            while ( ( UCSR0A & (0x20))==0 );
            UDR0 = 0;
            while ( ( UCSR0A & (0x20))==0 );
            UDR0 = adr_lo;
            next_data=pageBuffer[bc++];
            while ( ( UCSR0A & (0x20))==0 );
            UDR0 = next_data;
            //--BlockSize;
            while ( ( UCSR0A & (0x20))==0 ); 
            UDR0 = 0x48;
            --WordCount;                                     
            while ( ( UCSR0A & (0x20))==0 ); 
            UDR0 = 0;
            while ( ( UCSR0A & (0x20))==0 ); 
            UDR0 = adr_lo++; 
            next_data=pageBuffer[bc++];
            while ( ( UCSR0A & (0x20))==0 ); 
            UDR0 =   next_data;            
            if(WordCount==0)
            {
                //WritePage();
                while ( ( UCSR0A & (0x20))==0 );
                UDR0 = 0x4C;
                while ( ( UCSR0A & (0x20))==0 );
                UDR0 = adr_hi;
                while ( ( UCSR0A & (0x20))==0 );
                UDR0 = adr_lo-1;
                while ( ( UCSR0A & (0x20))==0 );
                UDR0 = 0;
                while ( ( UCSR0A & (0x20))==0 ); 
                chk_data=0xff;
                bc-=(UINT)(g_DevicePollCode<<1);
                adr_lo-=g_DevicePollCode;
                //g_BlockSize-=g_DevicePollCode; 
                g_BlockSize--; 
                //copy intel hex to page buffer
                if(g_BlockSize > 0 || bootPages>0){//if not end of file then read next part to write
                    if(g_BlockSize==0){
                       if(g_Addr!=appCS){
                            #ifdef PRINT_LCD
                            lcd_putsf_row(3,"cs r");
                            lcd_puts_hex(g_Addr>>8);
                            lcd_puts_hex(g_Addr);
                            lcd_putsf(" e"); 
                            lcd_puts_hex(appCS>>8);
                            lcd_puts_hex(appCS);
                            #endif 
                            return FALSE;
                       }
                       appCS=bootCS;
                       g_Addr=0;
                       g_BlockSize=bootPages;
                       bootPages=0;
                       //boot=0; 
                    }
                    if(bc!=0){
                            if(readPageBin((UINT)g_DevicePollCode<<1,pageBuffer)==FALSE)
                                return FALSE;
                    }
                    else{    
                        if(readPageBin((UINT)g_DevicePollCode<<1,&pageBuffer[(UINT)(g_DevicePollCode<<1)])==FALSE)
                            return FALSE;
                    }     
                }
                else{
                   delay_ms(1);
                }
                while(WordCount != g_DevicePollCode){
                    if(chk_data==0xff){                       
                        if(pageBuffer[bc]!=0xff){
                            chk_data=pageBuffer[bc];
                            while ( ( UCSR0A & (0x80)) )
                                next_data=UDR0;
                            //next_data=UDR0; 
                            readCounter=20;//10us=1spi read 250*10us=2.5ms check counter
                            //do
                            while((--readCounter)!=0)
                            {
                                while ( ( UCSR0A & (0x20))==0 );
                                UDR0 =bc%2? 0x28:0x20;
                                while ( ( UCSR0A & (0x80)) ==0);
                                next_data= UDR0;
                                while ( ( UCSR0A & (0x20))==0 ); 
                                UDR0 = adr_hi;
                                while ( ( UCSR0A & (0x80)) ==0);
                                next_data=UDR0;
                                while ( ( UCSR0A & (0x20)) ==0 );
                                UDR0 = adr_lo;
                                while ( ( UCSR0A & (0x80)) ==0);
                                next_data=UDR0;
                                while ( ( UCSR0A & (0x20)) ==0 );
                                UDR0 = 0x0;
                                while ( ( UCSR0A & (0x80)) ==0);
                                next_data= UDR0;
                                if(next_data==chk_data){
                                    /*
                                    if(g_BlockSize==0){
                                        #ifdef PRINT_LCD 
                                        lcd_putsf_row(3,"0cd=");
                                        lcd_puts_hex(chk_data);
                                        lcd_putsf(" nd=");
                                        lcd_puts_hex(next_data); 
                                        while(1);
                                        #endif
                                    }
                                    */
                                    break;
                                }
                                else{
                                    delay_ms(1);
                                    /*
                                    #ifdef PRINT_LCD 
                                    lcd_putsf_row(3,"cd=");
                                    lcd_puts_hex(chk_data);
                                    lcd_putsf(" nd=");
                                    lcd_puts_hex(next_data); 
                                    while(1);
                                    #endif 
                                    */
                                }
                                //delay_ms(10);
                                /*
                                if((--readCounter)==0){ 
                                    break;
                                    }
                                */
                                /*
                                #ifdef PRINT_LCD 
                                lcd_putsf_row(3,"cd=");
                                lcd_puts_hex(chk_data);
                                lcd_putsf(" nd=");
                                lcd_puts_hex(next_data);
                                #endif   
                                */
                            }                    
                            //while(next_data!=chk_data); 
                            
                        }
                    }
                    if(bc%2){
                        adr_lo++;
                        WordCount++;
                    }
                    bc++;    
                }
                if(chk_data==0xff){
                    DELAY_WR_FLASH();
                    DELAY_WR_FLASH();
                    //delay_ms(10);
                }
                //adr_hi=Addr>>8;
                WordCount=0;
                adr_lo-=g_DevicePollCode;
                bc-=(UINT)(g_DevicePollCode<<1);
                while ( ( UCSR0A & (0x80)) )
                    next_data=UDR0;
                while ( ( UCSR0A & (0x20))==0 );
                UDR0 = 0x20;
                while(WordCount != g_DevicePollCode){
                    
                    while ( ( UCSR0A & (0x20))==0 ); 
                    UDR0 = adr_hi;
                    while ( ( UCSR0A & (0x80)) ==0);
                    next_data=UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = adr_lo;
                    while ( ( UCSR0A & (0x80)) ==0);
                    next_data=UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x0;
                    while ( ( UCSR0A & (0x80)) ==0);
                    next_data= UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x28;
                    chk_data=pageBuffer[bc++];
                    while ( ( UCSR0A & (0x80))==0);
                    next_data= UDR0;                    
                    if(next_data!=chk_data){
                        /*
                        while(1){
                            lcd_putsf_row(0,"Lo Addr");
                            lcd_puts_hex(adr_hi);
                            lcd_puts_hex(adr_lo);
                            lcd_putsf_row(1,"R");
                            lcd_puts_hex((next_data));
                            lcd_putsf_row(2,"W");
                            lcd_puts_hex(chk_data); 
                        }
                        */
                        return FALSE;    
                    }
                    while ( ( UCSR0A & (0x20))==0 ); 
                    UDR0 = adr_hi;
                    ++WordCount;
                    while ( ( UCSR0A & (0x80))==0 );
                    next_data=UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = adr_lo++;
                    while ( ( UCSR0A & (0x80))==0 );
                    next_data=UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x0;
                    if(!adr_lo)
                        ++adr_hi;
                    while ( ( UCSR0A & (0x80))==0 );
                    next_data=UDR0;
                    while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x20;
                    chk_data=pageBuffer[bc++];
                    while ( ( UCSR0A & (0x80))==0);
                    next_data= UDR0;
                    if(next_data!=chk_data){
                        /*
                        while(1){
                            lcd_putsf_row(0,"Hi Addr ");
                            if(!adr_lo){
                                adr_lo=0xff;
                                adr_hi--; 
                            }
                            lcd_puts_hex(adr_hi);
                            lcd_puts_hex(adr_lo);
                            lcd_putsf_row(1,"R");
                            lcd_puts_hex((next_data));
                            lcd_putsf_row(2,"W");
                            lcd_puts_hex(chk_data); 
                        }
                        */
                        return FALSE;
                    }                   
                }
                while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x20;
                while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x20;
                while ( ( UCSR0A & (0x20))==0 );
                    UDR0 = 0x20;
                if(bc>=(UINT)g_DevicePollCode<<2)
                   bc=0; 
            }
		}
        if(g_Addr!=appCS){ 
        #ifdef PRINT_LCD
            lcd_putsf_row(3,"cs r");
            lcd_puts_hex(g_Addr>>8);
            lcd_puts_hex(g_Addr);
            lcd_putsf(" e"); 
            lcd_puts_hex(appCS>>8);
            lcd_puts_hex(appCS);
        #endif 
            return FALSE;
        }
        #asm("sei");
        return TRUE;
}

/*****************************************************************************
����� ������
******************************************************************************/
void FIFO_Reset(void)
{
  	g_wrI =
    g_rdI = 0;
    g_FIFODataSize = 0;
}

/*****************************************************************************
������ � �����
******************************************************************************/
void FIFO_Write(UCHAR data)
{
  	if(AVR910_FIFO_SIZE > g_FIFODataSize)
    {
      	g_FIFO[g_wrI++] = data;
    	g_wrI &= AVR910_FIFO_MASK;
        g_FIFODataSize++;
    }
}

/*****************************************************************************
������ �� ������
******************************************************************************/
UCHAR FIFO_Read(void)
{  	
    if(0 < g_FIFODataSize)
    {
        UCHAR data = g_FIFO[g_rdI++];
        g_rdI &= AVR910_FIFO_MASK;
        g_FIFODataSize--;
        return data;
    }

  	return 0;
}

/*****************************************************************************
���������, ���� �� � ������ ��������� ��� �������
******************************************************************************/
void CheckCommParams(UCHAR req_bytes)
{
  	// ���� � FIFO ������ ������, ��� ��������� ��� ���������� �������
  	// ������������� ���� �������� ������� ����������
  	if(req_bytes > g_FIFODataSize)
    	g_NeedParams = TRUE;
    else
      	g_NeedParams = FALSE;
}


