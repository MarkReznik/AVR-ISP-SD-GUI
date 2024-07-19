/*****************************************************************************
Файл: avr910.с

Creted by PROTTOSS
Mail to PROTTOSS@mail.ru

Эмулятор AVR910

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
Установки программатора
******************************************************************************/



//#define LOW_SCK_DELEY()	delay_us(tmpimage.spidiv)//#asm("nop")//			//300 // количество тактов МК для частоты 20 кГц (25 + 25 мкс)
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
Глобальные переменные
******************************************************************************/
BOOL    g_IsFuseClock;
UCHAR	g_DeviceType;		// тип выбранного МК

UCHAR	g_DeviceCode;
UCHAR	g_DevicePollCode;	// код поллинга или размер буфера страниц устройства
UINT 	g_Addr; 			// адрес памяти в устройстве

// структура буфера принимаемых от USB данных
BOOL	g_BlockMode;		// флаг записи/чтения в блочном режиме
ULONG	g_BlockSize;		// остаточный размер блока //support 64k...
UCHAR	g_MemType;			// тип памяти в блочном режиме
UCHAR	g_WordCount;		// количество загруженных в МК байт/слов в блочном режиме
UCHAR	g_Operation;		// тип выполняемой блочной операции
UCHAR	g_Command;			// исполняемая в текущий момент команда
BOOL	g_NeedParams;		// флаг ожидания параметров команды

// структура FIFO буфера команд/данных
UCHAR	g_FIFO[AVR910_FIFO_SIZE]; // собственно буфер
UCHAR	g_wrI;			// индекс для записи в буфер
UCHAR	g_rdI;			// индекс для чтения из буфера
UCHAR	g_FIFODataSize; // размер данных в буфере

// структура передаваемых в USB данных
UCHAR	g_txBuf[AVR910_TX_DATA_SIZE];	// буфер передаваемых данных
UCHAR 	g_txWR;			// индекс начала записываемых данных
UCHAR	g_txRD;			// индекс начала читаемых данных

// строка идентификатора
flash UCHAR SwID[] =
{
  	'A', 'V', 'R', ' ', 'I', 'S', 'P', 0 ,
};

/*****************************************************************************
Прототипы функций
******************************************************************************/

// команды SPI-инструкций AVR
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

// команды SPI-инструкций 89S
#define S89_ADDR_HBYTE(h_addr)		((h_addr << 3) | ((h_addr >> 3) & 0x04))
#define S89_PROG_EN()				Write123(0xAC, 0x53, 0x00)
#define S89_CHIP_ERASE()			Write123(0xAC, 0x04, 0x00)
#define S89_RD_FLASH(addr)			Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x01, LOBYTE(addr), 0x00)
#define S89_WR_FLASH(addr, data)	Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x02, LOBYTE(addr), data)
#define S89_RD_EEPROM(addr)			Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x05, LOBYTE(addr), 0x00)
#define S89_WR_EEPROM(addr, data)	Write123(S89_ADDR_HBYTE(HIBYTE(addr)) | 0x06, LOBYTE(addr), 0x00)
#define S89_WR_LOCK(data)			Write123(0xAC, (data & 0xE0) | 0x07, 0x00)

// тайминги для операций записи AVR
#define AVR_DELAY_RESET()		delay_ms(20)//__delay_cycles(300000) // > 20 ms
#define AVR_DELAY_WR_FUSE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define AVR_DELAY_WR_FLASH()	delay_ms(5)//__delay_cycles(150000)	// > 5 ms
#define AVR_DELAY_WR_EEPROM()	delay_ms(10)//__delay_cycles(150000)	// > 10 mс
#define AVR_DELAY_ERASE()		delay_ms(20)//__delay_cycles(300000)	// > 20 mс

// тайминги для операций записи 89S
#define S89_DELAY_RESET()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define S89_DELAY_WR_FUSE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define S89_DELAY_WR_FLASH()	delay_us(2500)//__delay_cycles(30000)	// > 2,5 ms
#define S89_DELAY_WR_EEPROM()	delay_us(2500)//__delay_cycles(30000)	// > 2,5 ms
#define S89_DELAY_ERASE()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms

#define DELAY_RESET()			delay_ms(20)//__delay_cycles(300000)  // > 20 ms
#define DELAY_WR_FUSE()			delay_ms(10)//__delay_cycles(150000) 	// > 10 ms
#define DELAY_WR_FLASH()		delay_ms(10)//__delay_cycles(150000)	// > 10 ms
#define DELAY_WR_EEPROM()		delay_ms(10)//__delay_cycles(150000)	// > 10 mс
#define DELAY_ERASE()			delay_ms(50)//__delay_cycles(300000)	// > 20 mс

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
Инициализация
******************************************************************************/
void AVR910_Init(void)
{
  	// конфигурируем выводы светодиодов
  	//LED_DDR 	|= (1 << PIN_LED_WR) | (1 << PIN_LED_RD)| (1 << PIN_LED_PASS);
    //LED_PORT	&= ~((1 << PIN_LED_WR) | (1 << PIN_LED_RD)| (1 << PIN_LED_PASS));
    LED_OK_INIT();
    LED_ERR_INIT();
    //MUX_SEL_INIT();
    ReleasePorts();
    // конфигурируем вход low_sck
    //ISP_PORT	|= (1 << PIN_LOW_SCK); // подтягиваем ко входу пуллап
    //ISP_DDR		&= ~(1 << PIN_LOW_SCK);	// конфигурируем как вход
    //ISP_PORT	|= (1 << PIN_SS); // подтягиваем ко входу пуллап
    //ISP_DDR		|= (1 << PIN_SS);	// конфигурируем как вход
    // конфигурируем таймер-счетчик TC1
//    TCCR1A = (0 << WGM11)| (0 << WGM10)|			   // CTC
//      		 (0 << COM1A1) | (1 << COM1A0);			   // Toggle OC1A on Compare Match
//	TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10) | // clkIO /1 (No prescaling)
//      		 (0 << WGM13)| (1 << WGM12);			   // CTC
//    OCR1A  = (UINT)5;
//
//    // выводим меандр
//	//ISP_DDR |= (1 << PIN_CLOCK);
//    CLOCK_DDR |= (1 << PIN_CLOCK);
    // конфигурируем SPI порт
    //SPSR = 0x00; // очищаем SPI2X

    // инициализируем AVR910 глобальные переменные
  	g_Addr = 0;
    g_BlockMode = FALSE;
    g_NeedParams = FALSE;
    FIFO_Reset();
    g_IsFuseClock=FALSE;
    //ReleasePorts();
    //initBTN;
}

/*****************************************************************************
Прием сообщения
******************************************************************************/
//void AVR910_Command(UCHAR *data, UCHAR len)
void AVR910_Command(void)
{
	g_txWR = g_txRD = 0;	// сбрасываем буфер передачи

    // копируем данные из буфера USB в FIFO программатора
    //while(len--)
    	//FIFO_Write(*data++);


    // считываем принятые из USB данные
	while(0 < g_FIFODataSize)
    {
      	// проверка блочного режима работы
      	if(TRUE == g_BlockMode)
    	{
          	// проверка операции (запись, чтение)
          	if(OPERATION_WRITE == g_Operation)
            {
              	// блочная операция записи
              	WriteBlock();
            	continue;
            }
            else
            {
              	// операция чтения еще не закончилась
              	// а мы получили данные - прерываем операцию чтения и блочный режим
              	g_BlockMode = FALSE;
            }
   		}

        // Проверяем, обрабатываем ли мы команду
        if(FALSE == g_NeedParams)
        	g_Command = GetOneChar();


      	// Анализируем команду
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

        // если байт параметров команды не хватает, ждем
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
Записывает в переменную data адрес начала данных и возвращает в переменной
g_txLen количество данных
******************************************************************************/
UCHAR AVR910_IsRxData(UCHAR **data)
{
  	*data = &g_txBuf[g_txRD]; // устанавливаем адрес начала читаемых данных
  	return (g_txWR - g_txRD); // возвращаем длину данных
}

/*****************************************************************************
Освобождает буфер данных от переданных в USB данных
в переменной bytesRead содержится количество прочитанных данных из буфера
данных.
******************************************************************************/
void AVR910_ResetTXData(void)
{
   g_txRD = g_txWR = 0;
}
void AVR910_ClearTXData(UCHAR bytesRead)
{
  	// очищаем буфер от прочитанных данных и
    // вычисляем новый указатель данных
    g_txRD += bytesRead;

    // проверяем, пуст ли буфер
    if(g_txRD == g_txWR)
    {
      	// если да, то сбрасываем буфер в ноль и
    	// проверяем блочный режим работы и операцию чтения
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
Обмен данными по SPI
записывает в SPI порт данные wr_data и возвращает прочитанные при обмене данные
******************************************************************************/
UCHAR ExSPI(UCHAR wr_data)
{
  	   UCHAR ddd;
    
    // проверяем джампер low_sck
//  	if(ISP_PIN & (1 << PIN_LOW_SCK)) // не замкнут, аппаратный SPI
//	{
//        SPI_ON();	// включаем SPI
//        SPDR = wr_data;	// загружаем SPI данными
//        while(!(SPSR & (1<<SPIF))){};	// ждем окончания передачи
//		return SPDR;	// возвращаем принятые данные
//    }
//    else // иначе софтверный SPI
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
        //SPI_OFF();	// выключаем аппаратный SPI
      	UCHAR i;
        UCHAR rd_data=0;
        for(i = 0; i < 8; i++)
        {
          	// отправляем старший бит отправляемого байта
          	if(0 != (wr_data & 0x80))
            	SET_MOSI();//ISP_PORT |= (1 << PIN_MOSI); // = 1
            else
            	CLR_MOSI();//ISP_PORT &= ~(1 << PIN_MOSI);	// = 0

            // подготавливаем следующий бит
        	wr_data <<=1;

            // принимаем очередной (начиная со старшего) бит
            // принимаемого байта
            rd_data <<= 1;
            if(0 != READ_MISO())//(ISP_PIN & (1 << PIN_MISO)))
            	rd_data++; // устанавливаем единичный бит, если высокий принимаемый уровень

			// формируем импульс на выводе SCK МК
            SET_SCK();//ISP_PORT |= (1 << PIN_SCK);	// высокий
            if(g_IsFuseClock==TRUE)
                FUSE_SCK_DELEY;
            else
                LOW_SCK_DELEY(); // задержка
            CLR_SCK();//ISP_PORT &= ~(1 << PIN_SCK);	// низкий
            if(g_IsFuseClock==TRUE)
                FUSE_SCK_DELEY;
            else
                LOW_SCK_DELEY(); // задержка
    	}
        return rd_data;  
        #endif
    }
}

/*****************************************************************************
Установить порты в рабочее состояние
******************************************************************************/
void CatchPorts(void)
{
  	// подключаем выводы SPI к ISP разъему
  	//ISP_DDR  |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_RESET);

    // подтягиваем пуллап к выводу MISO
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
    
    // разрешаем SPI и устанавливаем скорость
    //SPI_ON();

    // переводим RESET в активное состояние
    if(g_DeviceType == S89)
      	SET_RESET();
   //else
      	//CLR_RESET();
    AVR_DELAY_RESET();	// задержка

}

/*****************************************************************************
Отключить порты
******************************************************************************/
void ReleasePorts(void)
{
  	//CatchPorts();
    // запрещаем SPI
    //ISP_OFF();    
    // отключаем выводы SPI от ISP разъема
  	//ISP_DDR &=  ~((1 << PIN_SCK) | (1 << PIN_MISO) | (1 << PIN_MOSI) | (1 << PIN_RESET));
    //ISP_PORT &= ~((1 << PIN_SCK) | (1 << PIN_MISO) | (1 << PIN_MOSI) | (1 << PIN_RESET));
}

/*****************************************************************************
Возвращает данные из буфера приемника USB
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
Возвращает данные в буфер передатчика USB
******************************************************************************/

void PutOneChar(UCHAR data)
{
  	if(g_txWR < AVR910_TX_DATA_SIZE)
      	g_txBuf[g_txWR++] = data;
}

/*****************************************************************************
Записывает последовательно 3 байта в SPI и возвращает результат от записи 3-го
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
Записывает последовательно 4 байта в SPI и возвращает результат от записи 4-го
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
Вход в режим программирования
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
    ReleasePorts(); 	// отключаем порты
    // проверяем код устройства
    if(S89 == g_DeviceType) //89S
    {
      	//ReleasePorts(); // отключаем порты
		CatchPorts();	// подключаем порты к ISP
   		S89_DELAY_RESET();	// задержка

        // посылаем команду входа в режим программирования
        S89_PROG_EN();
        progOK=1;
    }
    else
    {
      	// синхронизируемся с подключенным чипом
        #ifdef PRINT_LCD
        lcd_putsf_row(3,"Echo53:");
        #endif
        for(c = 0; c < 3; c++)
        {
            
            CatchPorts();		// подключаем порты к ISP
            //AVR_DELAY_RESET();	// задержка

            // 32 цикла попыток засинхронизироваться с программируемым МК
        
           	// пытаемся вывести чип в режим программирования
			// при посылке 3-го байта в SPI чип должен вернуть код предыдущей посылки
          	ctrl_byte = AVR_PROG_EN();
            ExSPI(0x00);
            #ifdef PRINT_LCD
            lcd_puts_hex(g_MyUDR);
            #endif
            // получили верный контрольный байт - синхронизация выполнена
            if(0x53 == ctrl_byte){
           		progOK=1;
                break;//goto m1;
           	}
            // если нет валидного возврата, то сдвигаем при каждой попытке строб
          	/*
            SET_SCK();
    		delay_us(50);//__delay_cycles(600); // задержка 50 мкс
			CLR_SCK();
    		delay_us(50);//__delay_cycles(600);
            */
        }
        // если мы не засинхронизировались,
        // считаем, что устройство не найдено
        if(progOK==0){
            ReleasePorts(); // отключаем порты
            err = NACK; // посылаем код ошибки
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
Возвращает коды поддерживаемых устройств
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
Выбор типа подключенного микроконтроллера
Устанавливает индекс в массиве описателей устройств, если полученный код
контроллера от хоста верный
******************************************************************************/
void SetDevType(void)
{
  	UCHAR code, err = NACK, i = 0, request;

    // проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

    request = GetOneChar();

    // сравниваем все коды МК в таблице устройств с полученным
    // пока код МК не равен 0
	while(0 != (code = DeviceDesc[i]))
    {
      	// если код совпал, есть поддерживаемое устройство,
      	// формируем данные для устройства
      	if(request == code)
        {
          	// устанавливаем тип МК
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
Зажечь индикатор программирования
команда игнорируется
******************************************************************************/
void SetLED(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	GetOneChar();
	PutOneChar(ACK);
}

/*****************************************************************************
Погасить индикатор программирования
команда игнорируется
******************************************************************************/
void ClrLED(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	GetOneChar();
	PutOneChar(ACK);
}

/*****************************************************************************
Универсальная команда: пишет в SPI 3 байта данных и возвращает в буфер
передачи результат от записи 4-го (0x00)
******************************************************************************/
void tx123rx4(void)
{
  	UCHAR b1,b2,b3;
    // проверяем, есть ли данные для команды
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
Универсальная команда: пишет в SPI 3 байта данных и возвращает в буфер
передачи результат от записи 4-го (d4)
******************************************************************************/
void tx1234rx4(void)
{
  	UCHAR b1,b2,b3,b4;
    // проверяем, есть ли данные для команды
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
Запись в память программ, младший байт
******************************************************************************/
void WriteProgLB(void)
{
  	UCHAR data;
    // зажигаем индикатор записи
  	LED_WR_ON();

  	// получаем байт данных
	data = GetOneChar();

    // пишем байт
    if(g_DeviceType == S89)
    	S89_WR_FLASH(g_Addr, data);
    else
    	AVR_WR_FLASH_LB(g_Addr, data);

    // проверяем запись байта
	PollData(0x20, data);

    // инкрементируем адрес, если 89S
    if(S89 == g_DeviceType)
      	g_Addr++;

    // гасим индикатор записи
    LED_WR_OFF();
}

/*****************************************************************************
Запись в память программ, старший байт
******************************************************************************/
void WriteProgHB(void)
{
  	UCHAR data;
    if(S89 != g_DeviceType)
    {
      	// зажигаем индикатор записи
  		LED_WR_ON();

      	// получаем старший байт данных
		data = GetOneChar();

      	// пишем в SPI старший байт
        AVR_WR_FLASH_HB(g_Addr, data);

    	// проверяем запись байта
		PollData(0x28, data);

    	// инкрементируем адрес
    	g_Addr++;

        // гасим индикатор записи
    	LED_WR_OFF();
    }
}

/*****************************************************************************
Одиночная запись в память программ, младший байт
******************************************************************************/
void WriteProgOneLB(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteProgLB();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
Одиночная запись в память программ, старший байт
******************************************************************************/
void WriteProgOneHB(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteProgHB();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
Одиночная запись в память данных
******************************************************************************/
void WriteOneData(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	WriteData();
    PutOneChar(ACK); // OK
}

/*****************************************************************************
Проверка поллинга и страничного режима программирования
******************************************************************************/
void PollData(UCHAR command, UCHAR data)
{
  	// проверяем поллинг
  	if(g_DevicePollCode & 0x0F)
    {
      	// если данные совпадают с кодом поллинга, мы не можем проверить запись
      	if(g_DevicePollCode == data)
        	DELAY_WR_FLASH();

        else
        {
          	// 256 циклов поллинга
          	UCHAR cycle = 0xFF;
          	do
            {
              	// читаем записанные
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

            // если валидный код так и не получен (((
            if(0 == cycle)
            	DELAY_WR_FLASH();
        }
    }
}

/*****************************************************************************
Запись страницы в память программ
******************************************************************************/
void WritePage(void)
{
  	// загружаем команду Write Programm Memory Page
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
Чтение памяти программ
******************************************************************************/
void ReadProg(UCHAR com_byte1, UCHAR com_byte2)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();

  	if(S89 != g_DeviceType)// AVR - читаем слово
    {
      	PutOneChar(Write1234(com_byte1, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
        PutOneChar(Write1234(com_byte2, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
    }
    else//S89 - читаем байт
    {
      	PutOneChar(S89_RD_FLASH(g_Addr));
    }

    // инкременируем адрес
	g_Addr++;

    // гасим индикатор чтения
  	LED_RD_OFF();
}

/*****************************************************************************
Запись в память данных (EEPROM)
******************************************************************************/
void WriteData(void)
{
  	UCHAR data;
    // зажигаем индикатор записи
  	LED_WR_ON();

	// получаем байт данных
	data = GetOneChar();

    // проверяем тип устройства
    if(S89 != g_DeviceType) // AVR
    	AVR_WR_EEPROM(g_Addr, data);
    else // 89S
    	if(0x86 == g_DeviceCode)// EEPROM есть только у 8252
           	S89_WR_EEPROM(g_Addr, data);

    DELAY_WR_EEPROM();

    // инкрементируем адрес
    g_Addr++;

    // гасим индикатор записи
    LED_WR_OFF();
}

/*****************************************************************************
Чтение памяти данных (EEPROM)
******************************************************************************/
void ReadData(void)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();

  	// проверяем тип устройства
    if(S89 != g_DeviceType)// AVR
    	PutOneChar(Write1234(0xA0, HIBYTE(g_Addr), LOBYTE(g_Addr), 0x00));
	else
    	if(0x86 == g_DeviceCode)//S89 EEPROM имеется только у 8252
        	PutOneChar(Write123(((HIBYTE(g_Addr) << 3) | 0x05), LOBYTE(g_Addr), 0x00));

    // инкременируем адрес
	g_Addr++;

    // гасим индикатор чтения
  	LED_RD_OFF();
}

/*****************************************************************************
Стереть память программ и данных (EEPROM) чипа
******************************************************************************/
void ChipErase(void)
{
  	// зажигаем индикатор записи
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
  	// посылаем команду Chip Erase, задержка и OK
  	if(g_DeviceType == S89)
      	S89_CHIP_ERASE();
    else
      	AVR_CHIP_ERASE();
	DELAY_ERASE();
    PutOneChar(ACK);

    // гасим индикатор записи
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
Запись битов блокировки
******************************************************************************/
void WriteLock(void)
{
  	UCHAR data;
    // проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// зажигаем индикатор записи
  	LED_WR_ON();
    g_IsFuseClock=TRUE;

	// получаем байт данных, пишем lock bits, задержка и OK
	data = GetOneChar();
	if(g_DeviceType == S89)
      	S89_WR_LOCK(data);
    else
      	AVR_WR_LOCK(data);
	DELAY_WR_FUSE();
    PutOneChar(ACK);

    // гасим индикатор записи
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
Запись бит управления
******************************************************************************/
void WriteFuse(void)
{
  	UCHAR err,data;
    // проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// зажигаем индикатор записи
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// получаем байт данных, пишем fuse bits, задержка и OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // гасим индикатор записи
    LED_WR_OFF(); 
    g_IsFuseClock=FALSE;
}
void WriteHFuse(void)
{
  	UCHAR err,data;
    // проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// зажигаем индикатор записи
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// получаем байт данных, пишем fuse bits, задержка и OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE_H(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // гасим индикатор записи
    LED_WR_OFF(); 
    g_IsFuseClock=FALSE;
}
void WriteEXTFuse(void)
{
  	UCHAR err,data;
    // проверяем, есть ли данные для команды
    CheckCommParams(1);
    if(TRUE == g_NeedParams)
      	return;

  	// зажигаем индикатор записи
  	LED_WR_ON();
    g_IsFuseClock=TRUE;
	// получаем байт данных, пишем fuse bits, задержка и OK
	err = ACK, data = GetOneChar();
    if(g_DeviceType == S89)
      	err = NACK;
    else
    	AVR_WR_FUSE_EXT(data);
    DELAY_WR_FUSE();
    PutOneChar(err);

    // гасим индикатор записи
    LED_WR_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
Чтение битов управления и блокировки (младший байт)
******************************************************************************/
void ReadFuse(void)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE());

    // гасим индикатор чтения
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
Чтение битов управления и блокировки (младший байт)
******************************************************************************/
void ReadLock(void)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_LOCK());

    // гасим индикатор чтения
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
Чтение битов управления и блокировки (старший байт)
******************************************************************************/
void ReadHFuse(void)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE_H());

    // гасим индикатор чтения
  	LED_RD_OFF(); 
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
Чтение битов управления и блокировки (старший байт)
******************************************************************************/
void ReadEXTFuse(void)
{
  	// зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем
    PutOneChar((g_DeviceType == S89)? NACK: AVR_RD_FUSE_EXT());

    // гасим индикатор чтения
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}
/*****************************************************************************
Чтение сигнатуры чипа
******************************************************************************/
void ReadSignature(void)
{
  	UCHAR addr;
    // зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем последовательно Vendor, Family, Number
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
    // гасим индикатор чтения
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}

void ReadCalibrationByte(void)
{
  	UCHAR addr;
    // зажигаем индикатор чтения
  	LED_RD_ON();
    g_IsFuseClock=TRUE;
    // читаем последовательно Vendor, Family, Number
    addr = 0;// case when 4 calibration bytes 
    do
    {
    	//PutOneChar((g_DeviceType == S89)?0xFF:(AVR_RD_SIGN(addr)));
        PutOneChar(AVR_RD_CALIBR(addr));//g_txBuf[g_txWR++] = data;
        addr++;
    }
    while(addr<=3); // case when 4 calibration bytes
    // гасим индикатор чтения
  	LED_RD_OFF();
    g_IsFuseClock=FALSE;
}

/*****************************************************************************
Выход из режима программирования
******************************************************************************/
void LeaveProgMode(void)
{
    ReleasePorts();
  	PutOneChar(ACK);
}

/*****************************************************************************
Проверка поддержки блочного режима записи/чтения
******************************************************************************/
void GetBlockMode(void)
{
  	PutOneChar(YES);
    PutOneChar((UCHAR)(AVR910_REPORT_BLOCK_SIZE >> 8));
    PutOneChar(AVR910_REPORT_BLOCK_SIZE & 0xFF);
}

/*****************************************************************************
Возвращает идентификатор ПО
******************************************************************************/
void GetSWID(void)
{

    UCHAR flash *ptr = SwID;
  	while(0 != *ptr)
      	PutOneChar(*ptr++);
}

/*****************************************************************************
Возвратить версию ПО
******************************************************************************/
void GetSWVer(void)
{
  	PutOneChar(SOFTWARE_VER_1);
    PutOneChar(SOFTWARE_VER_2);
}

/*****************************************************************************
Возвратить версию платы
******************************************************************************/
void GetHWVer(void)
{
  	PutOneChar(HARDWARE_VER_1);
    PutOneChar(HARDWARE_VER_2);
}

/*****************************************************************************
Возвратить поддержку автоприращения адреса
******************************************************************************/
void GetAutoIncAddr(void)
{
  	PutOneChar(ACK);
}

/*****************************************************************************
Возвратить тип программатора
******************************************************************************/
void GetPROGType(void)
{
  	PutOneChar(PROGRAMMER_TYPE);
}

/*****************************************************************************
Установить адрес
******************************************************************************/
void SetAddr(void)
{
  	// проверяем, есть ли данные для команды
    CheckCommParams(2);
    if(TRUE == g_NeedParams)
      	return;

  	g_Addr = MAKEUINT(GetOneChar(), GetOneChar());
    PutOneChar(ACK);
}

/*****************************************************************************
Обработка команды блочного чтения
******************************************************************************/
void ReadBlockCommand(void)
{
  	UCHAR size_h,size_l;
    // проверяем, есть ли данные для команды
    CheckCommParams(3);
    if(TRUE == g_NeedParams)
      	return;

  	// получаем размер блока и тип памяти
  	size_h = GetOneChar();
    size_l = GetOneChar();
    g_BlockSize = ((UINT)(size_h << 8)) | ((UINT)(size_l));
	g_MemType = GetOneChar();

    g_Operation = OPERATION_READ;
    g_BlockMode = TRUE;

    // проверяем код устройства
    if(S89 != g_DeviceType)
       	if('F' == g_MemType)// в память программ AVR пишем словами
      		g_BlockSize /= 2;
	ReadBlock();
}

/*****************************************************************************
Блочное чтение данных
******************************************************************************/
void ReadBlock(void)
{
  	// проверяем тип памяти
    if('F' == g_MemType)//FLASH
    {
      	// заполняем буфер данными
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
      	// заполняем буфер данными
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
Обработка команды блочной записи
******************************************************************************/
void WriteBlockCommand(void)
{
  	UCHAR size_h,size_l,size_e=1;
    // проверяем, есть ли данные для команды
    CheckCommParams(4);
    if(TRUE == g_NeedParams){// if less than 4 bytes waiting==true -> for big size
      CheckCommParams(3);
      size_e=0;
      if(TRUE == g_NeedParams)
        return;
    }

  	// получаем размер блока и тип памяти
  	if(size_e)
        size_e = GetOneChar();
    size_h	= GetOneChar();
    size_l	= GetOneChar();
    g_BlockSize		= ((ULONG)((ULONG)size_e << 16)) | ((UINT)(size_h << 8)) | ((UINT)(size_l));
    g_MemType		= GetOneChar();
	g_BlockMode		= TRUE;
    g_Operation		= OPERATION_WRITE;
    g_WordCount 	= 0;

    // проверяем код устройства
    if(S89 != g_DeviceType)
       	if('F' == g_MemType)// в память программ AVR пишем словами*/
      		g_BlockSize /= 2;
}

/*****************************************************************************
Блочный режим записи данных
******************************************************************************/
void WriteBlock(void)
{
  	// проверяем тип памяти
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

            // Проверяем заполняемость страницы
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

    // если блок кончился
    if(0 == g_BlockSize)
    {
      	// а страница не догруженна
      	if(g_WordCount != 0)

          	// значит конец записи данных,
          	// если писали в память программ то догружаем страницу
          	if('F' == g_MemType)//Flash
      			WritePage();

      	// отмечаем, что блок принят
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
Сброс буфера
******************************************************************************/
void FIFO_Reset(void)
{
  	g_wrI =
    g_rdI = 0;
    g_FIFODataSize = 0;
}

/*****************************************************************************
Запись в буфер
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
чтение из буфера
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
Проверяем, есть ли в буфере параметры для команды
******************************************************************************/
void CheckCommParams(UCHAR req_bytes)
{
  	// если в FIFO меньше данных, чем требуется для параметров команды
  	// устанавливаем флаг ожидания прихода параметров
  	if(req_bytes > g_FIFODataSize)
    	g_NeedParams = TRUE;
    else
      	g_NeedParams = FALSE;
}


