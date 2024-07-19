#ifndef MYCODE_H
#define MYCODE_H


#define PAGE_SIZE_BYTES 256 //AVR Atmega. Block Programming Page size in bytes
//fuse definitions
#define MAX_FUSES   4
#define FUSE_PROT 0            /* memory protection */
#define FUSE_LOW 1            /* Low fuse */
#define FUSE_HIGH 2            /* High fuse */
#define FUSE_EXT 3			/* Extended fuse */

#define MAX_PROJECTS    (1000/10)//(EEPROM(1K=1024)/(sizeof(eeimage)))
typedef enum{IDLE=0,PASS,FAIL,RUNNING,SDFAIL,READSD} STANDALONE_MODE;

#define ROLL_WINDOW 5
#define ROLL_SIZE   65

/*
#define IDLE    0
#define PASS    1
#define FAIL    2
#define RUNNING 3
*/
extern UCHAR tmpbytes[];//length 32
extern UCHAR *value;
extern volatile STANDALONE_MODE      StandaloneMode;
extern volatile unsigned int Counter10ms;
extern UCHAR      debug;
extern UCHAR    dirnum; //unique file number in eeprom directory of project
extern UCHAR    folderCounter; //folders inccrement counter
extern eeprom  UCHAR    SaveDirNum;
extern eeprom  UCHAR   SaveLockState;
extern UCHAR spidiv;

//sdcard struct to handle project and hex file definitions
typedef struct image {
    char hexfile[16];           /* Ie "12345678.hex" 8+'.'+3 12 chars*/
    char progname[16];          /* Ie "COM118 00 " 20 chars*/
    char chipname[16];           /* ie "atmega168" 12 chars*/
    UCHAR chipcode;                  /* device code ie m64 is 0x45 */
    UCHAR signature[3];           /* bytes of signature mega8 = 1E 93 02*/
    UCHAR progfuses[MAX_FUSES];           /* lock,low,high,ext fuses to set during programming */
    UCHAR normfuses[MAX_FUSES];           /* lock,low,high,ext fuses to set after programming */
    UCHAR maskfuses[MAX_FUSES];                  /* lock,low,high,ext mask fuses 00 means not avilable */
    ULONG chipsize;                 /* chip size in bytes mega8 = 8192 */
    UINT pagesizebytes;           /* page size in bytes for flash programming mega8 = 128*/
    UINT ProgCounterDone;           /* Counter of success programming maximum ProgCounterTotal*/
    UINT ProgCounterTotal;         /* Total programming available maximum 2^32*/
    //UCHAR ProgCounterReset;         /* Reset program counter boolean condition */
    char fw_ver[32];                   /* Firmware version x.xx.xxx  maximum 10 chars        */
    char debug;
    UCHAR rollValue;
    UCHAR spidiv;
    UINT eepromsizebytes;           /* EEPROM size in bytes */
    UCHAR eepromfile;
    UCHAR save_serial;
    UCHAR save_calib;
} image_t;
//eeprom struct to handle project and hex file definitions
typedef struct eeimage {  //10bytes*100=1000bytes total is 1024 bytes
    UINT ProgCounterDone;     /* 2^16=64K Counter of success programming maximum ProgCounterTotal*/
    //UCHAR EncryptCounter;         /* encryption counter*/
    UCHAR FolderUniqueValue[3];    //folder unique values -> [0x11][0x12][0x22] 1=0xFFF=EncryptCounter  2=0xFFF=FileUniqueValue  
    //UINT FileUniqueValue;  //file unique value
} eeimage_t;

/*****************************************************************************
uCU definitions
******************************************************************************/
//#define CPU_CLOCK			16000000 	// freq in Hz

/*****************************************************************************
Types definitions
******************************************************************************/
#define PROGMEM     flash

extern image_t tmpimage;
extern UCHAR g_UpSkip;
extern UCHAR g_DownSkip;
extern UCHAR g_MaxFolders;
extern int   g_CpuFreq;
extern UCHAR g_MyUDR;
#define     MSG_SIZE    10 
extern char msg_buf[MSG_SIZE][16];
extern char msg_buf_cnt;
extern char CanShow;
extern UCHAR pageBuffer[];
extern ULONG pageaddr;
extern bit blankpage;
extern bit EndOfFile;

#ifdef PRINT_LCD
void ChangeMsg(void);
#endif
void MsgBufClr(void);
void StartADC(unsigned char ch);
UCHAR hexton (UCHAR h);
CHAR ntohexchar (UCHAR n);
UCHAR * ntohexstr (UCHAR n);
UCHAR hextobyte(char *str);
UINT hextouint(char *str);
ULONG hextoulong(char *str);
void hextobytese(char *str,eeprom UCHAR *arr,UCHAR len);
void hextobytes(char *str,UCHAR *arr,UCHAR len);
UCHAR readImagePageSD (ULONG pageaddr, UINT pagesize, UCHAR *page);
UCHAR verifyImageSD(void);
UCHAR SaveSerialToEepromAndInc(void);
UCHAR WriteChipEeprom(UCHAR *data, UINT len, UINT startaddr);
UCHAR VerifyChipEeprom(UCHAR *data, UINT len, UINT startaddr);
UCHAR ReadChipEeprom(void);
#ifdef BOOTEEPROM
void CopyEepromToRam(void);
#endif
UCHAR programFuses (UCHAR *fuses,UCHAR *fusemask);
UCHAR verifyFuses (UCHAR *fuses, UCHAR *fusemask);
UCHAR flashPage (UCHAR *pagebuff, UINT pagesize);
signed char strcmpe(char *str1,eeprom char *str2);

void PrintImage(void);
UCHAR Call_GetDir(void);
UCHAR Call_GetConfig(void);
void printFlashString(flash char *str);
void ResetGlobals(void);
void SetProgMode(void);
void ReadSignature(void);
UCHAR Call_ReadSignature(void);
void SetHiSpeed(void);
void SetLowSpeed(void);
#endif