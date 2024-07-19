#include "stdafx.h"
#include "avr910.h"
#include <string.h>


static UCHAR  ReleaseButton;
void getSignature(void);

/*****************************************************************************

******************************************************************************/
__C_task main(void)
{	
  //hardwareInit();
  AVR910_Init();
  SEI();

  // main event loop
  for(;;)
  {
      UCHAR *data;
      UCHAR bytesRead;     
      if((PINC&(1<<5))==0 && ReleaseButton){
        ReleaseButton=0;
        getSignature(); 
      }
      if(ReleaseButton==0)ReleaseButton=1;
  }
}

