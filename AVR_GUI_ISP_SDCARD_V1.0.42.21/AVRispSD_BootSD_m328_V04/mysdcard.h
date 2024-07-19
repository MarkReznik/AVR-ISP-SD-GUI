#ifndef MYSDCARD_H
#define MYSDCARD_H

#include "pff.h"

extern FATFS fs;
extern unsigned char buffer[];
extern unsigned int nbytes,nbytesCounter;
extern volatile unsigned char CounterLed;
extern char path[];
extern ULONG ReadByteCounter;
extern ULONG WriteByteCounter;
extern DWORD LastOffset;

//char *GetValue(flash char *Key);

void init_sd_tim1(void);
UCHAR sd_mount(void);
UCHAR fileOpen();
UCHAR fileRead(void);
void fileClose(void);
UCHAR pgm_read_byte_sd(void);
//UCHAR SetValueReady(flash char *Key,const char ready_val);
void init_sd_tim1(void);
UCHAR SetFlashArea(void);
UCHAR SetConfigArea(void);
UCHAR SetRollValue(void);
char *ReadValue(flash char *Key);
UCHAR WriteConfigValue(flash char *Key,char *Values, UINT len);
UCHAR ReadFileToBuffer();
UCHAR fileReadBin(void);
//UCHAR fileWriteBin(void);
UCHAR SetFileOffset(DWORD offset);
UCHAR SetBinFileOffset(DWORD offset);
UCHAR SetBinFlashOffset(DWORD offset);
UCHAR readPageBin(UINT bytesToRead,UCHAR *pbuffer);
#endif