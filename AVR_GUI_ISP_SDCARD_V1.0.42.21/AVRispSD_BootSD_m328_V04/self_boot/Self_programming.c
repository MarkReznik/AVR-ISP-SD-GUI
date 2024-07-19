/*****************************************************************************  
*  
* Atmel Corporation  
*  
* File              : Self_programming.c  
* Compiler          : IAR EWAAVR 2.28a / 3.10c  
* Revision          : $Revision: 1.5 $  
* Date              : $Date: Monday, April 04, 2005 12:13:24 UTC $  
* Updated by        : $Author: raapeland $  
*  
* Support mail      : avr@atmel.com  
*  
* Supported devices : Any device having the "Self-programming" feature.  
*  
* AppNote           : AVR106: C functions for reading and writing to FLASH.  
*  
* Description       : The file contains functions for easy reading and writing  
*                     of Flash memory on parts having the "Self-programming"  
*                     feature. The user functions are as follows:  
*  
*                     ReadFlashByte()  
*                     ReadFlashPage()  
*                     WriteFlashByte()  
*                     WriteFlashPage()  
*                     RecoverFlash()  
*  
*                     The remaining functions contained in this file are used  
*                     by the functions listet above.  
******************************************************************************/   
//#include <ioavr.h> 
#include <avr/io.h> 
//#include <inavr.h>   
#include "Self_programming.h"   
   
/* These EEPROM bits have different names on different devices. */   
#ifndef EEPE   
        #define EEPE  EEWE  //!< EEPROM program/write enable.   
        #define EEMPE EEMWE //!< EEPROM master program/write enable.   
#endif   
   
/******************************************************************************  
* Declare global struct variable in EEPROM if Flash recovery enabled.  
* FlashBackup pageNumber holds Flash pageaddress (/PAGESIZE) the data in Flash  
* recovery buffer should be written to if data need to be recovered.  
* FlashBackup.status tells if data need to be recovered.  
*****************************************************************************/   
#ifdef __FLASH_RECOVER   
__eeprom struct {   
  unsigned int  pageNumber;   
  unsigned char status;   
}FlashBackup = {0};   
#endif   
   
/******************************************************************************  
* Function: ReadFlashByte(MyAddressType ucFlashStartAdr)  
* Description:  
* Function Returns one byte located on Flash address given by ucFlashStartAdr.  
******************************************************************************/   
unsigned char ReadFlashByte(MyAddressType flashStartAdr){   
  return const( unsigned char)*((MyFlashCharPointer) flashStartAdr);   
} // Returns data from Flash   
   
/******************************************************************************  
* Function: ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)  
* Description;  
* Function reads one Flash page from address flashStartAdr and stores data  
* in array dataPage[]. The number of bytes stored is depending upon the  
* Flash page size. The function returns FALSE if input address is not a Flash  
* page address, else TRUE.  
******************************************************************************/   
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
   
/******************************************************************************  
* Function: WriteFlashByte(MyAddressType flashAddr, unsigned char data)  
* Description;  
* Function writes byte data to Flash address flashAddr. Returns FALSE if  
* input address is not valid Flash byte address for writing, else TRUE.  
******************************************************************************/   
unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data){   
  MyAddressType  pageAdr;   
  unsigned char eepromInterruptSettings;   
  if( AddressCheck( flashAddr & ~(PAGESIZE-1) )){   
   
    eepromInterruptSettings= EECR & (1<<EERIE); // Stores EEPROM interrupt mask   
    EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt   
    while(EECR & (1<<EEPE));                // Wait if ongoing EEPROM write   
   
    pageAdr=flashAddr & ~(PAGESIZE-1);      // Gets Flash page address from byte address   
   
    #ifdef __FLASH_RECOVER   
    FlashBackup.status=0;                   // Inicate that Flash buffer does   
                                            // not contain data for writing   
    while(EECR & (1<<EEPE));   
    LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer   
    WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer   
    FlashBackup.pageNumber = (unsigned int) (pageAdr/PAGESIZE); // Stores page address   
                                                       // data should be written to   
    FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer   
                                               // contains unwritten data   
    while(EECR & (1<<EEPE));   
    #endif   
   
    LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer   
    WriteBufToFlash(pageAdr);               // Writes to Flash   
   
    #ifdef __FLASH_RECOVER   
    FlashBackup.status = 0;                 // Indicates that Flash recovery buffer   
                                            // does not contain unwritten data   
    while(EECR & (1<<EEPE));   
    #endif   
   
    EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask   
    return TRUE;                            // Return TRUE if address   
                                            // valid for writing   
  }   
  else   
    return FALSE;                           // Return FALSE if address not   
                                            // valid for writing   
}   
   
/******************************************************************************  
* Function: WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)  
* Description:  
* Function writes data from array dataPage[] to Flash page address  
* flashStartAdr. The Number of bytes written is depending upon the Flash page  
* size. Returns FALSE if input argument ucFlashStartAdr is not a valid Flash  
* page address for writing, else TRUE.  
******************************************************************************/   
unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)   
{   
  unsigned int index;   
  unsigned char eepromInterruptSettings;   
  if( AddressCheck(flashStartAdr) ){   
    eepromInterruptSettings = EECR & (1<<EERIE); // Stoes EEPROM interrupt mask   
    EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt   
    while(EECR & (1<<EEPE));                // Wait if ongoing EEPROM write   
   
    #ifdef __FLASH_RECOVER   
    FlashBackup.status=0;                   // Inicate that Flash buffer does   
                                            // not contain data for writing   
    while(EECR & (1<<EEPE));   
    for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer   
      _SPM_FILLTEMP(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));   
    }   
    WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer   
    FlashBackup.pageNumber=(unsigned int)(flashStartAdr/PAGESIZE);   
    FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer   
                                           // contains unwritten data   
    while(EECR & (1<<EEPE));   
    #endif   
    for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer   
      #ifdef __HAS_RAMPZ__   
      RAMPZ = (unsigned char)(flashStartAdr >> 16);   
      #endif   
      _SPM_FILLTEMP(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));   
    }   
    WriteBufToFlash(flashStartAdr);         // Writes to Flash   
    #ifdef __FLASH_RECOVER   
      FlashBackup.status=0;                 // Inicate that Flash buffer does   
                                            // not contain data for writing   
      while(EECR & (1<<EEPE));   
    #endif   
   
    EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask   
    return TRUE;                            // Return TRUE if address   
                                            // valid for writing   
  }   
  else   
    return FALSE;                           // Return FALSE if not address not   
                                            // valid for writing   
}   
   
/******************************************************************************  
* Function: unsigned char RecoverFlash()  
* Description:  
* Function checks if global variable FlashBackup.status indicates that Flash recovery  
* buffer contains data that needs to be written to Flash. Writes data from  
* Flash recovery buffer to Flash page address given by FLASH_recovery.pageAdr.  
* This function should be called at program startup if FLASH recovery option  
* is enabeled.  
******************************************************************************/   
unsigned char RecoverFlash(void){   
#ifdef __FLASH_RECOVER   
  unsigned int index;   
  if(FlashBackup.status == FLASH_BUFFER_FULL_ID){ // Checks if Flash recovery   
                                                  //  buffer contains data   
    for(index=0; index < PAGESIZE; index+=2){     // Writes to Flash write buffer   
        _SPM_FILLTEMP( index, *((MyFlashIntPointer)(ADR_FLASH_BUFFER+index)) );   
    }   
    WriteBufToFlash((MyAddressType)FlashBackup.pageNumber * PAGESIZE);   
    FlashBackup.status=0;                   // Inicate that Flash buffer does   
                                            // not contain data for writing   
    while(EECR & (1<<EEPE));   
    return TRUE;                            // Returns TRUE if recovery has   
                                            // taken place   
  }   
#endif   
  return FALSE;   
}   
   
   
/******************************************************************************  
* Function: AddressCheck(MyAddressType flashAdr)  
* Description:  
* Functions checks if input argument is a valid Flash page address for  
* writing. Returns TRUE only if:  
* - Address points to the beginning of a Flash page  
* - Address is within the limits defined in Self_programming.h  
* - Address is not equal to page address used for buffring by the Flash recovery  
*   functions (if enabled).  
* Returns FALSE else.  
******************************************************************************/   
unsigned char AddressCheck(MyAddressType flashAdr){   
  #ifdef __FLASH_RECOVER   
  if( (flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) &&   
      (flashAdr != ADR_FLASH_BUFFER) && !(flashAdr & (PAGESIZE-1)) )   
    return TRUE;                            // Address is a valid page address   
  else   
    return FALSE;                           // Address is not a valid page address   
  #else   
  #if (ADR_LIMIT_LOW == 0)      // avoid compiler warning - radmoon   
  if((flashAdr <= ADR_LIMIT_HIGH) && !(flashAdr & (PAGESIZE-1) ) )   
  #else   
  if((flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) && !(flashAdr & (PAGESIZE-1) ) )   
  #endif   
    return TRUE;                            // Address is a valid page address   
  else   
    return FALSE;                           // Address is not a valid page address   
  #endif   
}   
   
   
/******************************************************************************  
* Function: WriteBufToFlash(MyAddressType ucFlashStartAdr)  
* Description:  
* Function writes Flash temporary buffer to Flash page address given by  
* input argument.  
******************************************************************************/   
void WriteBufToFlash(MyAddressType flashStartAdr){   
  #ifdef __HAS_RAMPZ__   
  RAMPZ = (unsigned char)(flashStartAdr >> 16);   
  #endif   
  _SPM_ERASE(flashStartAdr);        // ignore compiler warning! - radmoon   
  while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed   
  _SPM_PAGEWRITE(flashStartAdr);    // ignore compiler warning! - radmoon   
  while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed   
  #ifdef RWWSRE   
  __DataToR0ByteToSPMCR_SPM( 0, (unsigned char)(1<<RWWSRE)|(1<<SPMEN)); // Enable RWW   
  #endif   
}   
   
/******************************************************************************  
* Function: LpmReplaceSpm(MyAddressType flashAddr, unsigned char data)  
* Description:  
* The functions reads Flash page given by flashAddr, replaces one byte given by  
* flashAddr with data, and stores entire page in Flash temporary buffer.  
******************************************************************************/   
void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data){   
  unsigned int index, oddByte, pcWord;   
  MyAddressType  pageAdr;   
  oddByte=(unsigned char)flashAddr & 1;   
  pcWord=(unsigned int)flashAddr & (PAGESIZE-2); // Used when writing FLASH temp buffer   
  pageAdr=flashAddr & ~(PAGESIZE-1);        // Get FLASH page address from byte address   
  for(index=0; index < PAGESIZE; index+=2){   
    if(index==pcWord){   
      if(oddByte){   
        _SPM_FILLTEMP( index, (*(MyFlashCharPointer)(flashAddr & ~1) | ((unsigned int)data<<8)) );   
      }                                     // Write odd byte in temporary buffer   
      else{   
        _SPM_FILLTEMP( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );   
      }                                     // Write even byte in temporary buffer   
    }   
    else{   
      _SPM_FILLTEMP(index, *( (MyFlashIntPointer)(pageAdr+index) ) );   
    }                                       // Write Flash word directly to temporary buffer   
  }   
}   
