/*****************************************************************************
*
* (C) 2010, HP InfoTech srl, www.hpinfotech.com
*
* File              : flash.c
* Compiler          : CodeVisionAVR V2.xx
* Revision          : $Revision: 1.0 $
* Date              : $Date: December 9, 2010 $
* Updated by        : $Author: HP InfoTech $
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description       : Flash operations for AVR109 Self-programming
****************************************************************************/

#include <io.h>
#include "flash.h"

//#define	SPMCR_REG	SPMCSR

void dospmew(void)
{
    #asm
         ldi   r22,$03
         WR_SPMCR_REG_R22
         spm
    #endasm
    _WAIT_FOR_SPM();
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    #asm
        ldi   r22,$05
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    do
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
    while( SPMCR_REG & (1<<RWWSB) );    
}

void dospmw(void)
{
    #asm
        ldi   r22,$05
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    do
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
    while( SPMCR_REG & (1<<RWWSB) );    
}

void dospme(void)
{
    #asm
        ldi   r22,$03
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    //while( SPMCR_REG & (1<<RWWSB) )
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }    
}

void dospm(void)
{
    #asm
         ldi   r22,$01
         WR_SPMCR_REG_R22
         spm
    #endasm
    _WAIT_FOR_SPM();
    while( SPMCR_REG & (1<<RWWSB) )
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }    
}

#pragma warn-

unsigned char __AddrToZByteToSPMCR_LPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     lpm
     mov  r30,r0
#endasm
}

void __DataToR0ByteToSPMCR_SPM(unsigned char data, unsigned char ctrl)
{
#asm
     ldd  r0,y+1
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZWordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl) 
{
    #asm
         ldd  r30,y+3
         ldd  r31,y+4
         ldd  r0,y+1
         ldd  r1,y+2
         ld   r22,y
         WR_SPMCR_REG_R22
         spm
    #endasm
} 

void __AddrToZWordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data) 
{
_WAIT_FOR_SPM();
    #asm
         ldd  r30,y+2
         ldd  r31,y+3
         ldd  r0,y+0
         ldd  r1,y+1
         //ldi   r22,LOW(1)
         //WR_SPMCR_REG_R22
         //spm
    #endasm
dospm();
}

void __AddrToZByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZByteToSPMCR_SPM_E(void flash *addr)
{
    _WAIT_FOR_SPM();
    #asm
         ldd  r30,y+0
         ldd  r31,y+1
         //ld   r22,y
         //WR_SPMCR_REG_R22
         //spm
    #endasm
    #asm
        ldi   r22,$03
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    //while( SPMCR_REG & (1<<RWWSB) )
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
//dospme();
}

void __AddrToZByteToSPMCR_SPM_W(void flash *addr)
{
    _WAIT_FOR_SPM();
    #asm
         ldd  r30,y+0
         ldd  r31,y+1
         //ld   r22,y
         //WR_SPMCR_REG_R22
         //spm
    #endasm
    #asm
        ldi   r22,$05
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    do
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
    while( SPMCR_REG & (1<<RWWSB) );
//dospmw();
}

void __AddrToZByteToSPMCR_SPM_EW(void flash *addr)
{
    _WAIT_FOR_SPM();
    #asm
         ldd  r30,y+0
         ldd  r31,y+1
         //ld   r22,y
         //WR_SPMCR_REG_R22
         //spm
    #endasm
    #asm
        ldi   r22,$03
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    //while( SPMCR_REG & (1<<RWWSB) )
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
//dospme();
    #asm
         ldd  r30,y+0
         ldd  r31,y+1
         //ld   r22,y
         //WR_SPMCR_REG_R22
         //spm
    #endasm
    #asm
        ldi   r22,$05
        WR_SPMCR_REG_R22
        spm
    #endasm
    _WAIT_FOR_SPM();
    do
    {
    #asm
        ldi   r22,$11
        WR_SPMCR_REG_R22
        spm
    #endasm
        _WAIT_FOR_SPM();
    }
    while( SPMCR_REG & (1<<RWWSB) );
//dospmw();
//dospmew();
}

 
#ifdef _ATMEGA128
void __AddrToZ24WordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl)
{
#asm
     ldd  r30,y+3
     ldd  r31,y+4
     ldd  r22,y+5
     out  rampz,r22
     ldd  r0,y+1
     ldd  r1,y+2
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data)
{
_WAIT_FOR_SPM();
#asm
     ldd  r30,y+2
     ldd  r31,y+3
     ldd  r22,y+4
     out  rampz,r22
     ldd  r0,y+0
     ldd  r1,y+1
     //ld   r22,y
     //WR_SPMCR_REG_R22
     //spm
#endasm
dospm();
}

void __AddrToZ24ByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
{
#asm
     ldd  r30,y+1
     ldd  r31,y+2
     ldd  r22,y+3
     out  rampz,r22
     ld   r22,y
     WR_SPMCR_REG_R22
     spm
#endasm
}

void __AddrToZ24ByteToSPMCR_SPM_W(void flash *addr)
{
_WAIT_FOR_SPM();
#asm
     ldd  r30,y+0
     ldd  r31,y+1
     ldd  r22,y+2
     out  rampz,r22
     //ld   r22,y
     //WR_SPMCR_REG_R22
     //spm
#endasm
dospmw();
}

void __AddrToZ24ByteToSPMCR_SPM_E(void flash *addr)
{
_WAIT_FOR_SPM();
#asm
     ldd  r30,y+0
     ldd  r31,y+1
     ldd  r22,y+2
     out  rampz,r22
     //ld   r22,y
     //WR_SPMCR_REG_R22
     //spm
#endasm
dospme();
}

void __AddrToZ24ByteToSPMCR_SPM_EW(void flash *addr)
{
_WAIT_FOR_SPM();
#asm
     ldd  r30,y+0
     ldd  r31,y+1
     ldd  r22,y+2
     out  rampz,r22
     //ld   r22,y
     //WR_SPMCR_REG_R22
     //spm
#endasm
dospmew();
}
#endif

#ifdef _WARNINGS_ON_
#pragma warn+
#endif

