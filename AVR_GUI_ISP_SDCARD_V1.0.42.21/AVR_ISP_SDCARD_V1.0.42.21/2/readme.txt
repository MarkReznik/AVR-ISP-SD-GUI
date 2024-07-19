/*----------------------------------------------*/
/*      SDCARD folder name '1'  	*/
/*      1. hex file is BigClock.hex	*/
/*      2. project.txt file    		*/
/*      with next contents...   		*/
/*                              			*/
/*------------------------------		*/
/*      hexfile=BigClock.hex    		*/    //text
/*      chipname=Atmega8A       	*/    //text
/*      chipcode=0x76           		*/    //byte
/*      signature=1E9307h       		*/    //3 bytes
/*      progfuses=3F24A1FFh    	*/    //lock,low,high,ext   fuses
/*      normfuses=3C23A1FFh     	*/    //4 bytes
/*      maskfuses=3FFFFF00h     	*/    //4 bytes
/*      chipsize=00002000h      	*/    //long 8192 bytes
/*      pagesize=0040h          		*/    //int 64 words
/*      debug=01h               		*/    //byte
/*	ready=00h			*/	0=reset all parameters in eeprom 1=update eeprom if newer firmware
/*	maxprogs=00000020h	*/	maximum programming cycles
/*	fw_ver=1.0.0		*/	test 10 chars max
/*----------------------------------------------*/
