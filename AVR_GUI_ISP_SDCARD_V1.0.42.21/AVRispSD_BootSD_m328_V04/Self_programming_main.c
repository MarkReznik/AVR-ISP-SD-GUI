/*

Bootloader from SD card for AVR ISD SD project
Version history:
0.1: Initial
0.2: Add Support only FAT32
0.3: Add Support FAT16 DOS
0.4: Removed debug points

*/
#include <io.h>
#include <delay.h>
#include "flash.h"
#include "Self_programming.h"

#include "spi_sdcard.h"

#define SDBUF_SIZE  512
#define PAGES_PER_SDBUF (SDBUF_SIZE/PAGESIZE)



unsigned char result[5], sdBuf[SDBUF_SIZE], testBuf[PAGESIZE], token, SectorsPerCluster;
unsigned long appStartAdr,adr,SectorsPerFat,fat_begin_lba;
unsigned long cluster_begin_lba,fat_file_adr,fat_file_next_adr,filesize,readbytes;
unsigned int RootEntryCnt, appPages, pagesCnt, bytesChecksum, checksumCnt;
//(unsigned long)fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
//(unsigned long)cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FAT);
//(unsigned char)sectors_per_cluster = BPB_SecPerClus;
//(unsigned long)root_dir_first_cluster = BPB_RootClus;
//void testWrite();
void errorSD(unsigned char error_step);
unsigned long buf2num(unsigned char *buf,unsigned char len);
unsigned char compbuf(const unsigned char *src,unsigned char *dest);
void (*app_pointer)(void) = (void(*)(void))0x0000;

enum partition_types{DOS=0,FAT16,FAT32} partition_type;

void main( void ){

  unsigned int i,j;
  unsigned char rollNum;
  //partition_types ;

  /* initialize the USART control register
   TX and RX enabled, no interrupts, 8 data bits */

#ifdef PRINT_DEBUG
UCSR0A=0x00;
UCSR0B=0x18;
UCSR0C=0x06;

//UBRR0H=UBRR >> 8;
UBRR0L=UBRR & 0xFF;
#else

// Analog Comparator: Off
//ACSR=0x80;
//setup ADC buttons
//DIDR0=0x00;
ADMUX=ADC_VREF_TYPE & 0xff;  //0x20 AREF=reference, Left align=only MSB resd., ch=0
ADCSRA=0x83;    //  Prescaler=8 (1Mhz), Freerun.
//ADCSRB=0x00;    //
#endif


LED_OK_DDR.PIN_LED_OK=1;
LED_OK_ON();
LED_ERR_DDR.PIN_LED_ERR=1;
LED_ERR_OFF();

delay_ms(250);

i=0;j=0;

//putstr("Start\r");
#ifndef PRINT_DEBUG
while(1)
{
    if((TestADCs()==((1<<BUTTON_DOWN)))&&(j>10))
    {
        LED_OK_OFF();LED_ERR_OFF();
        if(++j>20 && i==0){
            break;
        }
        i=0;
    }
    else
    {
        if(TestADCs()==((1<<BUTTON_STOP))){
            LED_OK_TGL(); //toggle OK LED to show initiated STOP button press. Wait for DOWN press
            if(j<11){
                j++;
            }
        }
        else{
            j=0;
            LED_ERR_TGL();
            //go to app after 1 sec
            if(++i>10){
                LED_OK_OFF();LED_ERR_OFF();
                app_pointer();

            }
        }
    }
    delay_ms(100);
    //i++;j++;
}
#endif

  if((result[0]=SD_init())!=SD_SUCCESS)
  {
    errorSD(1);
  }
  LED_OK_TGL();
  //putchar('i');
  // read MBR get FAT start sector
  if((result[0]=SD_readSingleBlock(0, sdBuf, &token))!=SD_SUCCESS)
  {
    errorSD(2);
  }
  LED_OK_TGL();
  //putchar('1');
  partition_type=FAT32;
  if(sdBuf[0x1C2]<6){
    partition_type=DOS;
  }
  else if(sdBuf[0x1C2]<8){
    partition_type=FAT16;
  }

#ifdef PRINT_DEBUG  
  //printhex("2",partition_type,1);
#endif  
  //select algoritm for FAT parse
  //if((compbuf("MSDOS",&sdBuf[3])==0)||(partition_type==7)||(partition_type==6)||(partition_type==0xE)||(partition_type==0xB)||(partition_type==0xC))// WIN parsing
  if(partition_type != DOS)
  {
    adr=buf2num(&sdBuf[445+9],4);//FAT start sector. 1 sector = 512 bytes

    //load and read FAT ID (1st) sector. Get FAT info. Secors per Cluster and etc..
    if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS)
    {
    //putchar('e');//putchar('r');//putchar(result[0]);//putchar(token);
        errorSD(3);
    }
    fat_begin_lba=buf2num(&sdBuf[0x0E],2);//reserved sectors
    fat_begin_lba+=adr;//first sector of FAT data
    //printhex("3a",fat_begin_lba,4);
  }
  else//DOS parsing
  {
    fat_begin_lba=buf2num(&sdBuf[0x0E],2);//(reserved sectors)first sector of FAT data
    //putchar('2');//putchar('b');
  }

  //printhex("3",fat_begin_lba,4);

  SectorsPerCluster=sdBuf[0x0D];// 8 sectors per cluster
  //printhex("SpC",SectorsPerCluster,1);

  //if((partition_type==4)||(partition_type==6)||(partition_type==0xE)) //FAT/12/16
  if(partition_type != FAT32)
  {
      SectorsPerFat=buf2num(&sdBuf[22],2); // 0xF10 for test sdcard
      RootEntryCnt= buf2num(&sdBuf[11],2)>>4;
  }
  else{//FAT32
      SectorsPerFat=buf2num(&sdBuf[0x24],4); // 0xF10 for test sdcard
      RootEntryCnt=0;
  }
  //printhex("SpF",SectorsPerFat,4);

  //read the FAT fils/directories info from Root Directory cluster (usually 2),Number_of_Reserved_Sectors (usually 0x20). Looking for Folder '0' and clucter of FLASH.DAT file
  //(unsigned long)fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
  //fat_begin_lba=adr+0x20;//first sector of FAT data
  //(unsigned long)cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FAT);
  //Number_of_FATs always 2. Offset 0x10 8bit
  cluster_begin_lba=fat_begin_lba+(2*SectorsPerFat);//number of sector where data begin
  //printhex("cbl",cluster_begin_lba,4);
  //read root dir (sector 2 but always offset 2 too then 0) to find folder 0 FAT reference. and find Flash.dat sector
  //lba_addr = cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;
  adr=cluster_begin_lba +(2-2)*SectorsPerCluster;
  //adr*=512UL;
  result[1]=0;
  LED_OK_TGL();
  for(i=0;i<SectorsPerCluster;i++)
  {
      if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS){
        errorSD(4);
      }
#ifdef PRINT_DEBUG
      printhex("4",adr,4);
#endif
      for(j=0;j<(16);j++)
      {
           //putchar('5');
           if((result[1]=compbuf("0          ",&sdBuf[j*32]))!=0)
           {
                break;
           }
      }
      //putchar('6');
      if(result[1]!=0)
      {
        //putchar('7');
        fat_file_adr =buf2num(&sdBuf[j*32+0x14],2)<<16;
        fat_file_adr|=buf2num(&sdBuf[j*32+0x1A],2);
        break;
      }
      else
      {
        adr++;
      }
      LED_OK_TGL();
  }
  if(result[1]==0)
  {
      //putchar('f');//putchar('0');
      errorSD(5);//folder '0' not found
  }

  adr=cluster_begin_lba+ RootEntryCnt +(fat_file_adr-2)*SectorsPerCluster;
  /*
  printhex("7",cluster_begin_lba,4);
  printhex("7",RootEntryCnt,4);
  printhex("7",fat_file_adr,4);
  printhex("7",SectorsPerCluster,4);
  printhex("7",adr,4);
  */

  LED_OK_TGL();
  for(i=0;i<SectorsPerCluster;i++)
  {
      //putchar('9');
#ifdef PRINT_DEBUG
      //printhex("8",adr,4);
#endif
      if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS)
      {
        errorSD(6);
      }
      //putchar('A');
      for(j=0;j<(16);j++)
      {
           //putchar('B');
           if((result[1]=compbuf("FLASH   ",&sdBuf[j*32]))!=0)
           {
                //putchar('C');
                break;
           }
      }
      if(result[1]!=0)
      {
        //putchar('D');
        //read 1st number of cluster where data placed
        fat_file_adr =buf2num(&sdBuf[j*32+0x14],2)<<16;
        fat_file_adr|=buf2num(&sdBuf[j*32+0x1A],2);
        filesize = buf2num(&sdBuf[j*32+0x1C],8);
        break;
      }
      else
      {
        adr++;
      }
      LED_OK_TGL();
  }

  if(result[1]==0)
  {
      //putchar('f');//putchar('0');
      errorSD(16);//folder '0' not found
  }

  //check FAT for chain of clusters to read
  readbytes=0;
  while((fat_file_adr != 0x0FFFFFFFUL) && (fat_file_adr != 0xFFFF))
  {
    //read where next cluster from FAT, check that not EOF
#ifdef PRINT_DEBUG
    //printhex("7a",fat_file_adr,4);
    //printhex("7b",fat_begin_lba,4);
#endif
    if((result[0]=SD_readSingleBlock(fat_begin_lba+(fat_file_adr>>8), sdBuf, &token))!=SD_SUCCESS){
        errorSD(7);
    }
    if(partition_type!=FAT32){ //FAT/12/16
        fat_file_next_adr=buf2num(&sdBuf[(fat_file_adr<<1)%0x200],2);
    }
    else{//FAT32
        fat_file_next_adr=buf2num(&sdBuf[(fat_file_adr<<2)%0x200],4);
    }
#ifdef PRINT_DEBUG
    //printhex("7c",fat_file_next_adr,4);
#endif
    //putchar('E');
    adr=cluster_begin_lba+ RootEntryCnt +(fat_file_adr-2)*SectorsPerCluster;
    for(i=0;i<SectorsPerCluster;i++)
    {
        //read data from next sector of file cluster
#ifdef PRINT_DEBUG
        //printhex("8",adr,4);
#endif
        if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS){
            errorSD(8);
        }
        //putchar('F');
        //address 2000 = start adr flash app 3 bytes, flash pages 2 bytes, checksum 2 bytes
        //app bytes starts from 2048, roll 0x88
        if(readbytes==0)
        {
            for(rollNum=1;rollNum!=0;rollNum++)
            {
                for(j=0;j<10;j++)
                {
                    testBuf[j]=(sdBuf[j]<<1)|(sdBuf[j]>>7);  //ROL
                    testBuf[j]^=rollNum;//0x88;  //XOR
                }
                if(compbuf("[settings]",testBuf))
                {
                    //printhex("rol=",rollNum,1);
                    break;
                }
            }
            if(rollNum==0){
                 errorSD(9);
            }
        }
        for(j=0;j<512;j++)
        {
            sdBuf[j]=(sdBuf[j]<<1)|(sdBuf[j]>>7);  //ROL
            sdBuf[j]^=rollNum;//0x88;  //XOR
            checksumCnt+=sdBuf[j];
            //if(readbytes>2000)
                ////putchar(sdBuf[j]);
        }
        readbytes+=512;
        //read app data
        if(readbytes>2048)
        {
           for(pagesCnt=0;pagesCnt<PAGES_PER_SDBUF;pagesCnt++)
           {
               LED_OK_TGL();
               if(WriteFlashPage(appStartAdr, &sdBuf[pagesCnt*(int)PAGESIZE])==0)
               {
                    errorSD(10);
               }
               appStartAdr+=PAGESIZE;
               appPages--;
               if(appPages==0)
               {
                    LED_OK_OFF();LED_ERR_OFF();
                    //putchar('P');putchar('\r');
                    app_pointer();
               }
           }
        }
        //read app start adr, num of pages, checksum
        else if(readbytes>=2000)//Offset=512-48=464
        {
           if(PAGESIZE!=((((unsigned int)sdBuf[478]<<8)|((unsigned int)sdBuf[479]))<<1))
           {
               errorSD(11);
           }
           appStartAdr=(unsigned long)sdBuf[464]<<16;
           appStartAdr|=(unsigned long)sdBuf[465]<<8;
           appStartAdr|=(unsigned long)sdBuf[466];
           appPages=(unsigned int)sdBuf[467]<<8;
           appPages|=(unsigned int)sdBuf[468];
           bytesChecksum=(unsigned int)sdBuf[469]<<8;
           bytesChecksum|=(unsigned int)sdBuf[470];
           checksumCnt=0;
           //putchar('a');//putchar('d');//putchar('r');//putchar(sdBuf[464]);//putchar(sdBuf[465]);//putchar(sdBuf[466]);
        }
        if((fat_file_next_adr == 0x0FFFFFFFUL) || (fat_file_next_adr == 0xFFFF)){
            if(readbytes >= filesize)
            {
                break;
            }
            else if(i>=(SectorsPerCluster-1))
            {
                //printhex("12a",readbytes,4);
                //printhex("12b",filesize,4);
                errorSD(12);
            }
        }
        adr++;
    }
    fat_file_adr = fat_file_next_adr;
  }
  while(1);
}

unsigned char compbuf(const unsigned char *src,unsigned char *dest)
{
    while(*src)
    {
        if(*src++ != *dest++)
            return 0;
        //src++;dest++;
        //len--;
    }
    return 1;
}

void errorSD(unsigned char error_step)
{
    unsigned char i;
#ifdef PRINT_DEBUG
    printhex("err:",error_step,1);
#endif
    PORTC.1=0;
    LED_OK_OFF();
    for(i=0;i<error_step;i++){
        LED_ERR_OFF();
        delay_ms(300);
        LED_ERR_ON();
        delay_ms(300);
    }

    while(1);
}

unsigned long buf2num(unsigned char *buf,unsigned char len)
{
    unsigned long num=0;
    //unsigned char i;
    for(;len>0;len--)
    {
        num<<=8;
        num|=buf[len-1];
    }
    return num;
}

#ifndef PRINT_DEBUG
unsigned char TestADCs()
{
    unsigned char StateOfButtons=0;
    unsigned char adc_data;
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

#endif