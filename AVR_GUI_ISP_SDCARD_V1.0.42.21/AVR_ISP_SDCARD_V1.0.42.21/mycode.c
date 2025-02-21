#include "common.h"
#include <eeprom.h>

//Standalone globals
//#define  FLASH_START_ADDR_WORD  15000
//#define  FLASH_START_ADDR_BYTE  (FLASH_START_ADDR_WORD<<1)

//UCHAR chipname[12]; // chip signature compare variable
//UCHAR signature[3]; // chip signature compare variable

//ULONG chipsize =0 ; //chip size in bytes
//UINT pagesize;// target flash page size in bytes, i count bytes in page buffer
//UCHAR progfuses[MAX_FUSES];	       /* fuses to set during programming */    //Lock,Low,High,Ext
//UCHAR normfuses[MAX_FUSES];	       /* fuses to set after programming */
//UCHAR maskfuses[MAX_FUSES];
//UCHAR chipcode;                  /* device code ie m64 is 0x45 */
UCHAR tmpbytes[32];//resetprogcounter=00000000h
UCHAR tador=0;
UCHAR g_UpSkip;
UCHAR g_DownSkip;
UCHAR g_MaxFolders;
int   g_CpuFreq;
UCHAR spidiv;
UCHAR g_MyUDR;
UCHAR g_CalibBytes[4];
char msg_buf[MSG_SIZE][16];
char msg_buf_cnt=0;
char CanShow=0;

ULONG pageaddr = 0; //page address byte counter
volatile STANDALONE_MODE StandaloneMode=IDLE;
UCHAR pageBuffer[512];		       /* One page of flash in bytes = words/2 */
bit CanContinueRead=0;
bit EndOfFile=0;
ULONG OffsetX16=0;
UCHAR Save_i;
UCHAR Save_cksum = 0;
UCHAR Save_len;
bit blankpage; // flag is true if can skip page
UCHAR debug=0;
UCHAR *value;  //sdcard/tmpbytes[32] return string pointer if found
//bit FromSD;
//UCHAR *hextext;//
UCHAR dirnum;//number of project directory
UCHAR   folderCounter; //folders inccrement counter
eeprom  UCHAR   SaveDirNum  @5;  //eeprom cell to save last project directory number
eeprom  UCHAR   SaveLockState  @6;  //eeprom to list folder prees Up 10 sec to lock press Start 10 sec
char temptext[30];

//eeprom eeimage_t myimage[MAX_PROJECTS] @10;
eeprom eeimage_t myimage[MAX_FOLDERS] @10;
image_t tmpimage;
//flash byte hextext[40000] ;//,*hextextpos;
   //hextext =(byte flash  *)2000;

//error function. print error
void error(flash char *str)
{
#ifdef PRINT_DEBUG
   if(debug){
   while(tx_counter);
   printf("\r\nError.fptr=%u,buf[0]=%02x,buf[1]=%02x\r\n",(fs.fptr),buffer[0],buffer[1]);
   printf(str);
   while(tx_counter);}
#endif
#ifdef PRINT_LCD
    //lcd_putsf_row(3,str);
#endif
//while(1);
}

//read byte from byte prointer to SRAM
UCHAR pgm_read_byte(UCHAR *addr)
{
    return *addr;
}

//read byte from hextext byte pointer to SRAM then increment it
UCHAR hextext_read_byte(void)
{
    #ifdef BOOTEEPROM
    return *hextext++;
    #else
    return pgm_read_byte_sd();
    #endif
}

//convert char hex digit
UCHAR hexton (UCHAR h)
{
  if (h >= '0' && h <= '9')
    return(h - '0');
  if (h >= 'A' && h <= 'F')
    return((h - 'A') + 10);
  #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nBad hex=%02x\r\n",h);
    while(tx_counter);}
    #endif
  error("Bad hex digit!");
  return 0;
}
CHAR ntohexchar (UCHAR n)
{
  n&=0x0f;
  if (n >= 0 && n <= 9)
    return(n + '0');
  if (n >= 0xA && n <= 0xF){
    return (n-0x0A+'A');
  }
  return 'X';
}
UCHAR * ntohexstr (UCHAR n)
{
  static UCHAR str[3];
  str[0]=ntohexchar(n>>4);
  str[1]=ntohexchar(n&0x0f);
  str[2]=0;
  return str;
}
//convert 2 first chars in string to 2 hex digits that represent UCHAR
UCHAR hextobyte(char *str)
{
  return hexton(*str)<<4 | hexton(*(str+sizeof(UCHAR))); //read 0xFF  1=F, 1=F sizeof(UCHAR)=1 byte
}

//get bytes from string and put to array in eeprom. Exm: RAM str="76"(0x37,0x36) -> EEPROM arr[0]=0x76
void hextobytese(char *str,eeprom UCHAR *arr,UCHAR len)
{
  UCHAR i;
  for(i=0;i<len;i++){
   arr[i]=hextobyte(str+(i*2*sizeof(UCHAR)));
  }
}

//get bytes from string and put to array in SRAM. Exm: RAM str="76"(0x37,0x36) -> SRAM arr[0]=0x76
void hextobytes(char *str,UCHAR *arr,UCHAR len)
{
  UCHAR i;
  for(i=0;i<len;i++){
   arr[i]=hextobyte(str+(i*2*sizeof(UCHAR)));
  }
}

//convert 4 first chars in string to 4 hex digits that represent UINT
UINT hextouint(char *str)
{
  return (UINT)((UINT)hextobyte(str)<<8 | (UINT)hextobyte(str+2*sizeof(UCHAR))); //read 0xFFFF 1=F 2=F, 1=F 2=F   sizeof(UINT)=2 bytes
}

//convert 8 first chars in string to 8 hex digits that represent ULONG
ULONG hextoulong(char *str)
{
  return (ULONG)((ULONG)hextouint(str)<<16 | (ULONG)hextouint(str+2*sizeof(UINT)));//read 0xFFFFFFFF 1=F 2=F 3=F 4=F, 1=F 2=F 3=F 4=F sizeof(ULONG)=4 bytes
}

/*Copy null terminated string, from pointer to SRAM to pointer to eeprom*/
void strcpye(eeprom char *str1,char *str2)
{
   while(*str2){
      *str1= *str2;
      str1++;str2++;
   }
   str1=0;
}

/*Copy number of bytes, from pointer to SRAM to pointer to eeprom*/
void memcpye(eeprom UCHAR *str1,UCHAR *str2,UCHAR len)
{
   while(len){
      *str1= *str2;
      str1++;str2++;
      len--;
   }
}

/*Copy null terminated string, from pointer to eeprom -> to pointer to SRAM*/
void strecpy(char *str1,eeprom char *str2)
{
   while(*str2){
      *str1= *str2;
      str1++;str2++;
   }
   str2=0;
}
/*Copy null terminated string, from pointer to eeprom -> to pointer to SRAM n chars*/
void strencpy(char *str1,eeprom char *str2,char num)
{
   while(*str2 && num){
      *str1= *str2;
      str1++;str2++;
      num--;
   }
   str2=0;
}
/*Copy number of bytes, from pointer to eeprom -> to pointer to SRAM*/
void memecpy(UCHAR *str1,eeprom UCHAR *str2,UCHAR len)
{
   while(len){
      *str1= *str2;
      str1++;str2++;
      len--;
   }
}

/* compare RAM string with EEPROM string. If RAM > EEPROM return 1*/
signed char strcmpe(char *str1,eeprom char *str2)
{
   while(*str1 && *str2){
      if(*str1> *str2)return 1;
      if(*str1< *str2)return -1;
      str1++;str2++;
   }
   return 0;
}

void PrintImage(void){
#ifdef PRINT_DEBUG
        if(debug){
        printf("\r\ndebug=%d",tmpimage.debug);
        while(tx_counter);
        printf("\r\nfilename = %s",tmpimage.hexfile);
        while(tx_counter);
        printf("\r\nfw_ver = %s",tmpimage.fw_ver);
        while(tx_counter);
        printf("\r\nchipname = %s",tmpimage.chipname);
        while(tx_counter);
        printf("\r\nchipcode = 0x%x",tmpimage.chipcode);
        while(tx_counter);
        printf("\r\nsignature = %02x %02x %02x",tmpimage.signature[0],tmpimage.signature[1],tmpimage.signature[2]);
        while(tx_counter);
        printf("\r\nprogfuses = %02x %02x %02x %02x",tmpimage.progfuses[0],tmpimage.progfuses[1],tmpimage.progfuses[2],tmpimage.progfuses[3]);
        while(tx_counter);
        //memecpy(normfuses,myimage.normfuses,4);
        printf("\r\nnormfuses = %02x %02x %02x %02x",tmpimage.normfuses[0],tmpimage.normfuses[1],tmpimage.normfuses[2],tmpimage.normfuses[3]);
        while(tx_counter);
        //memecpy(maskfuses,myimage.maskfuses,4);
        printf("\r\nmaskfuses = %02x %02x %02x %02x",tmpimage.maskfuses[0],tmpimage.maskfuses[1],tmpimage.maskfuses[2],tmpimage.maskfuses[3]);
        while(tx_counter);
        printf("\r\nchipsizebytes = %lu",tmpimage.chipsize);
        while(tx_counter);
        printf("\r\npagesizebytes = %u",tmpimage.pagesizebytes);
        while(tx_counter);
        printf("\r\neepromsizebytes = %lu",tmpimage.eepromsizebytes);        
        while(tx_counter);
        printf("\r\nProgramming Done = %lu",myimage[dirnum].ProgCounterDone);
        while(tx_counter);
        printf("\r\nProgramming Available = %lu",tmpimage.ProgCounterTotal);
        while(tx_counter);
        }
#endif
}


/*Function for Boot From EEPROM version (SDCARD hex copied to FLASH).
    Copy the project definitions from eeprom to SRAM*/
#ifdef BOOTEEPROM
void CopyEepromToRam(void)
{
    strecpy(tmpimage.chipname,myimage[dirnum].chipname);
    tmpimage.chipcode=myimage[dirnum].chipcode;
    memecpy(tmpimage.signature,myimage[dirnum].signature,3);
    memecpy(tmpimage.progfuses,myimage[dirnum].progfuses,MAX_FUSES);
    memecpy(tmpimage.normfuses,myimage[dirnum].normfuses,MAX_FUSES);
    memecpy(tmpimage.maskfuses,myimage[dirnum].maskfuses,MAX_FUSES);
}
#endif
void SetHiSpeed(void)
{
    //UBRR0L = 6; //0b0000.0110  = SCK 1Mhz
    //UBRR0L = 3; //0b0000.0011  = SCK 2Mhz
    //UBRR0L = 1; //0b0000.0001  = SCK 3Mhz
    UBRR0L=0; // SCK = FOSC/2 = (8MHz Int RC)/2 = 4Mhz
    //if(spidiv<5)
    if(spidiv<6)
    {    
        if(UBRR0L==0 && spidiv==1){
            UBRR0L=1;    
        }    
        else if(UBRR0L==0 && spidiv>1){
            UBRR0L=1;
            UBRR0L <<= (spidiv-1); 
        }
        else
            UBRR0L <<= spidiv;
    }
    else
        SetLowSpeed();    
}
void SetLowSpeed(void)
{
    UBRR0L = 0xFF; 
}
UCHAR TryDecSpeed(void)
{
    if((UBRR0L&0x80)==0){
        spidiv++;                
        SetHiSpeed();
        return TRUE;
    }
    else
        return FALSE;
}
UCHAR Call_EnterProgMode(UCHAR IsLowSpeed)
{
    UCHAR retry=1;
    if(IsLowSpeed)
        SetLowSpeed();
    else
        SetHiSpeed();
    while(retry){
          
        #ifdef PRINT_DEBUG
        printFlashString("\r\nStarting Program Mode... ");
        #endif
        #ifdef PRINT_LCD        
        if(CanShow){
            itoa((int)((int)(g_CpuFreq/2)/((int)(UBRR0L)+(int)1)),tmpbytes);   // 1=0,1 2=0,1,2
            strcatf(tmpbytes,"Khz  ");
            lcd_puts_row(3,tmpbytes);
            delay_ms(LCD_MSG_LONG_DELAY);
        }
        delay_ms(50);
        lcd_putsf_row(3,"Enter Prog Mode");
        if(CanShow)
            delay_ms(LCD_MSG_SHORT_DELAY);
        #endif
        AVR910_Standalone("P");//send enter to prog mode -> should answer 0x0D=ACK  /* Turn on target power */
        if(g_txBuf[0]!=ACK){            
            if(TryDecSpeed())
                continue;
            retry=0;                            
            #ifdef PRINT_DEBUG
            printFlashString("FAIL");
            #endif
            #ifdef PRINT_LCD
            lcd_putsf_row3(3,"No target");
            #endif
        }
        else if(Call_ReadSignature()==FALSE){
            if(TryDecSpeed())
                continue;
            retry=0;
        }
        else
            break;
    }
    if(retry==0)
       return FALSE;    
    #ifdef PRINT_DEBUG
    printFlashString(" OK.");
    #endif
    #ifdef PRINT_LCD
    lcd_putsf(" Done");
    #endif    
    if(IsLowSpeed==FALSE)
        tmpimage.spidiv=spidiv;
    return TRUE;
}
UCHAR Call_GetConfig(void){
    #ifdef PRINT_LCD
    lcd_init(16);
    lcd_gotoxy(0,0);
    #endif
    if(sd_mount()==FALSE){// if sdcard is missing
        #ifdef PRINT_DEBUG
        printFlashString("\r\nCall sd_mount func failed \r\n");
        #endif
        Beeps(2,1,1);
        goto print_error;
    }
    else{//try to copy image definitions from sdcard to eeprom and reset to bootloader to load hex to flash
        #ifdef PRINT_DEBUG
        ("\r\nCall sd_mount func ok \r\n");
        #endif       
        strcpyf(path,"/0/config.ini");
        if(ReadFileToBuffer()==FALSE)
              goto print_error;
        tador=0;
        if((value=ReadValue("upO"))!=NULL)
            if(atoi(value)==1)
                if((value=ReadValue("downO"))!=NULL)
                    if(atoi(value)==1)
                        tador=1;
        if(tador!=1){    
            if((value=ReadValue("tador"))==NULL)
                tador=0;
            else{    
                tador=atoi(value);
            }
        }
        
        if(tador==0){
            //clear eeprom case
            if((value=ReadValue("up0"))!=NULL)
                if(atoi(value)==1)
                    if((value=ReadValue("down0"))!=NULL)
                        if(atoi(value)==1)
                        {
                            for(folderCounter=0;folderCounter<MAX_FOLDERS;folderCounter++)
                               {
                                   #ifdef PRINT_LCD_DEBUG
                                   lcd_putsf_row(1,"");
                                   //lcd_puts_hex(buffer[9]);
                                   lcd_puts_hex(buffer[9]&0x0f);
                                   //lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
                                   lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]&0x0f);
                                   lcd_puts_hex(buffer[10]);
                                   lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]);
                                   //delay_ms(250);
                                   #endif
                                   myimage[folderCounter].FolderUniqueValue[0]=0xFF;
                                   myimage[folderCounter].FolderUniqueValue[1]=0xFF;
                                   myimage[folderCounter].FolderUniqueValue[2]=0xFF; 
                               }
                            #ifdef PRINT_LCD
                            lcd_putsf_row(0,"Device erased");
                            lcd_putsf_row(1,"Remove eras SD");
                            lcd_putsf_row(2,"Insert norm  SD");
                            lcd_putsf_row(3,"Power cycle");
                            #endif
                            StandaloneMode=PASS;
                            Beeps(1,1,1500);
                            while(1);
                        }
            //check if virgin case
            if((value=ReadValue("virgin"))!=NULL)
            {
                if( eeprom_read_byte(4) != 0x44 )
                {
                    eeprom_update_byte(4,0x44);   
                }
                //check remove virgin case   
                if( eeprom_read_byte(4) == 0x44 ){
                    #ifdef PRINT_LCD
                    lcd_putsf_row(0,"Device unlocked");
                    lcd_putsf_row(1,"Remove unlck SD");
                    lcd_putsf_row(2,"Insert norm  SD");
                    lcd_putsf_row(3,"Power cycle");
                    #endif
                    StandaloneMode=PASS;
                    Beeps(1,1,1500);; 
                }
                else{
                    #ifdef PRINT_LCD
                    lcd_putsf_row(0,"Failed unlock");
                    lcd_putsf_row(1,"EEPROM issue");
                    #endif
                    StandaloneMode=FAIL;
                    Beeps(3,3,500);
                }
                while(1);
            }
            
        }//end if(tador==0)    
        if((value=ReadValue("up"))==NULL)
            if((value=ReadValue("upo"))==NULL)
                if((value=ReadValue("upO"))==NULL) 
                    if((value=ReadValue("up0"))==NULL)
                        goto print_error;
        g_UpSkip=atoi(value);
        if((value=ReadValue("down"))==NULL)
            if((value=ReadValue("downo"))==NULL)
                if((value=ReadValue("downO"))==NULL)
                    if((value=ReadValue("down0"))==NULL)
              goto print_error;
        g_DownSkip=atoi(value);
        if((value=ReadValue("maxfolders"))==NULL)
              goto print_error;
        g_MaxFolders=atoi(value);
        if(g_MaxFolders>MAX_FOLDERS)
              g_MaxFolders=MAX_FOLDERS;
        if((value=ReadValue("cpu_freq"))==NULL)
            g_CpuFreq=8000;//goto print_error;
        else{
                g_CpuFreq=atoi(value);
            }
        return TRUE;
    }
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nconfig: Up=%d Down=%d MaxFolders=%d",g_UpSkip,g_DownSkip,g_MaxFolders);
    while(tx_counter);}
    #endif
print_error:
    return FALSE;    
}

UCHAR Call_GetDir(void){
    UCHAR i;
    ResetGlobals();
    if(sd_mount()==FALSE){// if sdcard is missing
        #ifdef PRINT_DEBUG
        printFlashString("\r\nCall sd_mount func failed \r\n");
        #endif
        Beeps(2,1,2);
        return FALSE;
    }
    else{//try to copy image definitions from sdcard to eeprom and reset to bootloader to load hex to flash
        #ifdef PRINT_DEBUG
        printFlashString("\r\nCall sd_mount func ok \r\n");
        #endif
       //myimage[dirnum].RollCounter=63;//test case
       //tmpimage.RollCounter=myimage[dirnum].RollCounter; //63
        if( SetRollValue()==FALSE )
              return FALSE;
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nbuffer\r\n%s",buffer);
        while(tx_counter);}
        #endif
       
       
       if((value=ReadValue("pagesize"))==NULL)
            return FALSE;
       tmpimage.pagesizebytes=(atoi(value))<<1;
       //tmpimage.pagesizebytes=hextouint(value);
       if((value=ReadValue("chipsize"))==NULL)
            return FALSE;
       tmpimage.chipsize=atol(value);
       if((value=ReadValue("eepromsize"))==NULL)
            return FALSE;
       tmpimage.eepromsizebytes=atol(value);
       //tmpimage.chipsize=hextoulong(value);
       if((value=ReadValue("maskfuses"))==NULL)
            return FALSE;
       hextobytes(value,tmpimage.maskfuses,MAX_FUSES);
       if((value=ReadValue("normfuses"))==NULL)
            return FALSE;
       hextobytes(value,tmpimage.normfuses,MAX_FUSES);
       if((value=ReadValue("progfuses"))==NULL)
            return FALSE;
       hextobytes(value,tmpimage.progfuses,MAX_FUSES);
       if((value=ReadValue("signature"))==NULL)
            return FALSE;
       hextobytes(value,tmpimage.signature,3);
       tmpimage.spidiv=0;
       if((value=ReadValue("spidiv"))!=NULL)
       { 
            tmpimage.spidiv=atoi(value); 
            //tmpimage.spidiv=hextobyte(value);            
       }       
       if(tmpimage.spidiv>7)
            tmpimage.spidiv=0;
       spidiv=tmpimage.spidiv;     
       strncpy(tmpimage.fw_ver,value=ReadValue("proginfo"),30);
       strncpy(tmpimage.progname,value=ReadValue("progname"),12);        
       
       if(fileRead()==FALSE)
            return FALSE;//from byte 512+       
      
        
       buffer[0]^=0x77; 
       buffer[1]^=0x77;
       buffer[2]^=0x77;
       buffer[3]^=0x77;
       buffer[4]^=0x77;
       buffer[5]^=0x77;
       buffer[6]^=0x77;
       buffer[7]^=0x77;
       buffer[8]^=0x77;
       buffer[9]^=0x77;
       buffer[10]^=0x77;
       buffer[11]^=0x77;
       
       #ifdef PRINT_LCD_DEBUG
       lcd_putsf_row(2,"");
       lcd_puts_hex(buffer[1]);
       lcd_puts_hex(buffer[2]);
       lcd_puts_hex(buffer[4]);
       lcd_puts_hex(buffer[5]);
       lcd_puts_hex(buffer[9]);
       lcd_puts_hex(buffer[10]);
       //lcd_puts_hex(g_MaxFolders);
       delay_ms(1000);
       #endif
       if(tador==0){       
           //check for unique file name
           for(folderCounter=0;folderCounter<MAX_FOLDERS;folderCounter++)
           {
               #ifdef PRINT_LCD_DEBUG
               lcd_putsf_row(1,"");
               //lcd_puts_hex(buffer[9]);
               lcd_puts_hex(buffer[9]&0x0f);
               //lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
               lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]&0x0f);
               lcd_puts_hex(buffer[10]);
               lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]);
               //delay_ms(250);
               #endif
               if((myimage[folderCounter].FolderUniqueValue[1]&0x0f)==(buffer[9]&0x0f))
                    if((myimage[folderCounter].FolderUniqueValue[2])==(buffer[10]))
                        break; 
           }
           //if not found then check for empty eeprom space for new entry
           if(folderCounter==MAX_FOLDERS)
           {
                for(folderCounter=0;folderCounter<MAX_FOLDERS;folderCounter++)
                {
                    if(((((myimage[folderCounter].FolderUniqueValue[1])&0x0F)==0x0F)&&(myimage[folderCounter].FolderUniqueValue[2]==0xFF))||
                        ((myimage[folderCounter].FolderUniqueValue[0]==0xFF)&&((myimage[folderCounter].FolderUniqueValue[1]&0xF0)==0xF0)))
                        {
                            myimage[folderCounter].FolderUniqueValue[0] = buffer[1];
                            /*
                            myimage[folderCounter].FolderUniqueValue[1] |= buffer[2]&0xf0;
                            myimage[folderCounter].FolderUniqueValue[1] &=~(buffer[2]&0xf0);
                            myimage[folderCounter].FolderUniqueValue[1]|=buffer[9]&0x0F;
                            myimage[folderCounter].FolderUniqueValue[1]&=~(buffer[9]&0x0F);
                            */
                            myimage[folderCounter].FolderUniqueValue[1]=(buffer[2]&0xF0)|(buffer[9]&0x0F);
                            myimage[folderCounter].FolderUniqueValue[2]=buffer[10];
                            if(buffer[3]!=0)
                                myimage[folderCounter].ProgCounterDone=0;
                            break; 
                        }
                }
                if(folderCounter==MAX_FOLDERS){
                #ifdef PRINT_LCD
                    lcd_putsf_row(3,"no eespace ");
                    lcd_puts_hex(buffer[9]&0x0f);
                    lcd_puts_hex(buffer[10]);
                #endif    
                    return FALSE;
                }
           }
           
           if(buffer[0]==1)
           { //force change encryptcounter to next one
                if(tmpimage.ProgCounterTotal!=0 || eeprom_read_byte(4)!=0x44)
                {
                    if(buffer[1] <= myimage[folderCounter].FolderUniqueValue[0])
                    {
                        if((buffer[1] < myimage[folderCounter].FolderUniqueValue[0])
                            ||((buffer[1] == myimage[folderCounter].FolderUniqueValue[0])
                            &&((buffer[2]&0xf0) < (myimage[folderCounter].FolderUniqueValue[1]&0xf0))))
                        {
                            //copy values from SD to eeprom
                            if((buffer[1]) != (myimage[folderCounter].FolderUniqueValue[0]))
                                myimage[folderCounter].FolderUniqueValue[0] = buffer[1];
                            if((buffer[2]&0xf0) != (myimage[folderCounter].FolderUniqueValue[1]&0xf0))
                            {
                                myimage[folderCounter].FolderUniqueValue[1] &= 0x0F;
                                myimage[folderCounter].FolderUniqueValue[1] |= (buffer[2]&0xF0); 
                            }
                            //check if need reset licenses
                            if(buffer[3]!=0)
                            {
                                //myimage[folderCounter].ProgCounterDone=0; 
                            }
                        }
                    }    
                }        
           }
           //check&print if file counter reach 0xFFE, 1 before last 0xFFF.
           if(myimage[folderCounter].FolderUniqueValue[0]==0xFF){
                if((myimage[folderCounter].FolderUniqueValue[1]&0xF0)==0xE0)
                { //ask reset the encrypt counter
                    #ifdef PRINT_LCD
                    lcd_putsf_row(3,"last enc.");
                    #endif               
                    //return FALSE;             
                }
           }
           tmpimage.ProgCounterTotal=(UINT)((buffer[4]<<8)|buffer[5]); //copy total license
           //check file counter first two nibbles 0xFF is not less than in eeprom              
           if((tmpimage.ProgCounterTotal!=0 || eeprom_read_byte(4)!=0x44)&&(buffer[1] < myimage[folderCounter].FolderUniqueValue[0])){  // 0x01,0x23
                #ifdef PRINT_LCD
                lcd_putsf_row(2,"");
                lcd_puts_hex(buffer[1]>>4);
                lcd_puts_hex((buffer[1]<<4)|(buffer[2]>>4)); 
                lcd_putsf("<");
                lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]>>4);
                lcd_puts_hex((myimage[folderCounter].FolderUniqueValue[0]<<4)|
                            (myimage[folderCounter].FolderUniqueValue[1]>>4));
                #endif
                #ifdef PRINT_LCD
                lcd_putsf_row(3,"file is too old");
                #endif
                #ifdef PRINT_LCD_DEBUG
                    lcd_putsf_row(3,"0<");
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]); 
                    //delay_ms(1000);
                    #endif
                return FALSE;
           }
           //check file counter first two nibbles 0xFF is equal to in eeprom
           if((tmpimage.ProgCounterTotal!=0 || eeprom_read_byte(4)!=0x44)&&(buffer[1] == myimage[folderCounter].FolderUniqueValue[0])){
                //check file counter 3rd nibble 0xF0 is not less than in eeprom
                if((buffer[2]&0xf0) < (myimage[folderCounter].FolderUniqueValue[1]&0xf0))
               {
                    #ifdef PRINT_LCD
                    lcd_putsf_row(2,"");
                    lcd_puts_hex(buffer[1]>>4);
                    lcd_puts_hex((buffer[1]<<4)|(buffer[2]>>4)); 
                    lcd_putsf("<");
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]>>4);
                    lcd_puts_hex((myimage[folderCounter].FolderUniqueValue[0]<<4)|
                                (myimage[folderCounter].FolderUniqueValue[1]>>4));
                    #endif
                    #ifdef PRINT_LCD
                    lcd_putsf_row(3,"file is old one");
                    #endif
                    #ifdef PRINT_LCD_DEBUG
                    lcd_putsf_row(3,"=<");
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]); 
                    //delay_ms(1000);
                    #endif
                    return FALSE;
               }
               //check file counter 3rd nibble 0x0F is bigger than in eeprom
               else if((buffer[2]&0xf0) > (myimage[folderCounter].FolderUniqueValue[1]&0xf0))
               {
                  //copy values from SD to eeprom
                  myimage[folderCounter].FolderUniqueValue[1] &= 0x0F;
                  myimage[folderCounter].FolderUniqueValue[1] |= (buffer[2]&0xF0);
                  //check if need reset licenses
                  if(buffer[3]!=0)
                     myimage[folderCounter].ProgCounterDone=0; 
                  #ifdef PRINT_LCD_DEBUG
                    lcd_putsf_row(3,"=>");
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]); 
                    delay_ms(1000);
                  #endif   
               }
           }
           //file counter first two nibbles 0xFF is bigger than in eeprom
           else{//buffer[1] > myimage[folderCounter].FolderUniqueValue[0]
               //copy values from SD to eeprom
               myimage[folderCounter].FolderUniqueValue[0] = buffer[1];
               if((buffer[2]&0xf0) != (myimage[folderCounter].FolderUniqueValue[1]&0xf0))
               {
                  myimage[folderCounter].FolderUniqueValue[1] &= 0x0F;
                  myimage[folderCounter].FolderUniqueValue[1] |= (buffer[2]&0xF0); 
               }
               //check if need reset licenses
               if(buffer[3]!=0){
                    myimage[folderCounter].ProgCounterDone=0; 
               }
               #ifdef PRINT_LCD_DEBUG
                    lcd_putsf_row(3,"0>");
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[0]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[1]);
                    lcd_puts_hex(myimage[folderCounter].FolderUniqueValue[2]); 
                    delay_ms(1000);
               #endif
           }
           if(myimage[folderCounter].ProgCounterDone==0xffff)
                myimage[folderCounter].ProgCounterDone=0;
           tmpimage.ProgCounterDone=myimage[folderCounter].ProgCounterDone;      
           tmpimage.ProgCounterTotal=(UINT)((buffer[4]<<8)|buffer[5]);
       }// end if(tador==0)
       else{
           tmpimage.ProgCounterDone=0;
           tmpimage.ProgCounterTotal=0;
       }    
       if(tmpimage.rollValue==0)  
            tmpimage.ProgCounterTotal=0;
       tmpimage.eepromfile=buffer[6];
       tmpimage.save_serial=buffer[7];
       tmpimage.save_calib=buffer[8];
       /*
       lcd_putsf_row(3,"");
       lcd_puts_hex(dirnum);
       lcd_puts_hex(tmpimage.ProgCounterTotal>>8); 
       lcd_puts_hex(tmpimage.ProgCounterTotal);
       lcd_puts_hex(myimage[dirnum].ProgCounterDone>>8); 
       lcd_puts_hex(myimage[dirnum].ProgCounterDone);
       lcd_puts_hex(myimage[dirnum].EncryptCounter>>8); 
       lcd_puts_hex(myimage[dirnum].EncryptCounter);
       delay_ms(3000);
       */
       /*
       if(buffer[10]!=dirnum){// buffer[9]<<8|buffer[10]=dirnum
            #ifdef PRINT_LCD
            lcd_putsf_row(3,"Wrong file dir.");
            //delay_ms(3000);
            #endif
            return FALSE;
       }
       */
       #ifdef PRINT_DEBUG
       if(debug){
       while(tx_counter);
       while(tx_counter);
       printf("\r\nproginfo=%s",tmpimage.fw_ver);
       printf("\r\nspidiv=%d",tmpimage.spidiv);
       while(tx_counter);}
       #endif 
print_image:
       #ifdef BOOTEEPROM
       CopyEepromToRam();
       #endif
       #ifdef PRINT_DEBUG
       PrintImage();
       #endif
       #ifdef BOOTEEPROM
       myimage.FlashStartAddrByte=FLASH_START_ADDR_WORD<<1;
       AVR910_Standalone("E");//reboot  to bootloader
       #endif
   }
   return TRUE;
}

void ResetGlobals(void)
{
   CanContinueRead=0;
   EndOfFile=0;
   g_Operation = OPERATION_READ;
   g_BlockMode = FALSE;
   OffsetX16=0;
   blankpage=FALSE;
   pageaddr=0;//reset address byte counter
}
UCHAR Call_ReadSignature(void)
{
   //delay_ms(200);
   AVR910_Standalone("s");//send read signature -> Figure out what kind of CPU
   #ifdef PRINT_DEBUG
   if(debug){
   while(tx_counter);
   printf("\r\nTarget signature is: %02x %02x %02x",g_txBuf[0],g_txBuf[1],g_txBuf[2]);
   while(tx_counter);}
   #endif
   //AVR910 ReadSignature order is 02(3) 97(2) 1E(1) so need to reverse it
   if((tmpimage.signature[1]!=g_txBuf[1])||(tmpimage.signature[2]!=g_txBuf[2])){
       #ifdef PRINT_DEBUG
       printFlashString(" Wrong target\r\n");
       #endif
       delay_ms(1000);
       #ifdef PRINT_LCD
       lcd_putsf_row(3,"Wrong target");
       #endif
       return FALSE;
   }
   #ifdef PRINT_DEBUG
   printFlashString("\r\nReading signature: OK.");
   #endif
   #ifdef PRINT_LCD
   lcd_putsf_row(3,"Signature OK.");
   #endif
   AVR910_SendRxToUart();// reset get spi  buffer
   return TRUE;
}
UCHAR Call_ReadCalibrationByte(void)
{
   delay_ms(200);
   AVR910_Standalone("W");//send read calibration bytes
   g_CalibBytes[0]=g_txBuf[0];
   g_CalibBytes[1]=g_txBuf[1];
   g_CalibBytes[2]=g_txBuf[2];
   g_CalibBytes[3]=g_txBuf[3];
   #ifdef PRINT_DEBUG
   if(debug){
   while(tx_counter);
   printf("\r\nTarget signature is: %02x %02x %02x %02x",g_txBuf[0],g_txBuf[1],g_txBuf[2],g_txBuf[3]);
   while(tx_counter);}
   #endif
   //AVR910 ReadSignature order is 02(3) 97(2) 1E(1) so need to reverse it
   #ifdef PRINT_DEBUG
   printFlashString("\r\nReading calibration bytes: OK.");
   #endif
   #ifdef PRINT_LCD
   if(CanShow){
        lcd_putsf_row(3,"CLB:"); 
        lcd_puts_hex(g_CalibBytes[0]);
        lcd_puts_hex(g_CalibBytes[1]);
        lcd_puts_hex(g_CalibBytes[2]);
        lcd_puts_hex(g_CalibBytes[3]);
        delay_ms(LCD_MSG_LONG_DELAY);
   }
   #endif
   AVR910_SendRxToUart();
   return TRUE;
}
void Call_Erase(void)
{
   //delay_ms(200);
   #ifdef PRINT_DEBUG
   printFlashString("\r\nErasing please wait... ");
   #endif
   #ifdef PRINT_LCD
   lcd_putsf_row(3,"Erasing... ");
   #endif
   AVR910_Standalone("e");//chip erase
   #ifdef PRINT_DEBUG
   if(debug){
   printf("Done.");
   while(tx_counter);}
   #endif
   #ifdef PRINT_LCD
   lcd_putsf("Done");
   #endif
   AVR910_ResetTXData();
}
UCHAR Call_SetDevType(void)
{
   g_DevicePollCode=tmpimage.pagesizebytes/2;
   g_DeviceType=MEGA;
   return TRUE;
}
UCHAR Call_WriteProgFuses(void)
{
   SetLowSpeed();
   //delay_ms(100);
   #ifdef PRINT_LCD
   if(CanShow){
     lcd_putsf_row(3,"Prog Fuses ");
   }
   #endif   
   if (! programFuses(tmpimage.progfuses, tmpimage.maskfuses)){        // get fuses ready to program
     error("\r\nProgram Fuses fail");
     #ifdef PRINT_LCD
     lcd_putsf_row3(3,"Prog Fuses fail");
     #endif
     return FALSE;
   }
   if (! verifyFuses(tmpimage.progfuses, tmpimage.maskfuses) ) {
     error("\r\nFailed to verify fuses");
     #ifdef PRINT_LCD
     lcd_putsf_row3(3,"Chk Fuses fail");
     #endif
     return FALSE;
   }
   #ifdef PRINT_DEBUG
   printFlashString("\r\nProgram Fuses Verified OK.");
   #endif
   if(CanShow){
   #ifdef PRINT_LCD
     lcd_putsf("done"); 
   #endif  
   }
   return TRUE;
}
UCHAR Call_WriteFlash(void)
{
//here g_DeviceCode=chipcode, g_DevicePollCode=PageSize in words, g_DeviceType = MEGA
//['A',0x10,0x00]=SetAddr(); g_Addr=0x1000;
    /*Set g_Addr=0 start address for BlockMode*/
    SetHiSpeed();
    delay_ms(100);
    AVR910_SendRxToUart();FIFO_Write('A');FIFO_Write(0);FIFO_Write(0);AVR910_Command();
    if(g_txBuf[0]!=ACK){
        #ifdef PRINT_DEBUG
        printFlashString("\r\nSetAddr FAIL");
        #endif
        return FALSE;
    }
   //here g_Addr=0;
    //['B',0x00,0x80,'F'] WriteBlockCommand();g_BlockMode=TRUE;g_Operation=OPERATION_WRITE;g_BlockSize=0x0080;g_WordCount=0;g_MemType='Flash'(/2) -> BlockSize=0x0040
    AVR910_SendRxToUart();//clear uart
  /*Set BlockMode chip size in bytes and type of memory Flash F or Eeprom E*/
    FIFO_Write('B');FIFO_Write(tmpimage.chipsize>>16);FIFO_Write(tmpimage.chipsize>>8);FIFO_Write(tmpimage.chipsize&0xFF);FIFO_Write('F');AVR910_Command();
    //AVR910_Command();
  //here should be g_BlockSize(words)=chipsize/2, g_MemType='F'=flash, g_BlockMode=TRUE, g_Operation=Write, g_WordCount=0
  //do while pageaddr in bytes less than chipsize in bytes
    printFlashString("\r\nWriting flash...");
    #ifdef PRINT_LCD    
    if(CanShow){
        itoa((int)((int)(g_CpuFreq/2)/((int)(UBRR0L)+(int)1)),tmpbytes);   // 1=0,1 2=0,1,2
        strcatf(tmpbytes,"Khz  ");
        lcd_puts_row(3,tmpbytes);
        lcd_putchar((spidiv)+'0');
        delay_ms(LCD_MSG_LONG_DELAY);
    }
    lcd_putsf_row(3,"Flashing");
    #endif
    if(fileOpen()==0)
        return FALSE;
    #ifdef PRINT_DEBUG
    printFlashString("\r\nAfter fileOpen\r\n");
    #endif
    if(SetFlashArea()==0)
        return FALSE;
    #ifdef PRINT_DEBUG
    printFlashString("\r\nAfter set adr 1000\r\n");
    #endif
    if(fileRead()==0)
        return FALSE;
    while (pageaddr < tmpimage.chipsize) {
     //puts("\r\nBuffering page "); puthexchars(pageaddr);
 //hextextpos = readImagePage (hextext, pageaddr, pagesize, pageBuffer);

     if(readImagePageSD (pageaddr, tmpimage.pagesizebytes, pageBuffer)==0){
       #ifdef PRINT_DEBUG
       printFlashString("\r\nFlash programming failed");
       #endif
       #ifndef BOOTEEPROM
       fileClose();
       #endif
       return FALSE;
     }
     #ifdef DEBUG_PROTEUS
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nPage addr=%lu",pageaddr);
        while(tx_counter);}
        #endif
     #endif
     if (FALSE == blankpage) {//if there data to write the all page buffer to flash
            if (! flashPage(pageBuffer, tmpimage.pagesizebytes)){//
                #ifdef PRINT_DEBUG
                printFlashString("\r\nFlash programming failed");
                #endif
                #ifndef BOOTEEPROM
                fileClose();
                #endif
                return FALSE;
       }
     }
     else{//if no data to write, just increment the address in words by pagesize in words, and decrement remain g_BlockSize
        g_Addr = (pageaddr+tmpimage.pagesizebytes)/2;
        g_BlockSize-=(tmpimage.pagesizebytes/2);
     }
     if(EndOfFile)
        break;
     #ifdef PRINT_LCD
     if(CanShow){
       lcd_gotoxy(9,3);
       itoa((int)((float)pageaddr/(float)tmpimage.chipsize*(float)100),&temptext[0]);
       if((strlen(temptext))<3){
          lcd_puts(temptext);
          lcd_putsf("%");
       }
     }
     #endif
     pageaddr += tmpimage.pagesizebytes;//increment the address in bytes by pagesize in bytes

        
    }
    #ifdef PRINT_LCD
    lcd_putsf_row(3,"");
    #endif
    #ifdef PRINT_DEBUG
    printFlashString(" Done.");
    #endif
    #ifndef BOOTEEPROM
    fileClose();
    #endif
    return TRUE;
}
UCHAR Call_WriteFlashBin(void)
{
    SetHiSpeed();
    //delay_ms(100);    
    #ifdef PRINT_LCD    
    if(CanShow){
        itoa((int)((int)(g_CpuFreq/2)/((int)(UBRR0L)+(int)1)),tmpbytes);   // 1=0,1 2=0,1,2
        strcatf(tmpbytes,"Khz  ");
        lcd_puts_row(3,tmpbytes);
        lcd_putchar((spidiv)+'0');
        delay_ms(LCD_MSG_LONG_DELAY);
    }
    lcd_putsf_row(3,"Writing FLASH..");
    #endif
    if(WriteBlockBin()==FALSE){
    #ifdef PRINT_LCD
        lcd_putsf_row(3,"FLASH Failed!!!");
    #endif    
        return FALSE;
    }
    #ifdef PRINT_LCD
    lcd_putsf_row(3,"");
    #endif
    #ifdef PRINT_DEBUG
    printFlashString(" Done.");
    #endif
    #ifndef BOOTEEPROM
    fileClose();
    #endif
    return TRUE;
}

UCHAR Call_WriteEeprom(void)
{
    ResetGlobals();
    SetHiSpeed();
    /* Switch the memory type to EEPROM */
    g_MemType='E';
    //delay_ms(100);
    /*
    strcpyf(path,"/");
    itoa(dirnum,tmpbytes);
    strcat(path,tmpbytes);// "/1"
    strcatf(path,"/");// "/1/"
    strcatf(path,"eeprom.hex");// "/1/config.ini"
    */
    /*Try Read the eeprom.hex file first time*/
    //if(ReadFileToBuffer()==FALSE)
    //    return TRUE;    
    /* Set g_Addr=0 */
    AVR910_SendRxToUart();FIFO_Write('A');FIFO_Write(0);FIFO_Write(0);AVR910_Command();
    if(g_txBuf[0]!=ACK){
        printFlashString("\r\nSetAddr FAIL");
        return FALSE;
    }
    /*Write Eeprom memory byte by byte*/
    #ifdef PRINT_DEBUG
    printFlashString("\r\nWriting eeprom...");
    #endif
    #ifdef PRINT_LCD
    lcd_putsf_row(3,"Writing EEPROM.");
    #endif
    pageaddr = 0;
    blankpage = FALSE;
    #ifdef PRINT_DEBUG
     if(debug){
     while(tx_counter);
     printf("\r\nPage addr=%lu , eeprom size=%u\r\n",pageaddr,tmpimage.eepromsizebytes);
     while(tx_counter);}
     #endif
    LastOffset=0x40800L; 
    if(SetBinFileOffset(LastOffset)==FALSE)
        return FALSE;
    if(fileRead()==FALSE) 
        return FALSE;
    /*
    lcd_putsf_row(1,"");
    lcd_puts_hex(buffer[0]); 
    lcd_puts_hex(buffer[1]);
    lcd_puts_hex(buffer[2]);
    lcd_puts_hex(buffer[3]);
    lcd_putsf_row(2,"");
    lcd_puts_hex(LastOffset>>16); 
    lcd_puts_hex(LastOffset>>8);
    lcd_puts_hex(LastOffset);
    lcd_puts_hex(nbytes>>8); 
    lcd_puts_hex(nbytes);
    */  
    while (pageaddr < tmpimage.eepromsizebytes) {
         AVR910_ResetTXData();
         if(readImagePageSD (pageaddr, PAGE_SIZE_BYTES, pageBuffer)==0){
            #ifdef PRINT_DEBUG
            printFlashString("\r\nEEPROM programming failed");
            #endif
            #ifndef BOOTEEPROM
            fileClose();
            #endif
            return FALSE;
         }         
         #ifdef PRINT_DEBUG
         if(debug){
         while(tx_counter);
         printf("\r\nPage addr=%lu , eeprom size=%u",pageaddr,tmpimage.eepromsizebytes);
         while(tx_counter);}
         #endif
         if (FALSE == blankpage) {//if there data to write the all page buffer to flash
                if(WriteChipEeprom(pageBuffer,PAGE_SIZE_BYTES,pageaddr)==FALSE){
                    #ifdef PRINT_DEBUG
                    printFlashString("\r\nEEPROM programming failed");
                    #endif
                    #ifndef BOOTEEPROM
                    fileClose();
                    #endif
                    return FALSE;
                }
         }
         else{
            #ifdef PRINT_DEBUG
            printFlashString("\r\nIs blank.\r\n");
            #endif
         }
         if(EndOfFile){
            #ifdef PRINT_DEBUG
            printFlashString("\r\nEnd of file.\r\n");
            break;
            #endif     
         }
         pageaddr += PAGE_SIZE_BYTES;//increment the address in bytes by pagesize in bytes
    }
    #ifdef PRINT_DEBUG
    printFlashString(" Done.");
    #endif
    #ifndef BOOTEEPROM
    fileClose();
    #endif
    return TRUE;
}

UCHAR Call_VerifyEeprom(void)
{
    ResetGlobals();
    SetHiSpeed();
    g_MemType='E';
    //delay_ms(100);
    /*
    strcpyf(path,"/");
    itoa(dirnum,tmpbytes);
    strcat(path,tmpbytes);// "/1"
    strcatf(path,"/");// "/1/"
    strcatf(path,"eeprom.hex");// "/1/config.ini"
    */
    /*Try Read the eeprom.hex file first time*/
    //if(ReadFileToBuffer()==FALSE)
    //    return TRUE;
    /* Switch the memory type to EEPROM */    
    AVR910_SendRxToUart();FIFO_Write('A');FIFO_Write(0);FIFO_Write(0);AVR910_Command();
    if(g_txBuf[0]!=ACK){
        #ifdef PRINT_DEBUG
        printFlashString("\r\nSetAddr FAIL");
        #endif
        return FALSE;
    }
   //here g_Addr=0;
    AVR910_SendRxToUart();//clear uart
    /*Write Eeprom memory byte by byte*/
    #ifdef PRINT_DEBUG
    printFlashString("\r\nVerify eeprom...");
    #endif
    #ifdef PRINT_LCD
    lcd_putsf_row(3,"Verify EEPROM");
    #endif
    pageaddr = 0;
    blankpage = FALSE;
    LastOffset=0x40800L; 
    if(SetBinFileOffset(LastOffset)==FALSE)
        return FALSE;
    if(fileRead()==FALSE) 
        return FALSE;
    while (pageaddr < tmpimage.eepromsizebytes) {
         if(readImagePageSD (pageaddr, PAGE_SIZE_BYTES, pageBuffer)==0){
           #ifdef PRINT_DEBUG
           printFlashString("\r\nEEPROM verifying failed");
           #endif
           #ifndef BOOTEEPROM
           fileClose();
           #endif
           return FALSE;
         }
         #ifdef DEBUG_PROTEUS
            #ifdef PRINT_DEBUG
            if(debug){
            while(tx_counter);
            printf("\r\nPage addr=%lu",pageaddr);
            while(tx_counter);}
            #endif
         #endif
         if (FALSE == blankpage) {//if there data to write the all page buffer to flash
                if(VerifyChipEeprom(pageBuffer,PAGE_SIZE_BYTES,pageaddr)==FALSE){
                    #ifdef PRINT_DEBUG
                    printFlashString("\r\nEeprom verifying failed");
                    #endif
                    #ifndef BOOTEEPROM
                    fileClose();
                    #endif
                    #ifdef PRINT_LCD
                    lcd_putsf_row3(3,"Verify ee failed");
                    #endif
                    return FALSE;
                }
         }
         if(EndOfFile){
            #ifdef PRINT_DEBUG
            printFlashString("\r\nEnd of file.\r\n");
            #endif
            break;     
         }
         pageaddr += PAGE_SIZE_BYTES;//increment the address in bytes by pagesize in bytes
    }
    #ifdef PRINT_DEBUG
    printFlashString(" Done.");
    #endif
    #ifndef BOOTEEPROM
    fileClose();
    #endif
    return TRUE;
}

UCHAR Call_ResetChip(UCHAR IsLowSpeed)
{      
   #ifdef PRINT_DEBUG
   printFlashString("\r\nReseting chip...\r\n");
   #endif
   AVR910_ResetTXData();
   AVR910_Standalone("L");//end_pmode();
   return Call_EnterProgMode(IsLowSpeed);
}
void printFlashString(flash char *str)
{
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    putsf(str);
    while(tx_counter);}
    #endif
}
UCHAR SaveSerialToEepromAndInc()
{
    UINT    EepromStartAddress;
    ULONG   SerialToWriteToEeprom;
    ResetGlobals();
    g_MemType='E';
    strcpyf(path,"/");
    itoa(dirnum,tmpbytes);
    strcat(path,tmpbytes);// "/1"
    strcatf(path,"/");// "/1/"
    //strcat(path,tmpbytes);// "/1/1"
    strcatf(path,"serial.txt");// "/1/config.ini"
    /*Read last saved serial from serial.txt file*/
    if(ReadFileToBuffer()==FALSE)
        return FALSE; // if file not exists then no need to write serial to eeprom
    /* Read start address where to put the serial in eeprom */
    if((value=ReadValue("startaddress"))==NULL)
          return FALSE;
    EepromStartAddress=hextouint(value);
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\startaddress = %lu",EepromStartAddress);
    while(tx_counter);}
    #endif 
    /* Read the serial that need to write in eeprom */
    if((value=ReadValue("nextserial"))==NULL)
        return FALSE;    
    SerialToWriteToEeprom = hextoulong(value);
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nread 'nextserial' that will put to eeprom = %lu",SerialToWriteToEeprom);
    while(tx_counter);}
    #endif
    if(SerialToWriteToEeprom==0)
        return TRUE;
    /*Convert ascii ulong value to ulong(4 bytes) byte by byte*/
    tmpbytes[0]=(hexton(value[0])<<4)|(hexton(value[1])&0x0f);
    tmpbytes[1]=(hexton(value[2])<<4)|(hexton(value[3])&0x0f);
    tmpbytes[2]=(hexton(value[4])<<4)|(hexton(value[5])&0x0f);
    tmpbytes[3]=(hexton(value[6])<<4)|(hexton(value[7])&0x0f);
    /*Write serial byte by byte to eeprom*/
    if(WriteChipEeprom(tmpbytes,4,EepromStartAddress)==FALSE){
        #ifdef PRINT_DEBUG
        printFlashString("\r\nWrite serial to eeprom failed");
        #endif
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"EE write fail");
        #endif
        return FALSE;
    }    
    if(VerifyChipEeprom(tmpbytes,4,EepromStartAddress)==FALSE){
        printFlashString("\r\nVrify serial vs eeprom failed");
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"EE verify fail");
        #endif
        return FALSE;
    }
    
    /*prepare the tmpbytes with next serial in ascii 8 chars*/
    sprintf(tmpbytes,"%08lX",SerialToWriteToEeprom+1);
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nwill write to serial.txt at 'nextserial' field 'tmpbytes'=%s",tmpbytes);
    while(tx_counter);}
    #endif    
    if(WriteConfigValue("nextserial",tmpbytes,8)==FALSE)
        return FALSE;
    if(ReadFileToBuffer()==FALSE)
          return FALSE;
    if((value=ReadValue("nextserial"))==NULL)
          return FALSE;
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nread saved in serial.txt 'nextserial'=%lu",hextoulong(value));
    while(tx_counter);}
    #endif 
    #ifdef PRINT_LCD
    lcd_putsf_row3(3,"SN: ");
    ltoa(SerialToWriteToEeprom,tmpbytes);
    lcd_puts(tmpbytes);
    if(CanShow)
        delay_ms(LCD_MSG_LONG_DELAY);
    #endif
    return TRUE;
}
UCHAR SaveCalibBytesToEeprom()
{
    UINT    EepromStartAddress;
    if(Call_ReadCalibrationByte()==FALSE)
        return FALSE;
    ResetGlobals();
    g_MemType='E';
    strcpyf(path,"/");
    itoa(dirnum,tmpbytes);
    strcat(path,tmpbytes);// "/1"
    strcatf(path,"/");// "/1/"
    //strcat(path,tmpbytes);// "/1/1"
    strcatf(path,"calbytes.txt");// "/1/config.ini"
    /*Read last saved serial from serial.txt file*/
    if(ReadFileToBuffer()==FALSE)
        return FALSE; 
    /* Read start address where to put the serial in eeprom */
    if((value=ReadValue("startaddress"))==NULL)
          return TRUE;
    EepromStartAddress=hextouint(value);
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\startaddress = %lu",EepromStartAddress);
    while(tx_counter);}
    #endif   
    /*Write calib byte by byte to eeprom*/
    if(WriteChipEeprom(g_CalibBytes,4,EepromStartAddress)==FALSE){
        #ifdef PRINT_DEBUG
        printFlashString("\r\nWrite calib to eeprom failed");
        #endif
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"clb wr ee fail");
        delay_ms(LCD_MSG_LONG_DELAY);
        #endif
        return FALSE;
    }    
    if(VerifyChipEeprom(g_CalibBytes,4,EepromStartAddress)==FALSE){
        printFlashString("\r\nVrify serial vs eeprom failed");
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"calib vrf fail");
        delay_ms(1000);
        #endif
        return FALSE;
    }
    //sprintf(tmpbytes,"%08lX",(ULONG)g_CalibBytes[0]|(ULONG)g_CalibBytes[1]<<8|(ULONG)g_CalibBytes[2]<<16|(ULONG)g_CalibBytes[2]<<24); 
    //sprintf(tmpbytes,"%08lX",g_CalibBytes;);
    tmpbytes[0]= ntohexchar(g_CalibBytes[0]>>4);
    tmpbytes[1]= ntohexchar(g_CalibBytes[0]);
    tmpbytes[2]= ntohexchar(g_CalibBytes[1]>>4);
    tmpbytes[3]= ntohexchar(g_CalibBytes[1]);
    tmpbytes[4]= ntohexchar(g_CalibBytes[2]>>4);
    tmpbytes[5]= ntohexchar(g_CalibBytes[2]);
    tmpbytes[6]= ntohexchar(g_CalibBytes[3]>>4);
    tmpbytes[7]= ntohexchar(g_CalibBytes[3]); 
    if(WriteConfigValue("calibbytes",tmpbytes,8)==FALSE){
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"cal wr conf");
        #endif
        return FALSE;
    }
    if(ReadFileToBuffer()==FALSE){
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"cal rd conf");
        #endif
        return FALSE;
    }
    if((value=ReadValue("calibbytes"))==NULL){
        #ifdef PRINT_LCD
        lcd_putsf_row3(3,"cal rd val");
        #endif
        return FALSE;
    }
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\ncalib saved in calbytes.txt");
    while(tx_counter);}
    #endif 
cal_print:    
    #ifdef PRINT_LCD
    if(CanShow){
        lcd_putsf_row3(3,"CLB:");
        lcd_puts_hex(g_CalibBytes[0]);
        lcd_puts_hex(g_CalibBytes[1]);
        lcd_puts_hex(g_CalibBytes[2]);
        lcd_puts_hex(g_CalibBytes[3]);
        delay_ms(LCD_MSG_LONG_DELAY);
    }
    #endif
    return TRUE;
}
/*Main function for standalone AVR ISP proccess*/
void StandalonProg(void)
{
    //char tmpstr[3];
    //UCHAR retryVerify;
    StandaloneMode=RUNNING;
    /* re-initialize Timer1 */
    //TCNT1H=TCNT1L=0;// 1count=1/2000=
    /**/
    spidiv=tmpimage.spidiv;    
    printFlashString("\r\nStart flashing from SDcard...\r\n");
    LED_PASS_OFF(); 
    if(tador==0){       
        if((tmpimage.ProgCounterTotal <= myimage[folderCounter].ProgCounterDone)||(myimage[folderCounter].ProgCounterDone==0xFFFE)){    // 0 < 0 false
            if(tmpimage.ProgCounterTotal){// >0 case
                #ifdef PRINT_DEBUG
                if(debug){
                while(tx_counter);
                printf("\r\nLicenses Total = %lu - Done = %lu",tmpimage.ProgCounterTotal,myimage[folderCounter].ProgCounterDone);
                while(tx_counter);}
                #endif
                #ifdef PRINT_LCD
                lcd_putsf_row3(3,"No licenses");
                #endif
                goto end_prog;
            }
            if(eeprom_read_byte(4)!=0x44 && myimage[folderCounter].ProgCounterDone>=10){
                #ifdef PRINT_LCD
                lcd_putsf_row3(3,"Demo limit 10");
                #else
                #ifdef SEG7 
                show7seg('d');
                #endif
                #endif
                
                goto end_prog;
            }
        }
    }// end if(tador==0)
    /*Reset flash programminig global variables*/
    ResetGlobals();
    #ifndef DEBUG_PROTEUS
    if(Call_EnterProgMode(TRUE)==FALSE)  //low speed
       goto end_prog;
    Call_Erase();    
    if(Call_WriteProgFuses()==FALSE)
       goto end_prog;
    SetHiSpeed();
verify_flash:
    if(Call_ResetChip(FALSE)==FALSE) //reset in high speed
       goto end_prog;
    #endif
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nprog spidiv=%d\r\n",spidiv);
    while(tx_counter);}
    #endif
    if(Call_SetDevType()==FALSE)
       goto end_prog;
    Call_Erase();
    ResetGlobals();
    g_MemType='F';
    if(Call_WriteFlashBin()==FALSE){
       if(TryDecSpeed()){
            goto verify_flash;
       }
       //lcd_putsf(" Fail!");
       goto end_prog;
    }
    #ifdef PRINT_LCD    
    if(CanShow){
        itoa((int)((int)(g_CpuFreq/2)/((int)(UBRR0L)+(int)1)),tmpbytes);   // 1=0,1 2=0,1,2
        strcatf(tmpbytes,"Khz  ");
        lcd_puts_row(3,tmpbytes);
        lcd_putchar((spidiv)+'0');
        delay_ms(LCD_MSG_LONG_DELAY);
        delay_ms(LCD_MSG_LONG_DELAY);
    }
    lcd_putsf_row(3,"");
    #endif
    ResetGlobals();
    if(tmpimage.eepromfile!=0){
        if(Call_WriteEeprom()==FALSE)
            goto end_prog;
        if(Call_VerifyEeprom()==FALSE)
            goto end_prog; 
    }
    if(tmpimage.save_serial!=0){
        if(SaveSerialToEepromAndInc()==FALSE){
           #ifdef PRINT_LCD
           lcd_putsf_row3(3,"SN EE Failed.");       
           #endif
           goto end_prog;                     
        }
    }
    if(tmpimage.save_calib!=0){
        if(SaveCalibBytesToEeprom()==FALSE){
           #ifdef PRINT_LCD
           lcd_putsf_row3(3,"Calib Failed.");       
           #endif
           goto end_prog;                     
        }
    }
    // Set fuses to 'final' state
    if (! programFuses(tmpimage.normfuses, tmpimage.maskfuses)){
        error("\r\nNormal Fuses fail");
        goto end_prog;
    }
    SetHiSpeed();
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nNormal fuses writing done.");
    while(tx_counter);}
    #endif
    if(Call_ResetChip(TRUE)==FALSE) //low speed
       goto end_prog;
    if (! verifyFuses(tmpimage.normfuses, tmpimage.maskfuses) ) {
        error("\r\nFailed to verify fuses");
        goto end_prog;
    }
    SetHiSpeed();
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nNormal fuses verified correctly !!!");
    while(tx_counter);}
    #endif
    tmpimage.spidiv=spidiv;    
    //delay_ms(1000);
    StandaloneMode=PASS;
    LED_PASS_ON();
end_prog:
    AVR910_ResetTXData();
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf("\r\nLeaving programming mode... ");
    while(tx_counter);}
    #endif
    AVR910_Standalone("L");
    #ifdef PRINT_DEBUG
    if(debug){
    while(tx_counter);
    printf(" OK.\n");
    while(tx_counter);}
    #endif
    tmpimage.ProgCounterDone = myimage[folderCounter].ProgCounterDone;
    if(StandaloneMode!=PASS){
        CanShow=1;
        if(StandaloneMode!=SDFAIL){
            StandaloneMode=FAIL;
        }
        #ifdef PRINT_DEBUG
        if(debug){
        if(StandaloneMode!=SDFAIL)
            printf("\r\nFlashing from sdcard Failed .\r\n",tmpimage.ProgCounterDone);
        else{
            printf("\r\nSDCARD Failed .\r\n",);
            if(value==NULL)
                printf("project.txt file missing or corrupted.\r\n");
        }
        while(tx_counter);}
        #endif
        
    }
    else{
        CanShow=0;
        if(tmpimage.ProgCounterTotal!=0 || eeprom_read_byte(4)!=0x44)
            tmpimage.ProgCounterDone=++myimage[folderCounter].ProgCounterDone;
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nFlashing from sdcard Passed %u.\r\n",tmpimage.ProgCounterDone);
        while(tx_counter);}
        #endif
        #ifdef PRINT_LCD
        MsgBufClr();
        itoa((Counter10ms/100),temptext);
        lcd_putsf_row(3,"Time ");
        lcd_puts(temptext);
        lcd_putsf("sec");
        delay_ms(500);
        lcd_putsf_row(3,"Success!  ");
        if(tmpimage.ProgCounterTotal!=0 || eeprom_read_byte(4)!=0x44){
            itoa(tmpimage.ProgCounterDone,temptext);
            lcd_puts(temptext);
        }
        
        #endif
        Beeps(1,1,500);
    }
}

/*
 * readImagePage SDcard
 *
 * Read a page of intel hex image from a string in pgm memory.
*/

// Returns number of bytes decoded
UCHAR readImagePageSD (ULONG pageaddr, UINT pagesize, UCHAR *page)
{
  UINT i;
  //BOOL firstline = TRUE;
  UINT page_idx = 0;
  //byte *beginning = hextext;
  ULONG lineaddr;
  UCHAR b, cksum = 0;
  UCHAR len;
  
  //Serial.print("page size = "); Serial.println(pagesize, DEC);
  if(blankpage==FALSE){
      // 'empty' the page by filling it with 0xFF's
      for (i=0; i<pagesize; i++)
        page[i] = 0xFF;
      blankpage=TRUE;
  }
  while (1) {
    if(EndOfFile)
        return 1;
    if(CanContinueRead==0){
              // read one line!
        if (hextext_read_byte() != ':') {
           error("No colon?");
           return 0;//break;

        }
        // Read the byte count into 'len'
        len = hexton(hextext_read_byte());
        len = (len<<4) + hexton(hextext_read_byte());
        cksum = len;

        // read high address byte
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        lineaddr = b;

        // read low address byte
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        lineaddr = (lineaddr << 8) + b;
        lineaddr+=OffsetX16;//ADD OFFSET *16
        // example lineaddr=0xFC00 pageaddr=0 pagesize=128 -> (blankpage=true) next time pageaddr=pageaddr+pagesize
        if (lineaddr >= (pageaddr + pagesize)) {
          return 1;//break;
        }

        b = hexton(hextext_read_byte()); // record type
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        //Serial.print("Record type "); Serial.println(b, HEX);
        if (b == 0x1) {
             EndOfFile=1;
             // end record!
             return 1;//break;
        }
        // offset *16 record!
        else if (b == 0x2) {
             b = hexton(hextext_read_byte());
             b = (b<<4) + hexton(hextext_read_byte());
             cksum += b;
             OffsetX16=((ULONG)b<<12);
             b = hexton(hextext_read_byte());
             b = (b<<4) + hexton(hextext_read_byte());
             cksum += b;
             OffsetX16|=((ULONG)b<<4);
            b = hexton(hextext_read_byte());  // chxsum
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            if (cksum != 0) {
              error("Bad checksum: ");
              return 0;
              //Serial.print(cksum, HEX);
            }
            if (hextext_read_byte() != '\r') {
              error("No end of line");
              return 0;//break;
            }
            if (hextext_read_byte() != '\n') {
              error("No end of line");
              return 0;//break;
            }
            continue;
        } //no need to stuff
        else if (b == 0x3) {
            b = hexton(hextext_read_byte());
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            b = hexton(hextext_read_byte());
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            b = hexton(hextext_read_byte());
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            b = hexton(hextext_read_byte());
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            b = hexton(hextext_read_byte());  // chxsum
            b = (b<<4) + hexton(hextext_read_byte());
            cksum += b;
            if (cksum != 0) {
                error("Bad checksum: ");
                return 0;//break;
            //Serial.print(cksum, HEX);
            }
            if (hextext_read_byte() != '\r') {
              error("No end of line");
              return 0;//break;
            }
            if (hextext_read_byte() != '\n') {
              error("No end of line");
              return 0;//break;
            }
            continue;
        }
        i=0;
    }
    else{
        i=Save_i;
        cksum=Save_cksum;
        len=Save_len;
        CanContinueRead=0;
    }
    //blankpage=FALSE;
    for ( ; i < len; i++) {
      // read 'n' bytes
      b = hexton(hextext_read_byte());
      b = (b<<4) + hexton(hextext_read_byte());

      cksum += b;
      if(blankpage!=FALSE)
        if(b!=0xff)
            blankpage=FALSE;
      page[page_idx] = b;
      page_idx++;

      if (page_idx > (pagesize-1)) {
          CanContinueRead=1;
          //Save_lineaddr=lineaddr;
          Save_i=i+1;
          Save_cksum=cksum;
          Save_len=len;
          //error("Too much code");
          return 1;//break;
      }
    }
    if(CanContinueRead)
        return 1;//break;
    b = hexton(hextext_read_byte());  // chxsum
    b = (b<<4) + hexton(hextext_read_byte());
    cksum += b;
    if (cksum != 0) {
      error("Bad checksum: ");
      return 0;//break;
      //Serial.print(cksum, HEX);
    }
    if (hextext_read_byte() != '\r') {
              error("No end of line");
              return 0;//break;
    }
    if (hextext_read_byte() != '\n') {
      error("No end of line");
      return 0;//break;
    }
    if (page_idx == pagesize)
      return 0;//break;
    }
    return 0;//return hextext;
}


// Basically, write the pagebuff (with pagesize bytes in it) into page $pageaddr
UCHAR flashPage (UCHAR *pagebuff, UINT pagesize) {
  //SPI.setClockDivider(CLOCKSPEED_FLASH);
  UINT i;
  for (i=0; i < (pagesize/2); i++) {
    AVR910_ResetTXData();//clear send to uart tx buffer
    FIFO_Write(pagebuff[2*i]);
    FIFO_Write(pagebuff[2*i+1]);
    AVR910_Command();
  }
  return TRUE;
}

//Write data to eeprom. return bool
UCHAR WriteChipEeprom(UCHAR *data, UINT len, UINT startaddr)
{
   UINT i;
   AVR910_ResetTXData();//clear send to uart tx buffer
   // 1. 'A,0x00(high),0x01(low)' set address to write to eeprom. ACK response
   FIFO_Write('A');FIFO_Write(startaddr>>8);FIFO_Write(startaddr&0xff);AVR910_Command();
   for(i=0;(i<len)&&(g_Addr<tmpimage.eepromsizebytes);i++){            
        // 2   'D' - write byte to eeprom  and inc g_Addr          
        FIFO_Write('D');FIFO_Write(*(data+i));AVR910_Command();
        if(g_txBuf[0]!=ACK){
            return FALSE;
        }
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nee adr=%u data=%02X .",g_Addr-1,*(data+i));
        while(tx_counter);}
        #endif
   }
   return TRUE;
}

//verify data from eeprom. return bool
UCHAR VerifyChipEeprom(UCHAR *data, UINT len, UINT startaddr)
{
   UINT i;
   AVR910_ResetTXData();//clear send to uart tx buffer
   // 1. 'A,0x00(high),0x01(low)' set address to write to eeprom. ACK response
   FIFO_Write('A');FIFO_Write(startaddr>>8);FIFO_Write(startaddr&0xff);AVR910_Command();
   for(i=0;(i<len)&&(g_Addr<tmpimage.eepromsizebytes);i++){            
        // 2   'd' - read byte from eeprom  and inc g_Addr  
        FIFO_Write('d');AVR910_Command();
        if(g_txBuf[0]!=*(data+i)){
            return FALSE;
        }
        #ifdef PRINT_DEBUG
        if(debug){
        while(tx_counter);
        printf("\r\nbuf adr=%u data=%02X : ee adr=%u data=%02X .",i,*(data+i),g_Addr-1,g_txBuf[0]);
        while(tx_counter);}
        #endif
   }
   return TRUE;
}

//read data from eeprom. return bool
UCHAR ReadChipEeprom(void)
{
   UINT i;
   AVR910_ResetTXData();//clear send to uart tx buffer
   // 1. 'A,0x00(high),0x01(low)' set address to write to eeprom. ACK response
   FIFO_Write('A');FIFO_Write(0);FIFO_Write(0);AVR910_Command();
   for(i=0;i<(tmpimage.eepromsizebytes);i++){    
        // 2   'd' - read byte from eeprom  and inc g_Addr  
        FIFO_Write('d');AVR910_Command();
        //g_txBuf[0];        
   }
   return TRUE;
}

// verifyImage SDcard does a byte-by-byte verify of the flash hex against the chip
// Thankfully this does not have to be done by pages!
// returns true if the image is the same as the hextext, returns false on any error
UCHAR verifyImageSD ()  {
  ULONG lineaddr;
  UCHAR len;
  UCHAR b, cksum = 0;
  UCHAR i;
  
  #ifdef PRINT_LCD
  if(CanShow){
      itoa((int)((int)(g_CpuFreq/2)/((int)(UBRR0L)+(int)1)),tmpbytes);   // 1=0,1 2=0,1,2
      strcatf(tmpbytes,"Khz  ");
      lcd_puts_row(3,tmpbytes);
      delay_ms(200);
  }
  lcd_putsf_row(3,"Reading ");
  #endif  
  while (1) {
    #ifdef PRINT_LCD
    if(CanShow){
        lcd_gotoxy(9,3);
        itoa((int)((float)((float)lineaddr/(float)tmpimage.chipsize)*100),&temptext[0]);
        if((strlen(temptext))<3){
            lcd_puts(temptext);
            lcd_putsf("%");
        }
    }
    #endif
      // read one line!
    if (hextext_read_byte() != ':') {
      error("No colon");
      return FALSE;
    }
    len = hexton(hextext_read_byte());
    len = (len<<4) + hexton(hextext_read_byte());
    cksum = len;

    b = hexton(hextext_read_byte()); // record type
    b = (b<<4) + hexton(hextext_read_byte());
    cksum += b;
    lineaddr = b;
    b = hexton(hextext_read_byte()); // record type
    b = (b<<4) + hexton(hextext_read_byte());
    cksum += b;
    lineaddr = (lineaddr << 8) + b;
    lineaddr+=OffsetX16;//ADD OFFSET *16
    b = hexton(hextext_read_byte()); // record type
    b = (b<<4) + hexton(hextext_read_byte());
    cksum += b;

    //Serial.print("Record type "); Serial.println(b, HEX);
    if (b == 0x1) {
     // end record!
     break;
    }
    // offset *16 record!
    else if (b == 0x2) {
         b = hexton(hextext_read_byte());
         b = (b<<4) + hexton(hextext_read_byte());
         cksum += b;
         OffsetX16=((ULONG)b<<12);
         b = hexton(hextext_read_byte());
         b = (b<<4) + hexton(hextext_read_byte());
         cksum += b;
         OffsetX16|=((ULONG)b<<4);
        b = hexton(hextext_read_byte());  // chxsum
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        if (cksum != 0) {
          error("Bad checksum: ");
          //Serial.print(cksum, HEX);
        }
        if (hextext_read_byte() != '\r') {
          error("No end of line");
          return FALSE;
        }
        if (hextext_read_byte() != '\n') {
          error("No end of line");
          break;
        }
        continue;
    }
    else if (b == 0x3) {
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        b = hexton(hextext_read_byte());
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        b = hexton(hextext_read_byte());  // chxsum
        b = (b<<4) + hexton(hextext_read_byte());
        cksum += b;
        if (cksum != 0) {
            error("Bad checksum: ");
        //Serial.print(cksum, HEX);
        }
        if (hextext_read_byte() != '\r') {
          error("No end of line");
          return FALSE;
        }
        if (hextext_read_byte() != '\n') {
            error("No end of line");
            break;
        }
        continue;
    }
    for (i=0; i < len; i++) {
      // read 'n' bytes
      b = hexton(hextext_read_byte());
      b = (b<<4) + hexton(hextext_read_byte());
      cksum += b;
      // verify this byte!
      if (lineaddr % 2) {
        // for 'high' bytes:
        if (b != g_txBuf[0]){//(spi_transaction(0x28, lineaddr >> 9, lineaddr / 2, 0) & 0xFF)) {
          #ifdef PRINT_DEBUG
          if(debug){
          printf("verification error at address 0x%X",lineaddr);
          while(tx_counter);}
          #endif //puthexchars(lineaddr>>8);puthexchars(lineaddr&0xff);
          #ifdef PRINT_DEBUG
          if(debug){
          printf(" Should be 0x%X",b);
          while(tx_counter);}
          #endif //puthexchars(b);
          #ifdef PRINT_DEBUG
          if(debug){
          printf(" not 0x%X",g_txBuf[0]);
          while(tx_counter);}
          #endif
          //puthexchars(g_txBuf[0]);//Serial.println((spi_transaction(0x28, lineaddr >> 9, lineaddr / 2, 0) & 0xFF), HEX);
          #ifdef PRINT_LCD
          itoa(lineaddr,tmpbytes);
          lcd_putsf_row(0,"lineaddr ");
          lcd_puts(tmpbytes);
          lcd_putsf_row(1,"RD:");
          itoa(g_txBuf[0],tmpbytes);
          lcd_puts(tmpbytes);
          lcd_putsf("WR:");
          itoa(b,tmpbytes);
          lcd_puts(tmpbytes);
          delay_ms(1000);
          //error("Test flash fail");
          #endif
          return FALSE;
        }
      } else {
        // for 'low bytes'
        AVR910_ResetTXData();
        // 1. 'A,0x00,0x01' set address to read from flash. ACK response
        // 2   'R' - read high byte then low byte
        FIFO_Write('A');FIFO_Write(lineaddr>>9);FIFO_Write(lineaddr>>1);AVR910_Command();
        if(g_txBuf[0]!=ACK){
            //puts("\r\nSetAddr FAIL");
            return FALSE;
        }
        AVR910_ResetTXData();
        AVR910_Standalone("R");
        if (b != g_txBuf[1]){//(spi_transaction(0x28, lineaddr >> 9, lineaddr / 2, 0) & 0xFF)) {
          #ifdef PRINT_DEBUG
          if(debug){
          printf("\r\n0=%02x 1=%02x  \r\n",g_txBuf[0],g_txBuf[1]);
          printf("verification error at address 0x%X",lineaddr);
          while(tx_counter);}
          #endif //puthexchars(lineaddr>>8);puthexchars(lineaddr&0xff);
          #ifdef PRINT_DEBUG
          if(debug){
          printf(" Should be 0x%X",b);
          while(tx_counter);}
          #endif // puthexchars(b);
          #ifdef PRINT_DEBUG
          if(debug){
          printf(" not 0x%X",g_txBuf[1]);
          while(tx_counter);}
          #endif
          //puthexchars(g_txBuf[1]);//Serial.println((spi_transaction(0x28, lineaddr >> 9, lineaddr / 2, 0) & 0xFF), HEX);
          
          #ifdef PRINT_LCD
          itoa(lineaddr,tmpbytes);
          lcd_putsf_row(0,"lineaddr ");
          lcd_puts(tmpbytes);
          lcd_putsf_row(1,"RD:");
          itoa(g_txBuf[1],tmpbytes);
          lcd_puts(tmpbytes);
          lcd_putsf("WR:");
          itoa(b,tmpbytes);
          lcd_puts(tmpbytes);
          delay_ms(1000);
          //error("Test flash fail");
          #endif 
          
          return FALSE;
        }
      }
      lineaddr++;
    }

    b = hexton(hextext_read_byte());  // chxsum
    b = (b<<4) + hexton(hextext_read_byte());
    cksum += b;
    if (cksum != 0) {
      error("Bad checksum: ");
      #ifdef PRINT_DEBUG
      if(debug){
      printf("0x%x",cksum);
      while(tx_counter);}
      #endif
      return FALSE;
    }
    if (hextext_read_byte() != '\r') {
      error("No end of line");
      return FALSE;
    }
    if (hextext_read_byte() != '\n') {
      error("No end of line");
      return FALSE;
    }
  }
  return TRUE;
}

/*
 * programmingFuses
 * Program the fuse/lock bits
 */
UCHAR programFuses (UCHAR *fuses,UCHAR *fusemask)
{
  //SPI.setClockDivider(CLOCKSPEED_FUSES);
  UCHAR f;                                
  SetLowSpeed();
  #ifdef PRINT_DEBUG
  if(debug){
  while(tx_counter);
  printf("\r\nSetting fuses");
  while(tx_counter);}
  #endif
  #ifdef PRINT_LCD
  lcd_putsf_row(3,"Setting fuses");
  #endif
  f = pgm_read_byte(&fuses[FUSE_LOW]) | ~pgm_read_byte(&fusemask[FUSE_LOW]);//0x00 | ~0x01=0xFE  = 0xFE
  if (pgm_read_byte(&fusemask[FUSE_LOW])) {
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Set Low Fuses to: 0x%02x",f);
    while(tx_counter);}
    #endif
    AVR910_ResetTXData();
    FIFO_Write('f');//write low fuses
    FIFO_Write(f);
    AVR910_Command();
    AVR910_ResetTXData();
  }
  f = pgm_read_byte(&fuses[FUSE_HIGH]) | ~pgm_read_byte(&fusemask[FUSE_HIGH]);
  if (pgm_read_byte(&fusemask[FUSE_HIGH])) {
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Set High Fuse to: 0x%02x",f);
    while(tx_counter);}
    #endif
    AVR910_ResetTXData();
    FIFO_Write('n');//write high fuses
    FIFO_Write(f);
    AVR910_Command();
    AVR910_ResetTXData();
  }
  f = pgm_read_byte(&fuses[FUSE_EXT]) | ~pgm_read_byte(&fusemask[FUSE_EXT]);
  if (pgm_read_byte(&fusemask[FUSE_EXT])) {
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Set Ext Fuse to: 0x%02x",f);
    while(tx_counter);}
    #endif
    AVR910_ResetTXData();
    FIFO_Write('o');//write ext fuses
    FIFO_Write(f);
    AVR910_Command();
    AVR910_ResetTXData();
  }
  f = pgm_read_byte(&fuses[FUSE_PROT]) | ~pgm_read_byte(&fusemask[FUSE_PROT]); //used when hex in flash
  if (pgm_read_byte(&fusemask[FUSE_PROT])) {
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Set Lock Fuse to: 0x%02x",f);
    while(tx_counter);}
    #endif
    AVR910_ResetTXData();
    FIFO_Write('l');//write lock fuses
    FIFO_Write(f);
    AVR910_Command();
    AVR910_ResetTXData();
  }
  return TRUE;			/* */
}
/*
 * verifyFuses
 * Verifies a fuse set
 */
UCHAR verifyFuses (UCHAR *fuses, UCHAR *fusemask)
{
  //SPI.setClockDivider(CLOCKSPEED_FUSES);
  UCHAR f;
  UCHAR readfuse;
  SetLowSpeed();
  #ifdef PRINT_DEBUG
  if(debug){
  printf("\r\nVerifying fuses...");
  while(tx_counter);}
  #endif
  #ifdef PRINT_LCD
  lcd_putsf_row(3,"Verify fuses.");
  #endif
  f = pgm_read_byte(&fuses[FUSE_LOW]) | ~pgm_read_byte(&fusemask[FUSE_LOW]);
  if (pgm_read_byte(&fusemask[FUSE_LOW])) {
    AVR910_Standalone("F");  // low fuse
    readfuse=g_txBuf[0];
    readfuse |= ~pgm_read_byte(&fusemask[FUSE_LOW]);
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Low Fuse Writen: 0x%02x",f); printf("\r\n  Low Fuse Readed: 0x%02x",readfuse);
    while(tx_counter);}
    #endif
    if (readfuse != f)
      return FALSE;
  }
  f = pgm_read_byte(&fuses[FUSE_HIGH]) | ~pgm_read_byte(&fusemask[FUSE_HIGH]);
  if (pgm_read_byte(&fusemask[FUSE_HIGH])) {
    AVR910_Standalone("N"); // high fuse
    readfuse=g_txBuf[0];
    readfuse |= ~pgm_read_byte(&fusemask[FUSE_HIGH]);
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  High Fuse Writen: 0x%02x",f); printf("\r\n  High Fuse Readed: 0x%02x",readfuse);
    while(tx_counter);}
    #endif
    if (readfuse != f)
      return FALSE;
  }
  f = pgm_read_byte(&fuses[FUSE_EXT]) | ~pgm_read_byte(&fusemask[FUSE_EXT]);
  if (pgm_read_byte(&fusemask[FUSE_EXT])) {
    AVR910_Standalone("O"); // ext fuse
    readfuse=g_txBuf[0];
    readfuse |= ~pgm_read_byte(&fusemask[FUSE_EXT]);
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Ext Fuse Writen: 0x%02x",f); printf("\r\n  Ext Fuse Readed: 0x%02x",readfuse);
    while(tx_counter);}
    #endif
    if (readfuse != f)
      return FALSE;
  }
  f = pgm_read_byte(&fuses[FUSE_PROT]) | ~pgm_read_byte(&fusemask[FUSE_PROT]);
  if (pgm_read_byte(&fusemask[FUSE_PROT])) {
    AVR910_Standalone("Q");  // lock fuse
    readfuse=g_txBuf[0];
    readfuse |= ~pgm_read_byte(&fusemask[FUSE_PROT]);
    #ifdef PRINT_DEBUG
    if(debug){
    printf("\r\n  Lock Fuse Writen: 0x%02x",f); printf("\r\n  Lock Fuse Readed: 0x%02x",readfuse);
    while(tx_counter);}
    #endif
    if (readfuse != f)
      return FALSE;
  }
  return TRUE;			/* */
}

