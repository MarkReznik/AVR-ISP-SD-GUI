#include "common.h"
#include "mysdcard.h"

const unsigned char roll[] =

          { 0x00,   0xE9, 0x79, 0x25, 0x54, 0x88, 0xDF, 0xA5, 0xB9, 0xF6, 0xC2,
                    0xAB, 0xFC, 0xB3, 0xB1, 0x37, 0xE1, 0x83, 0xD5, 0x97, 0x04,
                    0x38, 0xFE, 0x99, 0x26, 0xE4, 0x8E, 0x4B, 0xF7, 0x04, 0xC5,
                    0xA5, 0x1F, 0x88, 0x68, 0xEF, 0x1D, 0x39, 0xBC, 0x62, 0xD8,
                    0x63, 0x0F, 0x6A, 0xA5, 0x12, 0xAA, 0xE5, 0x8C, 0x75, 0x36,
                    0xBA, 0x81, 0x8F, 0x30, 0x3D, 0xEB, 0xE0, 0xFE, 0x70, 0xFA,
                    0x94, 0xAE, 0xB5, 0x31 };
#ifdef PETITFATFS
/* MMC/SD/SD HC card support */
#include "mmc.h"
/* PETIT FATFS support */
#include "pff.h"
#endif

/* sprintf */
#include <stdio.h>
/* string functions */
#include <string.h>


/* error message list */
flash char * flash error_msg[]=
{
"", /* not used */
"FR_DISK_ERR",
"FR_INT_ERR",
"FR_NOT_READY",
"FR_NO_FILE",
"FR_NO_PATH",
"FR_INVALID_NAME",
"FR_DENIED",
"FR_EXIST",
"FR_INVALID_OBJECT",
"FR_WRITE_PROTECTED",
"FR_INVALID_DRIVE",
"FR_NOT_ENABLED",
"FR_NO_FILESYSTEM",
"FR_MKFS_ABORTED",
"FR_TIMEOUT"
};

// Declare your global variables here
/* FAT function result */
FRESULT res;
/* number of bytes written/read to the file */
UINT nbytes,nbytesCounter;
//ULONG nSector;
/* will hold the information for logical drive 0: */
#ifndef PETITFATFS
FATFS fat;
/* will hold the file information */
FIL file;
/* will hold the file information */
FILINFO fno;
#else
FATFS fs;          /* Work area (file system object) for the volume */
#endif

/* file path */
char path[]="0:/10/file1234.txt";
/* file read buffer */
unsigned char buffer[512];//;
/* byte counter to save read bytes till now */
ULONG ReadByteCounter;
ULONG WriteByteCounter;
DWORD LastOffset;

void init_sd_tim1(void)
{
/* initialize Timer1 overflow interrupts in Mode 0 (Normal) */
    TCCR1A=0x00;
    /* clkio/1024 */
    //TCCR1B=(1<<CS12)|(1<<CS10);
    /* clkio/64 */
    TCCR1B=(1<<CS11)|(1<<CS10);
    /* timer overflow interrupts will occur with 100Hz frequency */
    TCNT1H=T1_INIT>>8;
    TCNT1L=T1_INIT&0xFF;
    /* enable Timer1 overflow interrupt */
    #ifdef _MEGA64_INCLUDED_
    TIMSK=1<<TOIE1;
    #else
    TIMSK1=1<<TOIE1;
    #endif
}
/* display error message and stop */
void RESerror(FRESULT res)
{
    if(res!=0)
        StandaloneMode=SDFAIL;
    #ifdef PRINT_DEBUG
    if(debug){
        while(tx_counter); 
    #ifdef PETITFATFS
    //if ((res>=FR_DISK_ERR) && (res<=FR_NO_FILESYSTEM))
    #else
    //if ((res>=FR_DISK_ERR) &&(res<=FR_TIMEOUT))
    #endif
       printf("\r\nERROR %d: %p",res, error_msg[res]);
       while(tx_counter);}
    #endif
    #ifdef PRINT_LCD
    lcd_putsf_row3(3 ,error_msg[res]);
    #endif
/* stop here */
//while(1);
}

UCHAR sd_mount(void)
{
    UCHAR retry=3;
    while(retry)
    {
        retry--;
        /* initialize SPI interface and card driver */
        #ifdef PETITFATFS
        if((res=disk_initialize())!=0){
        #else
        if((res=disk_initialize(0))!=0){//0=DRIVE NUMBER
        #endif
            #ifdef PRINT_DEBUG
                if(debug){
                while(tx_counter);
                printf("\r\nSPI Init Failed. Return status %d.\r\n",res);
                if (res & STA_NOINIT) printf("Disk init failed");
                else
                if (res & STA_NODISK) printf("Card not present");
                else
                if (res & STA_PROTECT) printf("Card write\nprotected");
                while(tx_counter);}
            #endif
            #ifdef PRINT_LCD
                if (res & STA_NOINIT) lcd_putsf_row3(3,"Disk init failed");
                else
                if (res & STA_NODISK) lcd_putsf_row3(3,"Card not present");
                else
                if (res & STA_PROTECT) lcd_putsf_row3(3,"Card write prot");                
            #endif
            if(retry){
                continue;
            }
            else{
                delay_ms(500);
                StandaloneMode=SDFAIL;
                return FALSE;
            }
        }
        /* mount logical drive 0: */
        #ifdef PETITFATFS
        if ((res=pf_mount(&fs))==FR_OK)
        #else
        if((res=f_mount(0,&fat))==FR_OK)
        #endif
        {
            #ifdef PRINT_DEBUG
            if(debug){
            while(tx_counter);
            printf("\r\nLogical drive 0: mounted OK\r\n");
            while(tx_counter);}
            #endif
        }
        else{
            if(retry)
                continue;
            else
                /* an error occured, display it and stop */
                goto print_error;
        }
        return TRUE;
    }
print_error:
    RESerror(res);
    return FALSE;
}

UCHAR SetRollValue(void)
{
    //char *pvalue=NULL,*pkey;
    UINT  i;//buffer index    
    UCHAR retry=0,rollValue;//ROLL_WINDOW;
    debug=1;
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\ntmpimage.RollCounter=%d, ",tmpimage.RollCounter);
    while(tx_counter);}
    #endif
    if(fileOpen()==0)
        goto print_error;
    SetConfigArea();
    #ifndef  PETITFATFS
    res=f_read(&file,buffer,sizeof(buffer),&nbytes);
    #else
    res=pf_read(buffer,sizeof(buffer),&nbytes);
    #endif
    if(res)
        goto print_error;
    buffer[nbytes]=NULL;
    while(retry<ROLL_SIZE)//RollCounterToCheck < ROLL_SIZE)
    {
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nRollCounterToCheck=%d, ",RollCounterToCheck);
        while(tx_counter);}
        #endif
       memcpy(tmpbytes,buffer,strlenf("[settings]"));
       rollValue=roll[retry];
       for(i=0;(i<strlenf("[settings]"))&& (rollValue>0) ;i++)
       {
            #ifdef PRINT_DEBUG
            if(debug){
            while(tx_counter);
            printf("\r\n%d=%02x, %02x ",i,tmpbytes[i],buffer[i]);
            while(tx_counter);}
            #endif
            tmpbytes[i]=(tmpbytes[i]<<1)|(tmpbytes[i]>>7);  //ROL
            tmpbytes[i]^=rollValue;        //XOR
            #ifdef PRINT_DEBUG
            if(debug){
            while(tx_counter);
            printf("\r\n%d=%02x, %c",i,tmpbytes[i],tmpbytes[i]);
            while(tx_counter);}
            #endif
       }
       if(strncmpf(tmpbytes,"[settings]",strlenf("[settings]"))==0){
            break;
       }
       retry++;
    }
    
    if(retry>=ROLL_SIZE)//return null in case of reach roll counter window
         goto print_error;
    tmpimage.rollValue=rollValue;
    if(rollValue>0){
        for(i=0; i<nbytes ;i++)
        {
            buffer[i]=(buffer[i]<<1)|(buffer[i]>>7);  //ROL
            buffer[i]^=rollValue;        //XOR
        }
    }
     #ifdef PRINT_DEBUG
     if(debug){
     while(tx_counter);
     buffer[nbytes-1]=0;
     printf("\r\n%s",buffer);
     while(tx_counter);}
     #endif    
    return TRUE;
print_error:
    #ifndef  PETITFATFS
    f_close(&file);
    #endif
    RESerror(res);
    #ifdef PRINT_LCD
    //lcd_putsf_row(1,"No project file");
    #endif
    return FALSE;
}

UCHAR fileOpen()
{
    UCHAR retry=3;
    /* open the file in read mode */
    #ifndef  PETITFATFS
    strcpyf(path,"0:/1/project.txt");
    path[3]=dirnum+'0';
    #else
    //strcpyf(path,"/00/00.dat");
    strcpyf(path,"/");
    //itoa(folderCounter,tmpbytes);
    itoa(dirnum,tmpbytes);
    strcat(path,tmpbytes);// "/1"
    strcatf(path,"/");// "/1/"
    //strcat(path,tmpbytes);// "/1/1"
    
    strcatf(path,"flash.dat");// "/1/1.dat"
    #endif
    while(retry)
    {
        retry--;
        /* open the file in read mode */
        #ifndef  PETITFATFS
        if((res=f_open(&file,path,FA_READ|FA_OPEN_EXISTING))==FR_OK)
        #else
        if ((res=pf_open(path))==FR_OK)
        #endif
        {
            #ifdef PRINT_DEBUG
            if(debug){
            while(tx_counter);
            printf("\r\nFile %s opened OK.\r\n",path);
            //printf("\r\n%s key to find - %p",CopyKey,Key);
            while(tx_counter);}
            #endif
            #ifdef  PETITFATFS
            if((res=pf_lseek(0))!=FR_OK)
                goto print_error;//RESerror(res);
            #endif
            break;
        }
        else{
           /* an error occured, display it and stop */
           #ifdef PRINT_DEBUG
           if(debug){
           while(tx_counter);
           printf("\r\nWrong path: %s\r\n",path);
           while(tx_counter);}
           #endif
           if(retry)
                continue;
           else
                goto print_error;//RESerror(res);
        }
    }
    return 1;
print_error:
    RESerror(res);
    return 0;
}

UCHAR SetFlashArea()
{
   #ifdef  PETITFATFS
   if((res=pf_lseek(1000))!=FR_OK){
        RESerror(res);
        return FALSE;
   }
   #endif
   return TRUE;
}
UCHAR SetConfigArea()
{
   #ifdef  PETITFATFS
   if((res=pf_lseek(0))!=FR_OK){
        RESerror(res);
        return FALSE;
   }
   #endif
   return TRUE;
}
UCHAR SetFileOffset(DWORD offset)
{
   #ifdef  PETITFATFS
   offset+=1000L;
   if((res=pf_lseek(offset))!=FR_OK){
        RESerror(res);
        return FALSE;
   }
   #endif
   return TRUE;
}
UCHAR SetBinFileOffset(DWORD offset) //start address 16bit=b,b end address 16bit=b,b
{
   #ifdef  PETITFATFS
   if((res=pf_lseek( offset ))!=FR_OK){
        RESerror(res);
        return FALSE;
   }
   #endif
   return TRUE;
}
UCHAR SetBinFlashOffset(DWORD offset)
{
   #ifdef  PETITFATFS
   if((res=pf_lseek((DWORD)1024L + offset ))!=FR_OK){
        RESerror(res);
        return FALSE;
   }
   #endif
   return TRUE;
}
UCHAR fileRead(void)
{
    /* read and display the file's content.
       make sure to leave space for a NULL terminator
       in the buffer, so maximum sizeof(buffer)-1 bytes can be read */
    UINT  i;//buffer index
    UCHAR rollVal=tmpimage.rollValue;//roll[(tmpimage.RollCounter)%ROLL_SIZE];
    #ifndef  PETITFATFS
    res=f_read(&file,buffer,sizeof(buffer),&nbytes);
    #else
    res=pf_read(buffer,sizeof(buffer),&nbytes);
    #endif
    if ((res)==FR_OK){
      if(rollVal!=0){
        for(i=0;i<(nbytes);i++)
           {
                buffer[i]=(buffer[i]<<1)|(buffer[i]>>7);  //ROL
                buffer[i]^=rollVal;        //XOR
           }
       }
    }
    else
        goto print_error;
    //buffer[nbytes]=NULL;
    LastOffset+=nbytes;
    nbytesCounter=0;
    return TRUE;
print_error:
    RESerror(res);
    return FALSE;
}
UCHAR readPageBin(UINT bytesToRead,UCHAR *pbuffer)
{
    /* read and display the file's content.
       make sure to leave space for a NULL terminator
       in the buffer, so maximum sizeof(buffer)-1 bytes can be read */
    UINT  i;//buffer index
    UCHAR rollValue=tmpimage.rollValue;
    if(SetBinFileOffset(LastOffset)==FALSE)
        goto print_error;
    #ifndef  PETITFATFS
    res=f_read(&file,pbuffer,bytesToRead,&nbytes);
    #else
    res=pf_read(pbuffer,bytesToRead,&nbytes);
    #endif
    if ((res)==FR_OK){
       for(i=0;i<(nbytes);i++)
       {
          if(rollValue!=0){  
            pbuffer[i]=(pbuffer[i]<<1)|(pbuffer[i]>>7);  //ROL
            pbuffer[i]^=rollValue;        //XOR  
          }
          g_Addr+=pbuffer[i];
       }
    }
    else
        goto print_error;
    for(i=nbytes;i<bytesToRead;i++)
    {
        pbuffer[i]=0xFF;
    }    
    //buffer[nbytes]=NULL;
    LastOffset+=nbytes;
    nbytesCounter=0;
    return 1;
print_error:
    RESerror(res);
    return 0;
}
UCHAR fileReadBin(void)
{
    /* read and display the file's content.
       make sure to leave space for a NULL terminator
       in the buffer, so maximum sizeof(buffer)-1 bytes can be read */
    UINT  i;//buffer index
    #ifndef  PETITFATFS
    res=f_read(&file,buffer,sizeof(buffer),&nbytes);
    #else
    res=pf_read(buffer,sizeof(buffer),&nbytes);
    #endif
    if ((res)!=FR_OK){ //
        goto print_error;
    }
    //buffer[nbytes]=NULL;
    nbytesCounter=0;
    return 1;
print_error:
    RESerror(res);
    return 0;
}

void fileClose(void)
{
    /* close the file */
    #ifndef  PETITFATFS
    if ((res=f_close(&file))==FR_OK)
    {
       #ifdef PRINT_DEBUG
       if(debug){
       printf("\r\nFile %s closed OK.",path);
       while(tx_counter);}
       #endif
    }
    else
       /* an error occured, display it and stop */
       RESerror(res);
    #endif
}
UCHAR pgm_read_byte_sd(void)
{
   UCHAR result;
   if(nbytesCounter>=nbytes){
        fileRead();
   }
   result=buffer[nbytesCounter++];
   ReadByteCounter++;
   return result;
}

/* Read file from path that defined before to buffer */
UCHAR ReadFileToBuffer()
{
   /* open the file in read mode */
    #ifndef  PETITFATFS
    if((res=f_open(&file,path,FA_READ|FA_OPEN_EXISTING))==FR_OK)
    #else
    if ((res=pf_open(path))==FR_OK)
    #endif
    {
       #ifdef PRINT_DEBUG
       if(debug){
       while(tx_counter);
       printf("\r\nFile %s opened OK.\r\n",path);
       while(tx_counter);}
       #endif
    }
    else{
       /* an error occured, display it and stop */
       #ifdef PRINT_DEBUG
       if(debug){
       while(tx_counter);
       printf("\r\nWrong path: %s\r\n",path);
       while(tx_counter);}
       #endif
       goto print_error;//RESerror(res);
    }
    #ifdef  PETITFATFS
    if((res=pf_lseek(0))!=FR_OK)
       goto print_error;//RESerror(res);
    #endif
    /* read and display the file's content.
       make sure to leave space for a NULL terminator
       in the buffer, so maximum sizeof(buffer)-1 bytes can be read */
    #ifndef  PETITFATFS
    if((res=f_read(&file,buffer,sizeof(buffer),&nbytes))==FR_OK)
    #else
    if((res=pf_read(buffer,sizeof(buffer),&nbytes))==FR_OK)
    #endif
    //buffer[nbytes]=NULL;
    nbytesCounter=0;
    return TRUE;
print_error:
    #ifndef  PETITFATFS
    f_close(&file);
    RESerror(res);
    #endif
    return FALSE;    
}

char *FindValuePointerByKeyFromBuffer(flash char *Key)
{
   char *pvalue=NULL;
   if((pvalue=strstrf(buffer,Key))==NULL){
         #ifdef PRINT_DEBUG
         if(debug){
         while(tx_counter);
         printf("\r\n%p Key not found.",Key);
         while(tx_counter);}
         #endif
   }
   else
    pvalue+=(strlenf(Key)+1);
   return pvalue;
}

/*Get string value from key string after '=' Key=Value\r\n */
char *ReadValue(flash char *Key)
{
    char *pvalue=NULL;
    if((pvalue=FindValuePointerByKeyFromBuffer(Key))!=NULL)
    {       
       strncpy(tmpbytes,pvalue,32);//copy string from value pointer to tmpbytes 32 bytes 
       pvalue=tmpbytes;//set pointer to tmpbytes
       pvalue[strpos(pvalue,'\r')]=NULL;
       #ifdef PRINT_DEBUG
       while(tx_counter);
       if(debug){
       while(tx_counter);
       printf(" Found Key %p=%s.\r\n",Key,pvalue);
       while(tx_counter);}
       #endif
    }
    else
        goto print_error;
    
    return pvalue;
print_error:
    #ifndef  PETITFATFS
    f_close(&file);
    RESerror(res);
    #endif
    return pvalue;
}

    


/*Set string value from key string after '=' Key=Value\r\n */
UCHAR WriteConfigValue(flash char *Key,char *Values, UINT len)
{
    char *pvalue=NULL;        
    if(ReadFileToBuffer()==NULL)
       goto print_error; 
    if((pvalue=FindValuePointerByKeyFromBuffer(Key))!=NULL)
    {   //*(pvalue)=ready_val;//=xxxxxxxxh
       while(len){
          *pvalue=*Values;
          pvalue++;
          Values++;
          len--;
       }
       #ifdef  PETITFATFS
       if((res=pf_lseek(0))!=FR_OK)
            goto print_error;//RESerror(res);
       if((res=pf_write(buffer,(nbytes-1),&nbytes))!=FR_OK)
            goto print_error;
       if((res=pf_write(0,0,&nbytes))!=FR_OK)
            goto print_error;
       #endif
       //pvalue[strpos(pvalue,'\r')]=NULL;
       #ifdef PRINT_DEBUG
       if(debug){
       printf(" Found Key %p=%s.\r\n",Key,pvalue);
       while(tx_counter);}
       #endif
    }
    else
        goto print_error;
    /* close the file */
    #ifndef  PETITFATFS
    if ((res=f_close(&file))==FR_OK)
    {
       #ifdef PRINT_DEBUG
       if(debug){
       printf("\r\nFile %s closed OK.\r\n",path);
       while(tx_counter);}
       #endif
    }
    else
       /* an error occured, display it and stop */
       goto print_error;//RESerror(res);
    #endif
    //goto print_error;//return the pointer to value
    return TRUE;
print_error:
    #ifndef  PETITFATFS
    f_close(&file);
    RESerror(res);
    #endif
    return FALSE;
}