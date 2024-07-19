/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Standalone avr isp programmer for use with sdcard  lcd  maybe m64
Version : 12  isp pinout changed to port d to improve lcd progress
Date    : 06/25/2015
Author  : Mark Reznik
Company :
Comments: Based on usb avrprog from PROTTOSS
V18:    fixed fusemask to 1's instead 0's
        freq changed to 12MHz
        fixed up/down/start buttons
V19:    Buttons rotated. but not stable response
V21:    RC1.
V24     take pagesize in bytes. ini file hold pagesize in words
        DevicePollCode fixed to take the pagesize in word. 
V26:    Added eeprom features. Write SN, Write EEprom from eeprom.hex file
V28:    Fixed Stop button no reset response.
V29:    Config.ini moved from root to folder '0'
V30:    Fixed verify flash no end loop
        Fixed reset chip loop
        Fixed start beep 
V32     ISP out changed to USART SPI mode. PD1=TXD=MOSI, PD0=RXD=MISO, PD4=SCK, PD7=LCD_STR(EN)
V33     LCD 4 rows fixes
V34     Chenged LCD line 1,2 for 28 chars description
V35     3 lines info
V36     3 lines info fix. Added 3rd line read
V37     fix endless loop verify flash, added timer of flashing
V38     Added calibration bytes to eeprom feature
V40     convert data to binary
V42     Fast flash write, encoder counter.
V1.0.42.11  Add skip check file unique id in case free license
V1.0.42.12  Add virgin function, mini prog up button changed to stop once started.
V1.0.42.13  Add 7seg display function
V1.0.42.14  Fixed 7seg display function sequence
V1.0.42.15  Added tador skip function
V1.0.42.16c  Add asaf UPO DOWNO
V1.0.42.17  Virgin func changed to always while(1)
V1.0.42.18  Force func changed to skip reset licences.
V1.0.42.19  Fixed reset pin direction. wasn't defined as out.
V1.0.42.20  Added spidiv 6.
V1.0.42.21  Added only boot file support.

Chip type               : ATmega328
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small 2048
External RAM size       : 0
Data Stack size         : 256
*****************************************************/
#define FW_VER  "42.21"//isp via uart spi rolling
#include "common.h"
#include <eeprom.h>

volatile unsigned char CounterLed=0;
volatile unsigned char Counter100Hz=0;
volatile unsigned int Counter10ms=0;

// Declare your global variables here
char lcdnum[16];

UCHAR CanTestButtons=FALSE;
UCHAR DisableNextDir=FALSE;

UCHAR CurrentButton=BUTTONS_PRESSED;

volatile char CanChangeMsg=0;

volatile unsigned char  cnt_beep_delay_ms=0;
unsigned char  beep_delay_ms=0;
volatile unsigned char  cnt_beep_ms=0;
volatile unsigned char  cnt_beep_times=0;
volatile unsigned char  cnt_beep_tone=0;
unsigned char  beep_ms=0;
unsigned char  beep_times=0;
unsigned char  beep_tone=0;
volatile unsigned char  CanBeep=0;
volatile unsigned char  StopEvent=0;
unsigned char    DoTone=0;
unsigned char    DoTimes=0;
unsigned char    DoTypeLength=0;
unsigned int    CounterMsg=0;
unsigned char   CounterToggles=15;
char MaxMsg;
//UCHAR t1_init;
UCHAR t1_init_l;
UCHAR t1_init_h;

interrupt [TIM1_OVF] void timer_comp_isr(void)
{
    /* re-initialize Timer1 */
    TCNT1H=t1_init_h;//T1_INIT>>8;
    TCNT1L=t1_init_l;//T1_INIT&0xFF;
    /* card access low level timing function */
    #ifndef PETITFATFS
    disk_timerproc();
    #endif
    /* the rest of the interrupt service routine */
    /* .... */
    if(++Counter100Hz>20){//(1/2000=0.5m) 0.5ms*20=10ms
        Counter100Hz=0;
        Counter10ms++;
        
        if(StandaloneMode==IDLE){
        #ifdef PRINT_LCD    
            LED_PASS_OFF();
            LED_ERR_OFF();
        #else
        #ifdef SEG7
            if(DisableNextDir==FALSE){
                if(++CounterLed>=60){  //300ms
                    CounterLed=0;
                    show7seg(dirnum);
                }
                else if(CounterLed==30){
                    init7seg();
                }
            }
            else
                show7seg(dirnum);
        #else
            if(++CounterLed>=30){  //300ms
                CounterLed=0;
                if(CounterToggles<=(dirnum<<1)){
                    LED_PASS_TGL();
                }
                else if(CounterToggles>=((dirnum<<1)+9)){
                    CounterToggles=0;
                }
                else{
                    LED_PASS_OFF();
                }
                CounterToggles++;            
            }
        #endif
        #endif    
        }
        else if(StandaloneMode==RUNNING){
            if(++CounterLed>=3){  //30ms
                CounterLed=0;
                LED_ERR_OFF();
                LED_PASS_TGL();
            }
        }
        else if(StandaloneMode==PASS){
            LED_PASS_ON();
            LED_ERR_OFF();
            #ifdef SEG7 
            //show7seg('P');
            #endif
        }
        else if(StandaloneMode==FAIL){
            LED_PASS_OFF();
            if(++CounterLed>=60){
                CounterLed=0;
                LED_ERR_TGL();
                #ifdef SEG7 
                //show7seg('E');
                #endif
                if(StopEvent==0)
                    DoBeeps(3,1,1);
            }
        #ifdef PRINT_LCD    
            if(StopEvent==0){
                if(TestADCs()==(1<<BUTTON_STOP)){
                    StopEvent=1;
                    CanBeep=0;                 
                }
            }
        #endif    
        }
        else if(StandaloneMode==SDFAIL){
            LED_PASS_OFF();
            if(++CounterLed>=100){
                CounterLed=0;
                LED_ERR_TGL();
                #ifdef SEG7 
                show7seg('E');
                #endif
            }
            else if(CounterLed==50){
                #ifdef SEG7 
                init7seg();
                #endif
            }
        }
        else if(StandaloneMode==READSD){
            LED_PASS_OFF();
            LED_ERR_OFF();
            #ifdef SEG7 
            show7seg('-');
            #endif
            if(++CounterLed>=100){
                CounterLed=0;
                //LED_ERR_TGL();
            }
        }
//        if(msg_buf_cnt && (CanChangeMsg==0)){
//            if( (++CounterMsg>=50)){
//                CounterMsg=0;
//                CanChangeMsg=1;         
//            }
//        }
    }
    
    if((StandaloneMode!=RUNNING)&&CanBeep){
        if((Counter100Hz%2)==0){//1ms cnt
            if(cnt_beep_ms){
                cnt_beep_ms--;
                if(cnt_beep_ms==0){
                    if(cnt_beep_times){
                       cnt_beep_times--;
                       cnt_beep_delay_ms=beep_delay_ms;
                    }
                }
                if(cnt_beep_tone){
                   cnt_beep_tone--;
                   if(cnt_beep_tone==0){
                       BUZ_TGL();
                       cnt_beep_tone=beep_tone;
                   }
                }
            }
            else if(cnt_beep_delay_ms){ 
                cnt_beep_delay_ms--;
                if(cnt_beep_delay_ms==0){
                   cnt_beep_ms=beep_ms;
                }
            }
        }
        
         
    }
}



#define MAX_BUTTONS     4
#define ADC_VREF_TYPE   0x20

volatile unsigned char adc_data;
volatile BOOL ADC_DONE;



#define NUM_OF_TICKS    5000//ticks to measure long button press.  Max long press=NUM_OF_TICKS*TICK_DELAY=100*50=5sec
#define TICK_DELAY      1//50ms delay between ticks
#define DEBOUNCE        10//ticks to debounce
// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
    // Read the 8 most significant bits  (Vref*(1/1))1024=(11,1111,11)11=FF , (Vref*(1/2))512=(01,1111,11)11=7F (Vref*(1/4))256=3F (Vref*(1/8))128=1F
    // of the AD conversion result
    ADC_DONE=TRUE;
    adc_data=ADCH;
    // Place your code here
}




void main(void)
{
// Declare your local variables here
StandaloneMode=READSD;
//UINT i;//blink led loops
// Crystal Oscillator division factor: 1
#ifdef _MEGA328_INCLUDED_
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
#endif
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTD=0x00;
DDRD|=0x80;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
#ifdef _MEGA64_INCLUDED_
ASSR=0x00;
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;
#else
TCCR0A=0x00;
TCCR0B=0x00;
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;
#endif
// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2A output: Disconnected
// OC2B output: Disconnected
#ifdef _MEGA64_INCLUDED_
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;
#else
ASSR=0x00;
TCCR2A=0x00;
TCCR2B=0x00;
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;
#endif
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// Interrupt on any change on pins PCINT0-7: Off
// Interrupt on any change on pins PCINT8-14: Off
// Interrupt on any change on pins PCINT16-23: Off
EICRA=0x00;
EIMSK=0x00;
#ifdef _MEGA64_INCLUDED_
EICRA=0x00;
EICRB=0x00;
EIMSK=0x00;
#else
PCICR=0x00;
#endif
#ifdef _MEGA64_INCLUDED_
TIMSK=0x00;
ETIMSK=0x00;
#else
// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=0x00;

// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=0x00;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x00;
#endif
#ifdef PRINT_DEBUG
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 9600
UCSR0A=0x00;
UCSR0B=0xD8;
UCSR0C=0x06;
UBRR0H=0x00;
//UBRR0L=0x33; //9600 for 8Mhz osc
UBRR0L=0x4D; //9600 for 12Mhz osc
//UBRR0L=0x67; //9600 for 16Mhz osc
#else
UBRR0H = 0;
UBRR0L = 0;
/* Setting the XCKn port pin as output, enables master
mode. */
ISP_SCK_INIT();//XCK0_DDR |= (1<<XCK0);
/* Set MSPI mode of operation and SPI data mode 0. */
UCSR0C =
(1<<UMSEL01)|(1<<UMSEL00)|(0<<UCPHA0)|(0<<UCPOL0);
/* Enable receiver and transmitter. */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set baud rate. */
/* IMPORTANT: The Baud Rate must be set after the
transmitter is enabled */
UBRR0H = 0;    //+1 prescaler of clock
UBRR0L = 0;  // 2+1=3 12Mhz/2=6Mhz 6/3=2Mhz spi clock out
#endif

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
ADCSRB=0x00;
#ifndef _MEGA64_INCLUDED_
DIDR1=0x00;
#endif
// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: ADC Stopped
// Only the 8 most significant bits of
// the AD conversion result are used
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
// ADC4: On, ADC5: On
#ifndef _MEGA64_INCLUDED_
DIDR0=0x00;
#endif
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x83;//ADCSRA=0x8B;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

ADC_DONE=TRUE;
AVR910_Init();
init_sd_tim1();
BUZ_INIT();
debug=1;
// Global enable interrupts
#asm("sei")
#ifdef PRINT_DEBUG
if(debug)
    printf("Init done. Current folder is %d.\r\n",dirnum);
#endif
g_UpSkip=1;
g_DownSkip=1;
g_MaxFolders=5;
g_CpuFreq=8000;
/*
t1_init_h=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))>>8;
t1_init_l=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))&0xff;
Beeps(1,5,1);
while(1);
*/
#ifdef PRINT_LCD
PORTC&=~0x3F;
DDRC|=0x3F; //enable lcd control PC0,1,2,3,4,5=0b0011,1111
PORTD&=~0x80;
DDRD|=0x80; //enable lcd control STR=EN PD7
MsgBufClr();
lcd_init(16);
lcd_gotoxy(0,0);
lcd_putsf("AVRISPSD V");
lcd_putsf(FW_VER);
#ifndef DEBUG_PROTEUS
#ifdef PRINT_LCD
delay_ms(1000);
#endif
#endif
#endif

#ifdef SEG7 //7 segment
init7seg();
if(eeprom_read_byte(4)!=0x44)
{
    StandaloneMode=RUNNING;
    show7seg('d');
    delay_ms(1000);
    StandaloneMode=READSD;
}
#endif

t1_init_h=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))>>8;
t1_init_l=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))&0xff;
Call_GetConfig();
#ifndef PRINT_LCD
g_UpSkip=1;
g_DownSkip=1;
#ifdef SEG7
g_MaxFolders=10;
#else
g_MaxFolders=5;
#endif
#endif
//t1_init=(0x10000L-(g_CpuFreq/(T1_PRESC*T1_OVF_FREQ)));
t1_init_h=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))>>8;
t1_init_l=(UINT)(0x10000L-(g_CpuFreq*1000L/(T1_PRESC*T1_OVF_FREQ)))&0xff;
/*
#ifdef PRINT_LCD
lcd_putsf_row(1,"init ");
lcd_puts_hex(t1_init_h);
lcd_puts_hex(t1_init_l);
lcd_putsf(" ");
lcd_puts_hex(g_CpuFreq>>8);
lcd_puts_hex(g_CpuFreq);
delay_ms(3000);
#endif
*/

#ifdef PRINT_LCD
if(SaveDirNum==0 || SaveDirNum>=(g_MaxFolders-1)) //reset eeprom last project directory
    SaveDirNum=1;
dirnum=SaveDirNum;
#else
dirnum=1;// no lcd always start from 1
#ifdef SEG7
//show7seg(1);
#endif
#endif
//StandaloneMode=IDLE;

Beeps(0,0,0);
#ifdef PRINT_LCD
if(SaveLockState>1)
    SaveLockState=0;
if(lcd_GetProgName(0)==TRUE){
    StandaloneMode=IDLE;
}
else
    StandaloneMode=SDFAIL;
#else
if(SaveLockState!=0)
SaveLockState=0;
CounterToggles=15;
if(Call_GetDir()==TRUE){
    Beeps(1,1,1);
    StandaloneMode=IDLE;
}
else{
    StandaloneMode=SDFAIL;
    //return;
}
#endif
    while (1)
    {
        #ifndef PRINT_LCD
        CanShow=0;
        #endif
        CanTestButtons=TestADCs();
        switch(CanTestButtons)
        {
            #ifdef PRINT_LCD
            case (1<<BUTTON_UP):
                UpButtonAction();
                break;
            #endif    
            case (1<<BUTTON_DOWN):
            #ifdef PRINT_LCD
                DownButtonAction();
            #else
                if(DisableNextDir==TRUE)
                    StopButtonAction();  
                else{
                    if(StandaloneMode==FAIL || StandaloneMode==SDFAIL){
                        StopButtonAction();
                    }
                    else{
                        UpButtonAction(); 
                        //StopButtonAction();
                    } 
                }
            #endif   
                break;
            #ifdef PRINT_LCD    
            case (1<<BUTTON_START):StartButtonAction();
                break;
            #endif
            case (1<<BUTTON_STOP):
             #ifdef PRINT_LCD
                StopButtonAction();
             #else 
                StartButtonAction();
             #endif     
                break;        
            default:CanTestButtons=0;
                break;
        }
        if(StandaloneMode==IDLE || StandaloneMode==PASS){
               #asm("nop")
        }
        else if(StandaloneMode==FAIL || StandaloneMode==SDFAIL){
           //CanTestButtons=0;
           #ifdef PRINT_LCD
           if(CanShow==0){
                CanShow=1;
                //lcd_putsf_row3(3,"Press Stop");
           }
           #endif  
        }
        #ifdef PRINT_LCD
        if(StopEvent)  
            StopButtonAction();
        if(CanChangeMsg)
            ChangeMsg();
        #endif    
    } //end while
}//end main
void UpButtonAction()
{
    //bool isOneDirReadOk=false;
    int i;
    #ifdef DEBUG_BUTTONS;
    lcd_putsf_row(0,"Up");
    #endif
    #ifdef PRINT_LCD
    if((StandaloneMode!=IDLE)&& (StandaloneMode!=SDFAIL)){
        return;
    }
    #else
    #ifndef SEG7
    if((dirnum >= g_MaxFolders)){
        DoBeeps(3,1,1);
        return;
    }
    #endif
    if((StandaloneMode!=IDLE)){
        DoBeeps(3,1,1);
        return;
    }
    #endif
    if(SaveLockState!=0){
        #ifdef PRINT_LCD
        lcd_putsf_row(3,"List Locked");
        #endif
        Beeps(3,1,1);
        return;
    }
    for(i=NUM_OF_TICKS;i>0;i--){  //if up button pressed about 10 sec - unlock list function
        delay_ms(TICK_DELAY);
        if(i==(NUM_OF_TICKS-DEBOUNCE))
            DoBeeps(1,1,1);
        #ifdef PRINT_LCD
        if(TestButton(BUTTON_UP)==FALSE)
        #else
        if(TestButton(BUTTON_DOWN)==FALSE)
        #endif
            break;
        
    }
    if(i>(NUM_OF_TICKS-DEBOUNCE)){ //debounce
        return;
    }
    if(i==0){
        DoBeeps(3,1,1);//
        return;
    }
chkdir:    
    #ifdef PRINT_LCD
    if((dirnum+g_UpSkip) > g_MaxFolders)
        dirnum=1;
    else 
        dirnum+=g_UpSkip;
    #else
    CounterToggles=15; 
    if((dirnum+1) > g_MaxFolders){
        #ifndef SEG7
        DoBeeps(3,1,1);//
        return;
        #else 
        dirnum=1;
        #endif 
    }
    else 
        dirnum+=1;    
    #endif    
    #ifdef PRINT_DEBUG
    while(tx_counter);
    printf("Current Folder %d\r\n",dirnum);
    while(tx_counter);
    #endif
    StandaloneMode=READSD;
    #ifdef PRINT_LCD
    if(lcd_GetProgName(0)==TRUE){
       StandaloneMode=IDLE;
       Beeps(1,1,1);
    }
    else{
        StandaloneMode=SDFAIL;
        Beeps(3,1,1);
        return;
    }
    #else
    if(Call_GetDir()==TRUE){
        StandaloneMode=IDLE;
        Beeps(1,dirnum,1);
    }
    else{
        //StandaloneMode=SDFAIL;
        Beeps(3,1,1);
        #ifdef SEG7 
        dirnum=1;
        #else
        dirnum-=1;
        #endif
        if(Call_GetDir()==TRUE){
            StandaloneMode=IDLE;
            //Beeps(1,dirnum,1);
        }
        else{
            StandaloneMode=SDFAIL;
            Beeps(3,1,1);
            return;
        }
    }
    #endif
    StandaloneMode=IDLE;
    LEDS_OFF();
}

void StartButtonAction()
{
    int i;
    #ifdef DEBUG_BUTTONS;
    lcd_putsf_row(0,"Start");
    #endif
    if(StandaloneMode!=IDLE && StandaloneMode!=PASS){
       //DoBeeps(3,1,1);
       return;
    }
    for(i=NUM_OF_TICKS;i>0;i--){
        delay_ms(TICK_DELAY);
        
        if(i==(NUM_OF_TICKS-DEBOUNCE))
            Beeps(1,1,1);// one short beep
        #ifdef PRINT_LCD
        if(TestButton(BUTTON_START)==FALSE)
        #else
        if(TestButton(BUTTON_STOP)==FALSE)
        #endif
            break;    
    }
    if(i>(NUM_OF_TICKS-DEBOUNCE)) //debounce
        return;
    if(SaveDirNum!=dirnum)
        SaveDirNum=dirnum;
    if(i==0){// lock the list
        if(SaveLockState!=1)
            SaveLockState=1;
        DoBeeps(2,1,1);//one long beep
        #ifdef PRINT_LCD
        lcd_putsf_row(3,"List Locked");
        delay_ms(LCD_MSG_SHORT_DELAY);
        #endif
        return;
    }
    #ifdef PRINT_LCD
    itoa(dirnum,lcdnum);
    lcd_puts_row(0,lcdnum);
    lcd_putsf(".");
    lcd_puts(tmpimage.progname);
    lcd_putsf_row(3,"Programming ....");
    #else
    DisableNextDir=TRUE;
    #ifdef SEG7
    show7seg(dirnum);
    //show7seg('-');
    #endif
    #endif
    StandaloneMode=RUNNING;
    LEDS_OFF();
    //#asm("cli")
    Counter10ms=0;
    StandalonProg();    
    //#asm("sei")
    #ifdef PRINT_LCD
    //lcd_init(16);
    #endif
}

void DownButtonAction()
{
   int i;
   #ifdef DEBUG_BUTTONS;
   lcd_putsf_row(0,"Down");
   #endif
   if(StandaloneMode!=IDLE && StandaloneMode!=SDFAIL){
     //DoBeeps(3,1,1);
     return;
   }
   if(SaveLockState!=0){
        #ifdef PRINT_LCD
        lcd_putsf_row(3,"List Locked");
        #endif
        Beeps(3,1,1);
        return;
   }
   for(i=NUM_OF_TICKS;i>0;i--){  //if down button pressed about 10 sec - unlock list function
        delay_ms(TICK_DELAY);
        if(i==(NUM_OF_TICKS-DEBOUNCE))
            DoBeeps(1,1,1);
        if(TestButton(BUTTON_DOWN)==FALSE)
            break;
        
   }
   if(i==0){
        Beeps(3,1,1);//fail beep the button pressed too much time
        return;
   }
   if(i>(NUM_OF_TICKS-DEBOUNCE)){ //debounce
        return;
   }
   
   if(dirnum){
    if(dirnum <= g_DownSkip)
        dirnum=g_MaxFolders;
    else 
        dirnum-=g_DownSkip;
   }
   #ifdef PRINT_DEBUG
   while(tx_counter);
   printf("Current Folder %d\r\n",dirnum);
   while(tx_counter);
   #endif
   StandaloneMode=READSD;
   #ifdef PRINT_LCD
   if(lcd_GetProgName(0)==TRUE){
       StandaloneMode=IDLE;
       Beeps(1,1,1);
   }
   else{
       Beeps(2,1,2);
       StandaloneMode=SDFAIL;
       return;
   }
   #else
   if(Call_GetDir()==TRUE){
        StandaloneMode=IDLE;
        DoBeeps(1,dirnum,1);
   }
   else{
        StandaloneMode=SDFAIL;
        Beeps(2,1,2);
        return;
   }
   #endif
   StandaloneMode=IDLE;
   LEDS_OFF();
}

void StopButtonAction()
{
   int i;   
   #ifdef DEBUG_BUTTONS;
   lcd_putsf_row(0,"Stop");
   #endif
   if(StandaloneMode==RUNNING){
     Beeps(3,1,1);
     return;
   }
   for(i=NUM_OF_TICKS;i>0;i--){  //if down button pressed about 10 sec - unlock list function
        delay_ms(TICK_DELAY);
        #ifdef PRINT_LCD 
        if(TestButton(BUTTON_STOP)==FALSE)
        #else
        if(TestButton(BUTTON_DOWN)==FALSE)
        #endif
            break;
        if(i==(NUM_OF_TICKS-DEBOUNCE)){
            Beeps(1,1,1);//short beep once
            if(StandaloneMode==FAIL || StandaloneMode==SDFAIL)
                break; 
        }
   }
   if(i>(NUM_OF_TICKS-DEBOUNCE)){ //debounce
        return;
   }
   if(StandaloneMode!=FAIL){
       if(i==0){//unlock the listing
            if(SaveLockState!=0)
                SaveLockState=0;
            Beeps(2,1,1);//long beep once
            #ifdef PRINT_LCD
            lcd_putsf_row(3,"List Unlocked");
            delay_ms(LCD_MSG_SHORT_DELAY);
            #endif
       }
   } 
   StopEvent=0;
   CanShow=0;
   #ifdef DEBUG_BUTTONS1;
   lcd_putsf_row(0,"After Stop");
   delay_ms(1000);
   #endif
   LEDS_OFF();    //
   //StandaloneMode=IDLE;
   StandaloneMode=READSD;
   Call_GetConfig();
   #ifdef PRINT_LCD   
   if(lcd_GetProgName(0)==TRUE){//read dir again
        StandaloneMode=IDLE;
        if(SaveDirNum!=dirnum)
            SaveDirNum = dirnum;
   }
   else{
     StandaloneMode=SDFAIL;
     Beeps(2,1,2);
   }
   #else
    if(Call_GetDir()==TRUE){
        StandaloneMode=IDLE;
        Beeps(1,dirnum,1);
    }
    else{
        //StandaloneMode=SDFAIL;
        StandaloneMode=SDFAIL;
        Beeps(3,1,1);
        return;
        /*
        Beeps(3,1,1);
        dirnum-=1;
        if(Call_GetDir()==TRUE){
            //Beeps(1,dirnum,1);
        }
        else{
            StandaloneMode=SDFAIL;
            Beeps(3,1,1);
            return;
        }
        */
    }
   #endif
   while(TestButton(BUTTONS_RELEASED)==FALSE);
}

#ifdef PRINT_LCD
UCHAR lcd_GetProgName(UCHAR row)
{
   char num1[6],num2[6] ;
   msg_buf_cnt=0;
   //itoa(folderCounter,num1);
   itoa(dirnum,num1);
   if(row>1)
        return FALSE;
   lcd_init(16);
   lcd_gotoxy(0,0);
   lcd_clear();
   lcd_putsf_row(0,"Read folder ");
   lcd_puts(num1);
   //lcd_putchar(dirnum+'0');
   lcd_gotoxy(0,1);
   //delay_ms(500);
   if(Call_GetDir()==TRUE){        
        //itoa(folderCounter,lcdnum);
        itoa(dirnum,lcdnum);
        lcd_puts_row(row,lcdnum);
        lcd_putsf(".");
        lcd_puts(tmpimage.progname);
        lcd_puts_row(1,tmpimage.fw_ver);
        lcd_puts_row(2,&tmpimage.fw_ver[15]);
        lcd_putsf_row(3,"");
        if(tmpimage.ProgCounterTotal!=0){
           
           ltoa(tmpimage.ProgCounterDone,num1);
           lcd_puts(num1);
           lcd_putsf(" of ");
           ltoa(tmpimage.ProgCounterTotal,num2);
           lcd_puts(num2);
        }
        else
            lcd_putsf("Free license.");
        CanShow=1;
        return TRUE;
   }
   else{
        lcd_putsf_row(1,"Folder ");
        itoa(dirnum,num1);
        //itoa(folderCounter,num1);
        lcd_puts(num1);
        lcd_putsf(" error.");
   }
   return FALSE;
}

void lcd_putsf_row3(UCHAR row,flash char *str)
{
    lcd_putsf_row(row,str); 
    return;
    msg_buf_cnt=1 ;
    while((msg_buf_cnt < (MSG_SIZE-1)) && (msg_buf[msg_buf_cnt][0]!=0)){
       msg_buf_cnt++;
    }
    MaxMsg=msg_buf_cnt;
}
void lcd_putsf_row(UCHAR row,flash char *str)
{
   //char strn[16];
   strncpyf(lcdnum,str,15);
   #ifdef PRINT_LCD
   if(row>3)
        return;
   //str[16]=0;
   lcd_gotoxy(0,row);
   lcd_putsf("                ");
   lcd_gotoxy(0,row);
   lcd_puts(lcdnum);
   delay_ms(LCD_MSG_DELAY);
   #endif
}
void MsgBufClr(void)
{   
   return;
   for(msg_buf_cnt=0;msg_buf_cnt<MSG_SIZE;msg_buf_cnt++)
     msg_buf[msg_buf_cnt][0]=0;
   msg_buf_cnt=0;
   CanChangeMsg=0;
   MaxMsg=msg_buf_cnt;
   lcd_putsf_row(3,"Ready to Start.");
}
void ChangeMsg(void)
{
    return;
    msg_buf_cnt++;
    if((msg_buf_cnt>=MSG_SIZE)||(msg_buf[msg_buf_cnt][0]==0)){
       msg_buf_cnt=1;
    }
    CanChangeMsg=0;
}
void lcd_puts_row(UCHAR row,char *str)
{
   #ifdef PRINT_LCD
   if(row>3)
        return;
   strncpy(str,str,15);
   lcd_gotoxy(0,row);
   lcd_putsf("                ");
   lcd_gotoxy(0,row);
   lcd_puts(str);
   delay_ms(LCD_MSG_DELAY);
   #endif
}
void lcd_puts_hex(UCHAR hexnum)
{
   #ifdef PRINT_LCD
   lcd_putchar(ntohexchar((hexnum>>4)&0x0f));
   lcd_putchar(ntohexchar(hexnum&0x0f));
   delay_ms(LCD_MSG_DELAY);
   #endif
}
#endif

#ifdef SEG7
void init7seg(void)
{
    DDRC=0x3F;//PC0-5 seg A-F outputs
    DDRD.7=1;//PD7 seg G outputs
    PORTC=0x3F;//all 1's all segments off
    PORTD.7=1; //seg G off   
}

void show7seg(char num)
{
    switch(num){
        case 0:
            PORTC=0;
            PORTD.7=1;
            break; 
        case 1:
            PORTC=0x39;//XX1111001
            PORTD.7=1;
            break;
        case 2:
            PORTC=0x24;//XX100100
            PORTD.7=0;
            break;
        case 3:
            PORTC=0x30;//XX110000
            PORTD.7=0;
            break;
        case 4:
            PORTC=0x19;//XX011001
            PORTD.7=0;
            break;
        case 5:
            PORTC=0x12;//XX010010
            PORTD.7=0;
            break;
        case 6:
            PORTC=0x02;//XX000010
            PORTD.7=0;
            break;
        case 7:
            PORTC=0x38;//XX111000
            PORTD.7=1;
            break;
        case 8:
            PORTC=0x00;//XX000000
            PORTD.7=0;
            break;
        case 9:
            PORTC=0x10;//XX010000
            PORTD.7=0;
            break;
        case 10:
            PORTC=0;
            PORTD.7=1;
            break;
        case '-':
            PORTC=0x3F;//XX111111
            PORTD.7=0;
            break; 
        case 'd':
            PORTC=0x21;//XX100001
            PORTD.7=0;
            break;
        case 'e':
            PORTC=0x04;//XX000100
            PORTD.7=0;
            break;
        case 'r':
            PORTC=0x2F;//XX101111
            PORTD.7=0;
            break;    
        case 'E':
            PORTC=0x06;//XX000110
            PORTD.7=0;
            break;
        case 'F':
            PORTC=0x0E;//XX001110
            PORTD.7=0;
            break;
        case 'P':
            PORTC=0x0C;//XX001100
            PORTD.7=0;
            break;    
        default:
        break;    
    }
}
#endif


unsigned char TestButton(UCHAR bt)
{
    if(TestADCs()==(1<<bt))
        return TRUE;
    else
        return FALSE;
}

unsigned char TestADCs()
{
    unsigned char StateOfButtons=0;
    ADC_DONE=FALSE;
    ADMUX=(6 | (ADC_VREF_TYPE & 0xff)); //ADC6 ch=6, ADC7 ch=7
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(20);
    // Start the AD conversion
    ADCSRA|=0x40;
    while(ADCSRA&(1<<ADSC));//while(ADC_DONE==FALSE);
    adc_data=ADCH;
    if(adc_data<50)
        StateOfButtons|=1<<BUTTON_DOWN;
    else if(adc_data<150)
        StateOfButtons|=1<<BUTTON_STOP;
    ADC_DONE=FALSE;
    ADMUX=(7 | (ADC_VREF_TYPE & 0xff)); //ADC6 ch=6, ADC7 ch=7
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(20);
    // Start the AD conversion
    ADCSRA|=0x40;
    while(ADCSRA&(1<<ADSC));//while(ADC_DONE==FALSE);
    adc_data=ADCH;
    if(adc_data<50)
        StateOfButtons|=1<<BUTTON_UP;
    else if(adc_data<150)
        StateOfButtons|=1<<BUTTON_START;
    if(StateOfButtons)
        return StateOfButtons;
    return (1<<BUTTONS_RELEASED);
}

void StartADC(unsigned char ch)
{
    //char num1[3];
    //unsigned char chan=ch;
    ADC_DONE=FALSE;
    ADMUX=(ch | (ADC_VREF_TYPE & 0xff)); //ADC6 ch=6, ADC7 ch=7
    // Delay needed for the stabilization of the ADC input voltage
    delay_us(20);
    // Start the AD conversion
    ADCSRA|=0x40;
    while(ADC_DONE==FALSE);
#ifdef DEBUG_BUTTONS
    if(adc_data>150)
        return;
    lcd_clear();
    lcd_putsf_row(0,"ADC");
    itoa(ch,num1);
    lcd_puts(num1);
    lcd_putchar(' ');
    itoa(adc_data,num1);
    lcd_puts(num1);
    if(chan==6){
        if(adc_data<50)
            lcd_putsf(" Down");
        else if(adc_data<150)
            lcd_putsf(" Stop");
    }
    else if(chan==7){
        if(adc_data<50)
            lcd_putsf(" Up");
        else if(adc_data<150)
            lcd_putsf(" Start");
    }
#endif
}

#define BEEP_ON_TIME_MS 100
#define BEEP_OFF_TIME_MS 100
#define TYPE_1  1
#define TYPE_2  2
/*
void TestBeeps(void)
{
    if(CanBeep){
        Beeps(DoTone,DoTimes,DoTypeLength);
        CanBeep=0;
    }
}
*/
void Beeps(UCHAR tone,UCHAR times,int type_length)
{
    CanBeep=1;
    if(tone==0)
        CanBeep=0;
    StopEvent=0;
    cnt_beep_ms=beep_ms=(BEEP_ON_TIME_MS*type_length);
    cnt_beep_times=beep_times=times;
    cnt_beep_tone=beep_tone=tone;
    cnt_beep_delay_ms=beep_delay_ms=0;
    /*
    for(;times&&CanBeep;times--)
    {
        for(beep_ms=(BEEP_ON_TIME_MS*type_length);beep_ms&&CanBeep;beep_ms--)         
        {
           BUZ_TGL();
           delay_ms(tone);
           if(CanBeep==0)
                break;
        }
        if(CanBeep==0)
                break;
        delay_ms(BEEP_OFF_TIME_MS);
    }
    
    CanBeep=0;
    //StopEvent=0;
    */
}

void DoBeeps(UCHAR tone,UCHAR times,int type_length)
{
   Beeps(tone,times,type_length);
   return;
//   CanBeep=1;
//   StopEvent=0;
//   DoTone=tone;
//   DoTimes=times;
//   DoTypeLength=type_length;
}