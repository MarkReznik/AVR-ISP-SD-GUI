// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* - File              : Self_programming.c
* - Compiler          : IAR EWAVR 2.28a / 3.10c and newer
*
* - Support mail      : avr@atmel.com
*
* - Supported devices : All devices with bootloaders support.
*
* - AppNote           : AVR106 - C functions for reading and writing
*                       to flash memory.
*
* - Description       : The file contains functions for easy reading and writing
*                       of Flash memory on parts having the "Self-programming"
*                       feature. The user functions are as follows:
*
*                       ReadFlashByte()	
*                       ReadFlashPage()	
*                       WriteFlashByte()
*                       WriteFlashPage()	
*                       RecoverFlash()	
*
*                       The remaining functions contained in this file are used
*                       by the functions listet above.
*
* $Revision: 2.0 $
* $Date: Wednesday, January 18, 2006 15:18:52 UTC $
*
****************************************************************************/
#include <io.h>
//#include <inavr.h>
#include "Self_programming.h"
#include "flash.h"
#include <delay.h>


//void (*__AddrToZ24ByteToSPMCR_SPM_W_Test)(void flash *addr)= (void(*)(void flash *)) 0x0F9E0;
//void (*__AddrToZ24WordToR1R0ByteToSPMCR_SPM_F_Test)(void flash *addr, unsigned int data)= (void(*)(void flash *, unsigned int)) 0x0F9B4;


/*!
* Declare global struct variable in EEPROM if Flash recovery enabled.
* FlashBackup pageNumber holds Flash pageaddress (/PAGESIZE) the data in Flash
* recovery buffer should be written to if data need to be recovered.
* FlashBackup.status tells if data need to be recovered.
**/
#ifdef __FLASH_RECOVER
__eeprom struct {
  unsigned int  pageNumber;
  unsigned char status;
}FlashBackup = {0};
#endif


/*!
* The function Returns one byte located on Flash address given by ucFlashStartAdr.
**/
unsigned char ReadFlashByte(MyAddressType flashStartAdr){
//#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
  return (unsigned char)*((MyFlashCharPointer)flashStartAdr);
//#pragma diag_default=Pe1053 // Back to default.
} // Returns data from Flash

/*!
* The function reads one Flash page from address flashStartAdr and stores data
* in array dataPage[]. The number of bytes stored is depending upon the
* Flash page size. The function returns FALSE if input address is not a Flash
* page address, else TRUE.
**/
unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage){
  unsigned int index;
  if(!(flashStartAdr & (PAGESIZE-1))){      // If input address is a page address
    for(index = 0; index < PAGESIZE; index++){
      dataPage[index] = ReadFlashByte(flashStartAdr + index);
    }
    return TRUE;                            // Return TRUE if valid page address
  }
  else{
    return FALSE;                           // Return FALSE if not valid page address
  }
}

/*!
* The function writes byte data to Flash address flashAddr. Returns FALSE if
* input address is not valid Flash byte address for writing, else TRUE.
**/
unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data){
  MyAddressType  pageAdr;
  unsigned char eepromInterruptSettings,sregSettings;
  if( AddressCheck( flashAddr & ~(PAGESIZE-1) )){

    eepromInterruptSettings= EECR & (1<<EERIE); // Stores EEPROM interrupt mask
    EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt
    _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));                // Wait if ongoing EEPROM write
    
    sregSettings= SREG;
    #asm("cli");
    pageAdr=flashAddr & ~(PAGESIZE-1);      // Gets Flash page address from byte address

    #ifdef __FLASH_RECOVER
    FlashBackup.status=0;                   // Inicate that Flash buffer does
                                            // not contain data for writing
    _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
    LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
    WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
    FlashBackup.pageNumber = (unsigned int) (pageAdr/PAGESIZE); // Stores page address
                                                       // data should be written to
    FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer
                                               // contains unwritten data
    _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
    #endif

    LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
    
    
    WriteBufToFlash(pageAdr);               // Writes to Flash

    #ifdef __FLASH_RECOVER
    FlashBackup.status = 0;                 // Indicates that Flash recovery buffer
                                            // does not contain unwritten data
    _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
    #endif

    EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
    SREG = sregSettings;
    return TRUE;                            // Return TRUE if address
                                            // valid for writing
  }
  else
    return FALSE;                           // Return FALSE if address not
                                            // valid for writing
}

/*!
* The function writes data from array dataPage[] to Flash page address
* flashStartAdr. The Number of bytes written is depending upon the Flash page
* size. Returns FALSE if input argument ucFlashStartAdr is not a valid Flash
* page address for writing, else TRUE.
**/
unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)
{
  unsigned int index=0;
  unsigned char eepromInterruptSettings,sregSettings;
  if( AddressCheck(flashStartAdr) ){
    eepromInterruptSettings = EECR & (1<<EERIE); // Stoes EEPROM interrupt mask
    EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt
    _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));                // Wait if ongoing EEPROM write
    
    sregSettings= SREG;
    #asm("cli");
    
    #ifdef __FLASH_RECOVER
    FlashBackup.status=0;                   // Inicate that Flash buffer does
                                            // not contain data for writing
    while(EECR & (1<<EEWE));
    
    //_ENABLE_RWW_SECTION();
    
    _WAIT_FOR_SPM();
    //_PAGE_ERASE( flashStartAdr );
    
    for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer
      //_WAIT_FOR_SPM();
      //MY_FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
      _FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
    }
    
    WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
    FlashBackup.pageNumber=(unsigned int)(flashStartAdr/PAGESIZE);
    FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer
                                           // contains unwritten data
    _WAIT_FOR_EEWE();//vwhile(EECR & (1<<EEWE));
    #endif
    
    if(index==0xFFF1)
    {
        #ifdef _ATMEGA128
        __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F(0,0);
        __AddrToZ24ByteToSPMCR_SPM_EW((void flash *)0x1EF00);
        #endif
        #ifdef _ATMEGA328
        __AddrToZWordToR1R0ByteToSPMCR_SPM_F(0,0);
        __AddrToZByteToSPMCR_SPM_EW((void flash *)0x1EF00);
        #endif
    }
    _WAIT_FOR_SPM();
    _PAGE_ERASE( flashStartAdr );
    //_ENABLE_RWW_SECTION();
    
    for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer
      //_WAIT_FOR_SPM();
      //MY_FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
      _FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
    }
    //_PAGE_WRITE( flashStartAdr );
    WriteBufToFlash(flashStartAdr);         // Writes to Flash
    #ifdef __FLASH_RECOVER
      FlashBackup.status=0;                 // Inicate that Flash buffer does
                                            // not contain data for writing
      while(EECR & (1<<EEWE));
    #endif

    EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
    SREG = sregSettings;
    return TRUE;                            // Return TRUE if address
                                            // valid for writing
  }
  else
    return FALSE;                           // Return FALSE if not address not
                                            // valid for writing
}

/*!
* The function checks if global variable FlashBackup.status indicates that Flash recovery
* buffer contains data that needs to be written to Flash. Writes data from
* Flash recovery buffer to Flash page address given by FLASH_recovery.pageAdr.
* This function should be called at program startup if FLASH recovery option
* is enabeled.
**/
unsigned char RecoverFlash(){
#ifdef __FLASH_RECOVER
  unsigned int index;
  unsigned long flashStartAdr = (MyAddressType)FlashBackup.pageNumber * PAGESIZE;
  if(FlashBackup.status == FLASH_BUFFER_FULL_ID){ // Checks if Flash recovery
                                                  //  buffer contains data
    
    for(index=0; index < PAGESIZE; index+=2){     // Writes to Flash write buffer
        _WAIT_FOR_SPM();
        MY_FILL_TEMP_WORD( index, *((MyFlashIntPointer)(ADR_FLASH_BUFFER+index)) );
    }
    
    
    //WriteBufToFlash((MyAddressType)FlashBackup.pageNumber * PAGESIZE);
    _WAIT_FOR_SPM();
    MY_PAGE_ERASE( flashStartAdr );
    _WAIT_FOR_SPM();
    MY_PAGE_WRITE( flashStartAdr );
    _WAIT_FOR_SPM();
    _ENABLE_RWW_SECTION();
    FlashBackup.status=0;                   // Inicate that Flash buffer does
                                            // not contain data for writing
    while(EECR & (1<<EEWE));
    return TRUE;                            // Returns TRUE if recovery has
                                            // taken place
  }
#endif
  return FALSE;
}


/*!
* The function checks if input argument is a valid Flash page address for
* writing. Returns TRUE only if:
* - Address points to the beginning of a Flash page
* - Address is within the limits defined in Self_programming.h
* - Address is not equal to page address used for buffring by the Flash recovery
*   functions (if enabled).
* Returns FALSE else.
**/
unsigned char AddressCheck(MyAddressType flashAdr){
  #ifdef __FLASH_RECOVER
  // The next line gives a warning 'pointless comparison with zero' if ADR_LIMIT_LOW is 0. Ignore it.
  if( (flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) &&
      (flashAdr != ADR_FLASH_BUFFER) && !(flashAdr & (PAGESIZE-1)) )
    return TRUE;                            // Address is a valid page address
  else
    return FALSE;                           // Address is not a valid page address
  #else
  if((flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) && !(flashAdr & (PAGESIZE-1) ) )
    return TRUE;                            // Address is a valid page address
  else
  {
    /*
    while(1)
    {
      PORTC.5=0;
      delay_ms(500);
      PORTC.5=1;
      delay_ms(500);      
    }
    */
    return FALSE;                           // Address is not a valid page address
  }
  #endif
}


/*!
* The function writes Flash temporary buffer to Flash page address given by
* input argument.
**/

void WriteBufToFlash(MyAddressType flashStartAdr) {
    //_WAIT_FOR_SPM();
    //MY_PAGE_ERASE( flashStartAdr );
    //_PAGE_ERASE( flashStartAdr );
    //_WAIT_FOR_SPM();
    //_ENABLE_RWW_SECTION();
    //MY_PAGE_WRITE( flashStartAdr );
    //_PAGE_ERASE( flashStartAdr );
    _PAGE_WRITE( flashStartAdr );
    //_WAIT_FOR_SPM();
    //_ENABLE_RWW_SECTION();
/*
#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
  #ifdef __HAS_RAMPZ__
  RAMPZ = (unsigned char)(flashStartAdr >> 16);
  #endif
  _PAGE_ERASE(flashStartAdr);
  while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
  _PAGE_WRITE(flashStartAdr);
  while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
  #ifdef RWWSRE
  __DataToR0ByteToSPMCR_SPM( 0, (unsigned char)(1<<RWWSRE)|(1<<SPMEN)); // Enable RWW
  #endif
#pragma diag_default=Pe1053 // Back to default.
*/
}

/*!
* The function reads Flash page given by flashAddr, replaces one byte given by
* flashAddr with data, and stores entire page in Flash temporary buffer.
**/
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data){
//#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
    unsigned int index, oddByte, pcWord;
    
    MyAddressType  pageAdr;
    oddByte=(unsigned char)flashAddr & 1;
    pcWord=(unsigned int)flashAddr & (PAGESIZE-2); // Used when writing FLASH temp buffer
    pageAdr=flashAddr & ~(PAGESIZE-1);        // Get FLASH page address from byte address
    //while( SPMCR_REG & (1<<SPMEN) );
    //_ENABLE_RWW_SECTION();
    
    for(index=0; index < PAGESIZE; index+=2){
        if(index==pcWord){
          if(oddByte){
            //MY_FILL_TEMP_WORD( index, (*(MyFlashCharPointer)(flashAddr & ~1) | ((unsigned int)data<<8)) );
            _FILL_TEMP_WORD( index, (*(MyFlashCharPointer)(flashAddr & ~1) | ((unsigned int)data<<8)) );
          }                                     // Write odd byte in temporary buffer
          else{
            //MY_FILL_TEMP_WORD( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );
            _FILL_TEMP_WORD( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );
          }                                     // Write even byte in temporary buffer
        }
        else{
          //MY_FILL_TEMP_WORD(index, *( (MyFlashIntPointer)(pageAdr+index) ) );
          _FILL_TEMP_WORD(index, *( (MyFlashIntPointer)(pageAdr+index) ) );
        }                                       // Write Flash word directly to temporary buffer
    }
//#pragma diag_default=Pe1053 // Back to default.
}
