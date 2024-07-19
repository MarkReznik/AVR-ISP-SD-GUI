#ifndef AVR910DEVICES_H
#define AVR910DEVICES_H

//#include "integer.h"

// коды микроконтроллеров и поддержка поллинга и страничного режима записи
flash UCHAR DeviceDesc[] =
{
	0x13	,0xFF,	//AT90S1200
	0x20	,0x7F,	//AT90S2313A
	0x28	,0x7F,	//AT90S4414A
	0x30	,0xFF,	//AT90S4433A
	0x34	,0xFF,	//AT90S2333A
	0x38	,0x7F,	//AT90S8515A
	0x48	,0xFF,	//AT90S2323A
	0x4c	,0xFF,	//AT90S2343A
	0x51	,0xFF,	//tn10
	0x55	,0xFF,	//tn12
	0x56	,0xFF,	//tn15
	0x68	,0xFF,	//AT90S8535
	0x6c	,0xFF,	//AT90S4434
	0x86	,0xFF,	//AT89S8252  bug in avrprog in Block write Mode!(See Note 18 for Workaround)
	0x87	,0xFF,	//AT89S53    bug in avrprog

// Devices which support Page Programming. Dont forget the Page Size
// of different Devices.
// Maximum ist 0x40, due to limitated RAM in 2313 Chip.
// (Pages with bigger Pages are programmed multiple times)


// unofficial Device Codes. Match avrdude.conf with tis "avr910-devcode"



// avr910-devcode ,Pagesite in Words
	 0x01	,0x40,	//m640 		avr910-Devicecode not official!
//	 0x02	,0x40,	//m644 		avr910-Devicecode not official!
//	 0x03	,0x40,	//m645 		avr910-Devicecode not official!
	 0x04	,0x40,	//m649 		avr910-Devicecode not official!
	 0x05	,0x40,	//m6490 	avr910-Devicecode not official!
	 0x06	,0x40,	//90PWM2	avr910-Devicecode not official!
	 0x07	,0x40,	//90PWM3	avr910-Devicecode not official!
	 0x08	,0x40,	//m1280 	avr910-Devicecode not official!
	 0x09	,0x40,	//m1281		avr910-Devicecode not official!
//	 0x0a	,0x40,	//m2560 	avr910-Devicecode not official!
//	 0x0b	,0x40,	//m2561		avr910-Devicecode not official!
//	 0x0c	,0x40,	//m3250 	avr910-Devicecode not official!
//	 0x0d	,0x40,	//m6450		avr910-Devicecode not official!
	 0x0e	,0x10,	//tn24 		avr910-Devicecode not official!
	 0x1a	,0x10,	//tn25 		avr910-Devicecode not official!
	 0x0f	,0x20,	//tn44		avr910-Devicecode not official!  signature=1E9207h
	 0x1b	,0x20,	//tn45		avr910-Devicecode not official!
	 0x14	,0x20,	//tn84		avr910-Devicecode not official!
	 0x1c	,0x20,	//tn85		avr910-Devicecode not official!
	 0x1d	,0x40,	//CAN128	avr910-Devicecode not official!
	 0x23	,0x10,	//tn2313 	avr910-Devicecode not official! (STK500 Code used)
	 0x31	,0x20,	//m48		avr910-Devicecode not official!
	 0x33	,0x20,	//m88 		avr910-Devicecode not official!
	 0x35	,0x40,	//m168 		avr910-Devicecode not official!
//	 0x36	,0x40,	//m165 		avr910-Devicecode not official!
	 0x37	,0x40,	//m164 		avr910-Devicecode not official!
	 0x39	,0x40,	//m324 		avr910-Devicecode not official!
//	 0x3c	,0x40,	//m325 		avr910-Devicecode not official!
	 0x3d	,0x40,	//m329 		avr910-Devicecode not official!
	 0x3e	,0x40,	//m3290 	avr910-Devicecode not official!
	 0x57	,0x10,	//tn13 		avr910-Devicecode not official!

//Appearance of this Devicecodes does not mean this Devices are or will be fully supported !



// official Devicecodes as matched in AVRProg V1.40
	0x3a	,0x20,	//m8515, Pagesize 32 words (0x20,)
	0x41	,0x40,	//m103
	0x43	,0x80,	//m128   256 words
	0x45	,0x80,	//m64    128 words  64kb=10000h
	0x5e	,0x10,	//tn26
	0x60	,0x40,	//m161
	0x62	,0x40,	//m162
	0x64	,0x40,	//m163
	0x69	,0x20,	//m8535
	0x72	,0x40,	//m32    64 words
	0x74	,0x40,	//m16
	0x76	,0x20,	//m8
	0x78	,0x40,	//m169

    0x00,		//End of Table

// Devices with known avr910 Devicecodes, but not supported with this Programmer
//		0x42	,0x40,	//m603	obsolete
//		0x50			//tn11 Needs additional High Voltage Hardware and uses different Protocoll! No Support!
//		0x58			//tn19 Obsolete
//		0x5c			//tn28 Only supported in parallel Programming Mode!
//		0x65	,0x20,	//m83	obsolete
//		0x70			//AT90C8534  unknown Hardware, untested!
//		0x71			//AT90C8544  unknown Hardware, untested!
//		0x80			//AT89C1051  unknown Hardware, untested!
//		0x81			//AT89C2051  unknown Hardware, untested!

};

#endif//AVR910DEVICES_H
