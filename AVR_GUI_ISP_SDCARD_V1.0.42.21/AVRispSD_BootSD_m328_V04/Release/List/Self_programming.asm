
;CodeVisionAVR C Compiler V3.12 Advanced
;(C) Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Build configuration    : Release
;Chip type              : ATmega328P
;Program type           : Boot Loader
;Clock frequency        : 8.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 512 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: Yes
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#define _MODEL_SMALL_

	#pragma AVRPART ADMIN PART_NAME ATmega328P
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2048
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x08FF
	.EQU __DSTACK_SIZE=0x0200
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	PUSH R26
	PUSH R27
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	POP  R27
	POP  R26
	ICALL
	.ENDM

	.MACRO __CALL2EX
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	CALL __EEPROMRDD
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z
	MOVW R30,R0
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _token=R5
	.DEF _SectorsPerCluster=R4
	.DEF _RootEntryCnt=R6
	.DEF _RootEntryCnt_msb=R7
	.DEF _appPages=R8
	.DEF _appPages_msb=R9
	.DEF _pagesCnt=R10
	.DEF _pagesCnt_msb=R11
	.DEF _bytesChecksum=R12
	.DEF _bytesChecksum_msb=R13

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x3800

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800
	JMP  0x3800

_0x40000:
	.DB  0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20
	.DB  0x20,0x20,0x20,0x0,0x46,0x4C,0x41,0x53
	.DB  0x48,0x20,0x20,0x20,0x0,0x5B,0x73,0x65
	.DB  0x74,0x74,0x69,0x6E,0x67,0x73,0x5D,0x0

__GLOBAL_INI_TBL:
	.DW  0x0C
	.DW  _0x4003D
	.DW  _0x40000*2

	.DW  0x09
	.DW  _0x4003D+12
	.DW  _0x40000*2+12

	.DW  0x0B
	.DW  _0x4003D+21
	.DW  _0x40000*2+21

_0xFFFFFFFF:
	.DW  0

#define __GLOBAL_INI_TBL_PRESENT 1

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF THE BOOT LOADER
	LDI  R31,1
	OUT  MCUCR,R31
	LDI  R31,2
	OUT  MCUCR,R31

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x300

	.CSEG
;// This file has been prepared for Doxygen automatic documentation generation.
;/*! \file ********************************************************************
;*
;* Atmel Corporation
;*
;* - File              : Self_programming.c
;* - Compiler          : IAR EWAVR 2.28a / 3.10c and newer
;*
;* - Support mail      : avr@atmel.com
;*
;* - Supported devices : All devices with bootloaders support.
;*
;* - AppNote           : AVR106 - C functions for reading and writing
;*                       to flash memory.
;*
;* - Description       : The file contains functions for easy reading and writing
;*                       of Flash memory on parts having the "Self-programming"
;*                       feature. The user functions are as follows:
;*
;*                       ReadFlashByte()
;*                       ReadFlashPage()
;*                       WriteFlashByte()
;*                       WriteFlashPage()
;*                       RecoverFlash()
;*
;*                       The remaining functions contained in this file are used
;*                       by the functions listet above.
;*
;* $Revision: 2.0 $
;* $Date: Wednesday, January 18, 2006 15:18:52 UTC $
;*
;****************************************************************************/
;#include <io.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;//#include <inavr.h>
;#include "Self_programming.h"
;#include "flash.h"
     #define WR_SPMCR_REG_R22 out 0x37,r22
;#include <delay.h>
;
;
;//void (*__AddrToZ24ByteToSPMCR_SPM_W_Test)(void flash *addr)= (void(*)(void flash *)) 0x0F9E0;
;//void (*__AddrToZ24WordToR1R0ByteToSPMCR_SPM_F_Test)(void flash *addr, unsigned int data)= (void(*)(void flash *, unsig ...
;
;
;/*!
;* Declare global struct variable in EEPROM if Flash recovery enabled.
;* FlashBackup pageNumber holds Flash pageaddress (/PAGESIZE) the data in Flash
;* recovery buffer should be written to if data need to be recovered.
;* FlashBackup.status tells if data need to be recovered.
;**/
;#ifdef __FLASH_RECOVER
;__eeprom struct {
;  unsigned int  pageNumber;
;  unsigned char status;
;}FlashBackup = {0};
;#endif
;
;
;/*!
;* The function Returns one byte located on Flash address given by ucFlashStartAdr.
;**/
;unsigned char ReadFlashByte(MyAddressType flashStartAdr){
; 0000 003D unsigned char ReadFlashByte(MyAddressType flashStartAdr){

	.CSEG
; 0000 003E //#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
; 0000 003F   return (unsigned char)*((MyFlashCharPointer)flashStartAdr);
;	flashStartAdr -> Y+0
; 0000 0040 //#pragma diag_default=Pe1053 // Back to default.
; 0000 0041 } // Returns data from Flash
;
;/*!
;* The function reads one Flash page from address flashStartAdr and stores data
;* in array dataPage[]. The number of bytes stored is depending upon the
;* Flash page size. The function returns FALSE if input address is not a Flash
;* page address, else TRUE.
;**/
;unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage){
; 0000 0049 unsigned char ReadFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage){
; 0000 004A   unsigned int index;
; 0000 004B   if(!(flashStartAdr & (PAGESIZE-1))){      // If input address is a page address
;	flashStartAdr -> Y+4
;	*dataPage -> Y+2
;	index -> R16,R17
; 0000 004C     for(index = 0; index < PAGESIZE; index++){
; 0000 004D       dataPage[index] = ReadFlashByte(flashStartAdr + index);
; 0000 004E     }
; 0000 004F     return TRUE;                            // Return TRUE if valid page address
; 0000 0050   }
; 0000 0051   else{
; 0000 0052     return FALSE;                           // Return FALSE if not valid page address
; 0000 0053   }
; 0000 0054 }
;
;/*!
;* The function writes byte data to Flash address flashAddr. Returns FALSE if
;* input address is not valid Flash byte address for writing, else TRUE.
;**/
;unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data){
; 0000 005A unsigned char WriteFlashByte(MyAddressType flashAddr, unsigned char data){
; 0000 005B   MyAddressType  pageAdr;
; 0000 005C   unsigned char eepromInterruptSettings,sregSettings;
; 0000 005D   if( AddressCheck( flashAddr & ~(PAGESIZE-1) )){
;	flashAddr -> Y+5
;	data -> Y+4
;	pageAdr -> R16,R17
;	eepromInterruptSettings -> R19
;	sregSettings -> R18
; 0000 005E 
; 0000 005F     eepromInterruptSettings= EECR & (1<<EERIE); // Stores EEPROM interrupt mask
; 0000 0060     EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt
; 0000 0061     _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));                // Wait if ongoing EEPROM write
; 0000 0062 
; 0000 0063     sregSettings= SREG;
; 0000 0064     #asm("cli");
; 0000 0065     pageAdr=flashAddr & ~(PAGESIZE-1);      // Gets Flash page address from byte address
; 0000 0066 
; 0000 0067     #ifdef __FLASH_RECOVER
; 0000 0068     FlashBackup.status=0;                   // Inicate that Flash buffer does
; 0000 0069                                             // not contain data for writing
; 0000 006A     _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
; 0000 006B     LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
; 0000 006C     WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
; 0000 006D     FlashBackup.pageNumber = (unsigned int) (pageAdr/PAGESIZE); // Stores page address
; 0000 006E                                                        // data should be written to
; 0000 006F     FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer
; 0000 0070                                                // contains unwritten data
; 0000 0071     _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
; 0000 0072     #endif
; 0000 0073 
; 0000 0074     LpmReplaceSpm(flashAddr, data);         // Fills Flash write buffer
; 0000 0075 
; 0000 0076 
; 0000 0077     WriteBufToFlash(pageAdr);               // Writes to Flash
; 0000 0078 
; 0000 0079     #ifdef __FLASH_RECOVER
; 0000 007A     FlashBackup.status = 0;                 // Indicates that Flash recovery buffer
; 0000 007B                                             // does not contain unwritten data
; 0000 007C     _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));
; 0000 007D     #endif
; 0000 007E 
; 0000 007F     EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
; 0000 0080     SREG = sregSettings;
; 0000 0081     return TRUE;                            // Return TRUE if address
; 0000 0082                                             // valid for writing
; 0000 0083   }
; 0000 0084   else
; 0000 0085     return FALSE;                           // Return FALSE if address not
; 0000 0086                                             // valid for writing
; 0000 0087 }
;
;/*!
;* The function writes data from array dataPage[] to Flash page address
;* flashStartAdr. The Number of bytes written is depending upon the Flash page
;* size. Returns FALSE if input argument ucFlashStartAdr is not a valid Flash
;* page address for writing, else TRUE.
;**/
;unsigned char WriteFlashPage(MyAddressType flashStartAdr, unsigned char *dataPage)
; 0000 0090 {
_WriteFlashPage:
; .FSTART _WriteFlashPage
; 0000 0091   unsigned int index=0;
; 0000 0092   unsigned char eepromInterruptSettings,sregSettings;
; 0000 0093   if( AddressCheck(flashStartAdr) ){
	ST   -Y,R27
	ST   -Y,R26
	CALL __SAVELOCR4
;	flashStartAdr -> Y+6
;	*dataPage -> Y+4
;	index -> R16,R17
;	eepromInterruptSettings -> R19
;	sregSettings -> R18
	__GETWRN 16,17,0
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RCALL _AddressCheck
	CPI  R30,0
	BRNE PC+2
	RJMP _0xD
; 0000 0094     eepromInterruptSettings = EECR & (1<<EERIE); // Stoes EEPROM interrupt mask
	IN   R30,0x1F
	ANDI R30,LOW(0x8)
	MOV  R19,R30
; 0000 0095     EECR &= ~(1<<EERIE);                    // Disable EEPROM interrupt
	CBI  0x1F,3
; 0000 0096     _WAIT_FOR_EEWE();//while(EECR & (1<<EEWE));                // Wait if ongoing EEPROM write
_0xE:
	SBIC 0x1F,1
	RJMP _0xE
; 0000 0097 
; 0000 0098     sregSettings= SREG;
	IN   R18,63
; 0000 0099     #asm("cli");
	cli
; 0000 009A 
; 0000 009B     #ifdef __FLASH_RECOVER
; 0000 009C     FlashBackup.status=0;                   // Inicate that Flash buffer does
; 0000 009D                                             // not contain data for writing
; 0000 009E     while(EECR & (1<<EEWE));
; 0000 009F 
; 0000 00A0     //_ENABLE_RWW_SECTION();
; 0000 00A1 
; 0000 00A2     _WAIT_FOR_SPM();
; 0000 00A3     //_PAGE_ERASE( flashStartAdr );
; 0000 00A4 
; 0000 00A5     for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer
; 0000 00A6       //_WAIT_FOR_SPM();
; 0000 00A7       //MY_FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
; 0000 00A8       _FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
; 0000 00A9     }
; 0000 00AA 
; 0000 00AB     WriteBufToFlash(ADR_FLASH_BUFFER);      // Writes to Flash recovery buffer
; 0000 00AC     FlashBackup.pageNumber=(unsigned int)(flashStartAdr/PAGESIZE);
; 0000 00AD     FlashBackup.status = FLASH_BUFFER_FULL_ID; // Indicates that Flash recovery buffer
; 0000 00AE                                            // contains unwritten data
; 0000 00AF     _WAIT_FOR_EEWE();//vwhile(EECR & (1<<EEWE));
; 0000 00B0     #endif
; 0000 00B1 
; 0000 00B2     if(index==0xFFF1)
	LDI  R30,LOW(65521)
	LDI  R31,HIGH(65521)
	CP   R30,R16
	CPC  R31,R17
	BRNE _0x11
; 0000 00B3     {
; 0000 00B4         #ifdef _ATMEGA128
; 0000 00B5         __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F(0,0);
; 0000 00B6         __AddrToZ24ByteToSPMCR_SPM_EW((void flash *)0x1EF00);
; 0000 00B7         #endif
; 0000 00B8         #ifdef _ATMEGA328
; 0000 00B9         __AddrToZWordToR1R0ByteToSPMCR_SPM_F(0,0);
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(0)
	LDI  R27,0
	CALL ___AddrToZWordToR1R0ByteToSPMCR_SPM_F
; 0000 00BA         __AddrToZByteToSPMCR_SPM_EW((void flash *)0x1EF00);
	LDI  R26,LOW(61184)
	LDI  R27,HIGH(61184)
	CALL ___AddrToZByteToSPMCR_SPM_EW
; 0000 00BB         #endif
; 0000 00BC     }
; 0000 00BD     _WAIT_FOR_SPM();
_0x11:
_0x12:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x12
; 0000 00BE     _PAGE_ERASE( flashStartAdr );
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	CALL ___AddrToZByteToSPMCR_SPM_E
; 0000 00BF     //_ENABLE_RWW_SECTION();
; 0000 00C0 
; 0000 00C1     for(index = 0; index < PAGESIZE; index+=2){ // Fills Flash write buffer
	__GETWRN 16,17,0
_0x16:
	__CPWRN 16,17,128
	BRSH _0x17
; 0000 00C2       //_WAIT_FOR_SPM();
; 0000 00C3       //MY_FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
; 0000 00C4       _FILL_TEMP_WORD(index, (unsigned int)dataPage[index]+((unsigned int)dataPage[index+1] << 8));
	ST   -Y,R17
	ST   -Y,R16
	MOVW R30,R16
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADD  R26,R30
	ADC  R27,R31
	LD   R0,X
	CLR  R1
	ADIW R30,1
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADD  R26,R30
	ADC  R27,R31
	LD   R30,X
	MOV  R31,R30
	LDI  R30,0
	MOVW R26,R0
	ADD  R26,R30
	ADC  R27,R31
	CALL ___AddrToZWordToR1R0ByteToSPMCR_SPM_F
; 0000 00C5     }
	__ADDWRN 16,17,2
	RJMP _0x16
_0x17:
; 0000 00C6     //_PAGE_WRITE( flashStartAdr );
; 0000 00C7     WriteBufToFlash(flashStartAdr);         // Writes to Flash
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	RCALL _WriteBufToFlash
; 0000 00C8     #ifdef __FLASH_RECOVER
; 0000 00C9       FlashBackup.status=0;                 // Inicate that Flash buffer does
; 0000 00CA                                             // not contain data for writing
; 0000 00CB       while(EECR & (1<<EEWE));
; 0000 00CC     #endif
; 0000 00CD 
; 0000 00CE     EECR |= eepromInterruptSettings;        // Restore EEPROM interrupt mask
	IN   R30,0x1F
	OR   R30,R19
	OUT  0x1F,R30
; 0000 00CF     SREG = sregSettings;
	OUT  0x3F,R18
; 0000 00D0     return TRUE;                            // Return TRUE if address
	LDI  R30,LOW(1)
	RJMP _0x200000B
; 0000 00D1                                             // valid for writing
; 0000 00D2   }
; 0000 00D3   else
_0xD:
; 0000 00D4     return FALSE;                           // Return FALSE if not address not
	LDI  R30,LOW(0)
; 0000 00D5                                             // valid for writing
; 0000 00D6 }
_0x200000B:
	CALL __LOADLOCR4
	ADIW R28,8
	RET
; .FEND
;
;/*!
;* The function checks if global variable FlashBackup.status indicates that Flash recovery
;* buffer contains data that needs to be written to Flash. Writes data from
;* Flash recovery buffer to Flash page address given by FLASH_recovery.pageAdr.
;* This function should be called at program startup if FLASH recovery option
;* is enabeled.
;**/
;unsigned char RecoverFlash(){
; 0000 00DF unsigned char RecoverFlash(){
; 0000 00E0 #ifdef __FLASH_RECOVER
; 0000 00E1   unsigned int index;
; 0000 00E2   unsigned long flashStartAdr = (MyAddressType)FlashBackup.pageNumber * PAGESIZE;
; 0000 00E3   if(FlashBackup.status == FLASH_BUFFER_FULL_ID){ // Checks if Flash recovery
; 0000 00E4                                                   //  buffer contains data
; 0000 00E5 
; 0000 00E6     for(index=0; index < PAGESIZE; index+=2){     // Writes to Flash write buffer
; 0000 00E7         _WAIT_FOR_SPM();
; 0000 00E8         MY_FILL_TEMP_WORD( index, *((MyFlashIntPointer)(ADR_FLASH_BUFFER+index)) );
; 0000 00E9     }
; 0000 00EA 
; 0000 00EB 
; 0000 00EC     //WriteBufToFlash((MyAddressType)FlashBackup.pageNumber * PAGESIZE);
; 0000 00ED     _WAIT_FOR_SPM();
; 0000 00EE     MY_PAGE_ERASE( flashStartAdr );
; 0000 00EF     _WAIT_FOR_SPM();
; 0000 00F0     MY_PAGE_WRITE( flashStartAdr );
; 0000 00F1     _WAIT_FOR_SPM();
; 0000 00F2     _ENABLE_RWW_SECTION();
; 0000 00F3     FlashBackup.status=0;                   // Inicate that Flash buffer does
; 0000 00F4                                             // not contain data for writing
; 0000 00F5     while(EECR & (1<<EEWE));
; 0000 00F6     return TRUE;                            // Returns TRUE if recovery has
; 0000 00F7                                             // taken place
; 0000 00F8   }
; 0000 00F9 #endif
; 0000 00FA   return FALSE;
; 0000 00FB }
;
;
;/*!
;* The function checks if input argument is a valid Flash page address for
;* writing. Returns TRUE only if:
;* - Address points to the beginning of a Flash page
;* - Address is within the limits defined in Self_programming.h
;* - Address is not equal to page address used for buffring by the Flash recovery
;*   functions (if enabled).
;* Returns FALSE else.
;**/
;unsigned char AddressCheck(MyAddressType flashAdr){
; 0000 0107 unsigned char AddressCheck(MyAddressType flashAdr){
_AddressCheck:
; .FSTART _AddressCheck
; 0000 0108   #ifdef __FLASH_RECOVER
; 0000 0109   // The next line gives a warning 'pointless comparison with zero' if ADR_LIMIT_LOW is 0. Ignore it.
; 0000 010A   if( (flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) &&
; 0000 010B       (flashAdr != ADR_FLASH_BUFFER) && !(flashAdr & (PAGESIZE-1)) )
; 0000 010C     return TRUE;                            // Address is a valid page address
; 0000 010D   else
; 0000 010E     return FALSE;                           // Address is not a valid page address
; 0000 010F   #else
; 0000 0110   if((flashAdr >= ADR_LIMIT_LOW) && (flashAdr <= ADR_LIMIT_HIGH) && !(flashAdr & (PAGESIZE-1) ) )
	CALL SUBOPT_0x0
;	flashAdr -> Y+0
	CLR  R24
	CLR  R25
	TST  R25
	BRMI _0x1A
	LD   R26,Y
	LDD  R27,Y+1
	CPI  R26,LOW(0x7000)
	LDI  R30,HIGH(0x7000)
	CPC  R27,R30
	BRSH _0x1A
	LD   R30,Y
	LDD  R31,Y+1
	ANDI R30,0x7F
	CPI  R30,0
	BREQ _0x1B
_0x1A:
	RJMP _0x19
_0x1B:
; 0000 0111     return TRUE;                            // Address is a valid page address
	LDI  R30,LOW(1)
	JMP  _0x2000007
; 0000 0112   else
_0x19:
; 0000 0113   {
; 0000 0114     /*
; 0000 0115     while(1)
; 0000 0116     {
; 0000 0117       PORTC.5=0;
; 0000 0118       delay_ms(500);
; 0000 0119       PORTC.5=1;
; 0000 011A       delay_ms(500);
; 0000 011B     }
; 0000 011C     */
; 0000 011D     return FALSE;                           // Address is not a valid page address
	LDI  R30,LOW(0)
	JMP  _0x2000007
; 0000 011E   }
; 0000 011F   #endif
; 0000 0120 }
; .FEND
;
;
;/*!
;* The function writes Flash temporary buffer to Flash page address given by
;* input argument.
;**/
;
;void WriteBufToFlash(MyAddressType flashStartAdr) {
; 0000 0128 void WriteBufToFlash(MyAddressType flashStartAdr) {
_WriteBufToFlash:
; .FSTART _WriteBufToFlash
; 0000 0129     //_WAIT_FOR_SPM();
; 0000 012A     //MY_PAGE_ERASE( flashStartAdr );
; 0000 012B     //_PAGE_ERASE( flashStartAdr );
; 0000 012C     //_WAIT_FOR_SPM();
; 0000 012D     //_ENABLE_RWW_SECTION();
; 0000 012E     //MY_PAGE_WRITE( flashStartAdr );
; 0000 012F     //_PAGE_ERASE( flashStartAdr );
; 0000 0130     _PAGE_WRITE( flashStartAdr );
	CALL SUBOPT_0x0
;	flashStartAdr -> Y+0
	CALL ___AddrToZByteToSPMCR_SPM_W
; 0000 0131     //_WAIT_FOR_SPM();
; 0000 0132     //_ENABLE_RWW_SECTION();
; 0000 0133 /*
; 0000 0134 #pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
; 0000 0135   #ifdef __HAS_RAMPZ__
; 0000 0136   RAMPZ = (unsigned char)(flashStartAdr >> 16);
; 0000 0137   #endif
; 0000 0138   _PAGE_ERASE(flashStartAdr);
; 0000 0139   while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
; 0000 013A   _PAGE_WRITE(flashStartAdr);
; 0000 013B   while( SPMControllRegister & (1<<SPMEN) ); // Wait until Flash write completed
; 0000 013C   #ifdef RWWSRE
; 0000 013D   __DataToR0ByteToSPMCR_SPM( 0, (unsigned char)(1<<RWWSRE)|(1<<SPMEN)); // Enable RWW
; 0000 013E   #endif
; 0000 013F #pragma diag_default=Pe1053 // Back to default.
; 0000 0140 */
; 0000 0141 }
	JMP  _0x2000007
; .FEND
;
;/*!
;* The function reads Flash page given by flashAddr, replaces one byte given by
;* flashAddr with data, and stores entire page in Flash temporary buffer.
;**/
;void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data){
; 0000 0147 void LpmReplaceSpm(MyAddressType flashAddr, unsigned char data){
; 0000 0148 //#pragma diag_suppress=Pe1053 // Suppress warning for conversion from long-type address to flash ptr.
; 0000 0149     unsigned int index, oddByte, pcWord;
; 0000 014A 
; 0000 014B     MyAddressType  pageAdr;
; 0000 014C     oddByte=(unsigned char)flashAddr & 1;
;	flashAddr -> Y+9
;	data -> Y+8
;	index -> R16,R17
;	oddByte -> R18,R19
;	pcWord -> R20,R21
;	pageAdr -> Y+6
; 0000 014D     pcWord=(unsigned int)flashAddr & (PAGESIZE-2); // Used when writing FLASH temp buffer
; 0000 014E     pageAdr=flashAddr & ~(PAGESIZE-1);        // Get FLASH page address from byte address
; 0000 014F     //while( SPMCR_REG & (1<<SPMEN) );
; 0000 0150     //_ENABLE_RWW_SECTION();
; 0000 0151 
; 0000 0152     for(index=0; index < PAGESIZE; index+=2){
; 0000 0153         if(index==pcWord){
; 0000 0154           if(oddByte){
; 0000 0155             //MY_FILL_TEMP_WORD( index, (*(MyFlashCharPointer)(flashAddr & ~1) | ((unsigned int)data<<8)) );
; 0000 0156             _FILL_TEMP_WORD( index, (*(MyFlashCharPointer)(flashAddr & ~1) | ((unsigned int)data<<8)) );
; 0000 0157           }                                     // Write odd byte in temporary buffer
; 0000 0158           else{
; 0000 0159             //MY_FILL_TEMP_WORD( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );
; 0000 015A             _FILL_TEMP_WORD( index, ( (*( (MyFlashCharPointer)flashAddr+1)<<8)  | data ) );
; 0000 015B           }                                     // Write even byte in temporary buffer
; 0000 015C         }
; 0000 015D         else{
; 0000 015E           //MY_FILL_TEMP_WORD(index, *( (MyFlashIntPointer)(pageAdr+index) ) );
; 0000 015F           _FILL_TEMP_WORD(index, *( (MyFlashIntPointer)(pageAdr+index) ) );
; 0000 0160         }                                       // Write Flash word directly to temporary buffer
; 0000 0161     }
; 0000 0162 //#pragma diag_default=Pe1053 // Back to default.
; 0000 0163 }
;/*****************************************************************************
;*
;* (C) 2010, HP InfoTech srl, www.hpinfotech.com
;*
;* File              : flash.c
;* Compiler          : CodeVisionAVR V2.xx
;* Revision          : $Revision: 1.0 $
;* Date              : $Date: December 9, 2010 $
;* Updated by        : $Author: HP InfoTech $
;*
;* Target platform   : All AVRs with bootloader support
;*
;* AppNote           : AVR109 - Self-programming
;*
;* Description       : Flash operations for AVR109 Self-programming
;****************************************************************************/
;
;#include <io.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include "flash.h"
     #define WR_SPMCR_REG_R22 out 0x37,r22
;
;//#define	SPMCR_REG	SPMCSR
;
;void dospmew(void)
; 0001 0018 {

	.CSEG
; 0001 0019     #asm
; 0001 001A          ldi   r22,$03
; 0001 001B          WR_SPMCR_REG_R22
; 0001 001C          spm
; 0001 001D     #endasm
; 0001 001E     _WAIT_FOR_SPM();
; 0001 001F     #asm
; 0001 0020         ldi   r22,$11
; 0001 0021         WR_SPMCR_REG_R22
; 0001 0022         spm
; 0001 0023     #endasm
; 0001 0024     _WAIT_FOR_SPM();
; 0001 0025     #asm
; 0001 0026         ldi   r22,$05
; 0001 0027         WR_SPMCR_REG_R22
; 0001 0028         spm
; 0001 0029     #endasm
; 0001 002A     _WAIT_FOR_SPM();
; 0001 002B     do
; 0001 002C     {
; 0001 002D     #asm
; 0001 002E         ldi   r22,$11
; 0001 002F         WR_SPMCR_REG_R22
; 0001 0030         spm
; 0001 0031     #endasm
; 0001 0032         _WAIT_FOR_SPM();
; 0001 0033     }
; 0001 0034     while( SPMCR_REG & (1<<RWWSB) );
; 0001 0035 }
;
;void dospmw(void)
; 0001 0038 {
; 0001 0039     #asm
; 0001 003A         ldi   r22,$05
; 0001 003B         WR_SPMCR_REG_R22
; 0001 003C         spm
; 0001 003D     #endasm
; 0001 003E     _WAIT_FOR_SPM();
; 0001 003F     do
; 0001 0040     {
; 0001 0041     #asm
; 0001 0042         ldi   r22,$11
; 0001 0043         WR_SPMCR_REG_R22
; 0001 0044         spm
; 0001 0045     #endasm
; 0001 0046         _WAIT_FOR_SPM();
; 0001 0047     }
; 0001 0048     while( SPMCR_REG & (1<<RWWSB) );
; 0001 0049 }
;
;void dospme(void)
; 0001 004C {
; 0001 004D     #asm
; 0001 004E         ldi   r22,$03
; 0001 004F         WR_SPMCR_REG_R22
; 0001 0050         spm
; 0001 0051     #endasm
; 0001 0052     _WAIT_FOR_SPM();
; 0001 0053     //while( SPMCR_REG & (1<<RWWSB) )
; 0001 0054     {
; 0001 0055     #asm
; 0001 0056         ldi   r22,$11
; 0001 0057         WR_SPMCR_REG_R22
; 0001 0058         spm
; 0001 0059     #endasm
; 0001 005A         _WAIT_FOR_SPM();
; 0001 005B     }
; 0001 005C }
;
;void dospm(void)
; 0001 005F {
_dospm:
; .FSTART _dospm
; 0001 0060     #asm
; 0001 0061          ldi   r22,$01
         ldi   r22,$01
; 0001 0062          WR_SPMCR_REG_R22
         WR_SPMCR_REG_R22
; 0001 0063          spm
         spm
; 0001 0064     #endasm
; 0001 0065     _WAIT_FOR_SPM();
_0x20021:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20021
; 0001 0066     while( SPMCR_REG & (1<<RWWSB) )
_0x20024:
	IN   R30,0x37
	SBRS R30,6
	RJMP _0x20026
; 0001 0067     {
; 0001 0068     #asm
; 0001 0069         ldi   r22,$11
        ldi   r22,$11
; 0001 006A         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 006B         spm
        spm
; 0001 006C     #endasm
; 0001 006D         _WAIT_FOR_SPM();
_0x20027:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20027
; 0001 006E     }
	RJMP _0x20024
_0x20026:
; 0001 006F }
	RET
; .FEND
;
;#pragma warn-
;
;unsigned char __AddrToZByteToSPMCR_LPM(void flash *addr, unsigned char ctrl)
; 0001 0074 {
; 0001 0075 #asm
;	*addr -> Y+1
;	ctrl -> Y+0
; 0001 0076      ldd  r30,y+1
; 0001 0077      ldd  r31,y+2
; 0001 0078      ld   r22,y
; 0001 0079      WR_SPMCR_REG_R22
; 0001 007A      lpm
; 0001 007B      mov  r30,r0
; 0001 007C #endasm
; 0001 007D }
;
;void __DataToR0ByteToSPMCR_SPM(unsigned char data, unsigned char ctrl)
; 0001 0080 {
; 0001 0081 #asm
;	data -> Y+1
;	ctrl -> Y+0
; 0001 0082      ldd  r0,y+1
; 0001 0083      ld   r22,y
; 0001 0084      WR_SPMCR_REG_R22
; 0001 0085      spm
; 0001 0086 #endasm
; 0001 0087 }
;
;void __AddrToZWordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl)
; 0001 008A {
; 0001 008B     #asm
;	*addr -> Y+3
;	data -> Y+1
;	ctrl -> Y+0
; 0001 008C          ldd  r30,y+3
; 0001 008D          ldd  r31,y+4
; 0001 008E          ldd  r0,y+1
; 0001 008F          ldd  r1,y+2
; 0001 0090          ld   r22,y
; 0001 0091          WR_SPMCR_REG_R22
; 0001 0092          spm
; 0001 0093     #endasm
; 0001 0094 }
;
;void __AddrToZWordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data)
; 0001 0097 {
___AddrToZWordToR1R0ByteToSPMCR_SPM_F:
; .FSTART ___AddrToZWordToR1R0ByteToSPMCR_SPM_F
; 0001 0098 _WAIT_FOR_SPM();
	ST   -Y,R27
	ST   -Y,R26
;	*addr -> Y+2
;	data -> Y+0
_0x2002A:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x2002A
; 0001 0099     #asm
; 0001 009A          ldd  r30,y+2
         ldd  r30,y+2
; 0001 009B          ldd  r31,y+3
         ldd  r31,y+3
; 0001 009C          ldd  r0,y+0
         ldd  r0,y+0
; 0001 009D          ldd  r1,y+1
         ldd  r1,y+1
; 0001 009E          //ldi   r22,LOW(1)
         //ldi   r22,LOW(1)
; 0001 009F          //WR_SPMCR_REG_R22
         //WR_SPMCR_REG_R22
; 0001 00A0          //spm
         //spm
; 0001 00A1     #endasm
; 0001 00A2 dospm();
	CALL _dospm
; 0001 00A3 }
	JMP  _0x2000008
; .FEND
;
;void __AddrToZByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
; 0001 00A6 {
; 0001 00A7 #asm
;	*addr -> Y+1
;	ctrl -> Y+0
; 0001 00A8      ldd  r30,y+1
; 0001 00A9      ldd  r31,y+2
; 0001 00AA      ld   r22,y
; 0001 00AB      WR_SPMCR_REG_R22
; 0001 00AC      spm
; 0001 00AD #endasm
; 0001 00AE }
;
;void __AddrToZByteToSPMCR_SPM_E(void flash *addr)
; 0001 00B1 {
___AddrToZByteToSPMCR_SPM_E:
; .FSTART ___AddrToZByteToSPMCR_SPM_E
; 0001 00B2     _WAIT_FOR_SPM();
	ST   -Y,R27
	ST   -Y,R26
;	*addr -> Y+0
_0x2002D:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x2002D
; 0001 00B3     #asm
; 0001 00B4          ldd  r30,y+0
         ldd  r30,y+0
; 0001 00B5          ldd  r31,y+1
         ldd  r31,y+1
; 0001 00B6          //ld   r22,y
         //ld   r22,y
; 0001 00B7          //WR_SPMCR_REG_R22
         //WR_SPMCR_REG_R22
; 0001 00B8          //spm
         //spm
; 0001 00B9     #endasm
; 0001 00BA     #asm
; 0001 00BB         ldi   r22,$03
        ldi   r22,$03
; 0001 00BC         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00BD         spm
        spm
; 0001 00BE     #endasm
; 0001 00BF     _WAIT_FOR_SPM();
_0x20030:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20030
; 0001 00C0     //while( SPMCR_REG & (1<<RWWSB) )
; 0001 00C1     {
; 0001 00C2     #asm
; 0001 00C3         ldi   r22,$11
        ldi   r22,$11
; 0001 00C4         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00C5         spm
        spm
; 0001 00C6     #endasm
; 0001 00C7         _WAIT_FOR_SPM();
_0x20033:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20033
; 0001 00C8     }
; 0001 00C9 //dospme();
; 0001 00CA }
	JMP  _0x2000007
; .FEND
;
;void __AddrToZByteToSPMCR_SPM_W(void flash *addr)
; 0001 00CD {
___AddrToZByteToSPMCR_SPM_W:
; .FSTART ___AddrToZByteToSPMCR_SPM_W
; 0001 00CE     _WAIT_FOR_SPM();
	ST   -Y,R27
	ST   -Y,R26
;	*addr -> Y+0
_0x20036:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20036
; 0001 00CF     #asm
; 0001 00D0          ldd  r30,y+0
         ldd  r30,y+0
; 0001 00D1          ldd  r31,y+1
         ldd  r31,y+1
; 0001 00D2          //ld   r22,y
         //ld   r22,y
; 0001 00D3          //WR_SPMCR_REG_R22
         //WR_SPMCR_REG_R22
; 0001 00D4          //spm
         //spm
; 0001 00D5     #endasm
; 0001 00D6     #asm
; 0001 00D7         ldi   r22,$05
        ldi   r22,$05
; 0001 00D8         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00D9         spm
        spm
; 0001 00DA     #endasm
; 0001 00DB     _WAIT_FOR_SPM();
_0x20039:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20039
; 0001 00DC     do
_0x2003D:
; 0001 00DD     {
; 0001 00DE     #asm
; 0001 00DF         ldi   r22,$11
        ldi   r22,$11
; 0001 00E0         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00E1         spm
        spm
; 0001 00E2     #endasm
; 0001 00E3         _WAIT_FOR_SPM();
_0x2003F:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x2003F
; 0001 00E4     }
; 0001 00E5     while( SPMCR_REG & (1<<RWWSB) );
	IN   R30,0x37
	SBRC R30,6
	RJMP _0x2003D
; 0001 00E6 //dospmw();
; 0001 00E7 }
	JMP  _0x2000007
; .FEND
;
;void __AddrToZByteToSPMCR_SPM_EW(void flash *addr)
; 0001 00EA {
___AddrToZByteToSPMCR_SPM_EW:
; .FSTART ___AddrToZByteToSPMCR_SPM_EW
; 0001 00EB     _WAIT_FOR_SPM();
	ST   -Y,R27
	ST   -Y,R26
;	*addr -> Y+0
_0x20042:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20042
; 0001 00EC     #asm
; 0001 00ED          ldd  r30,y+0
         ldd  r30,y+0
; 0001 00EE          ldd  r31,y+1
         ldd  r31,y+1
; 0001 00EF          //ld   r22,y
         //ld   r22,y
; 0001 00F0          //WR_SPMCR_REG_R22
         //WR_SPMCR_REG_R22
; 0001 00F1          //spm
         //spm
; 0001 00F2     #endasm
; 0001 00F3     #asm
; 0001 00F4         ldi   r22,$03
        ldi   r22,$03
; 0001 00F5         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00F6         spm
        spm
; 0001 00F7     #endasm
; 0001 00F8     _WAIT_FOR_SPM();
_0x20045:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20045
; 0001 00F9     //while( SPMCR_REG & (1<<RWWSB) )
; 0001 00FA     {
; 0001 00FB     #asm
; 0001 00FC         ldi   r22,$11
        ldi   r22,$11
; 0001 00FD         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 00FE         spm
        spm
; 0001 00FF     #endasm
; 0001 0100         _WAIT_FOR_SPM();
_0x20048:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20048
; 0001 0101     }
; 0001 0102 //dospme();
; 0001 0103     #asm
; 0001 0104          ldd  r30,y+0
         ldd  r30,y+0
; 0001 0105          ldd  r31,y+1
         ldd  r31,y+1
; 0001 0106          //ld   r22,y
         //ld   r22,y
; 0001 0107          //WR_SPMCR_REG_R22
         //WR_SPMCR_REG_R22
; 0001 0108          //spm
         //spm
; 0001 0109     #endasm
; 0001 010A     #asm
; 0001 010B         ldi   r22,$05
        ldi   r22,$05
; 0001 010C         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 010D         spm
        spm
; 0001 010E     #endasm
; 0001 010F     _WAIT_FOR_SPM();
_0x2004B:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x2004B
; 0001 0110     do
_0x2004F:
; 0001 0111     {
; 0001 0112     #asm
; 0001 0113         ldi   r22,$11
        ldi   r22,$11
; 0001 0114         WR_SPMCR_REG_R22
        WR_SPMCR_REG_R22
; 0001 0115         spm
        spm
; 0001 0116     #endasm
; 0001 0117         _WAIT_FOR_SPM();
_0x20051:
	IN   R30,0x37
	SBRC R30,0
	RJMP _0x20051
; 0001 0118     }
; 0001 0119     while( SPMCR_REG & (1<<RWWSB) );
	IN   R30,0x37
	SBRC R30,6
	RJMP _0x2004F
; 0001 011A //dospmw();
; 0001 011B //dospmew();
; 0001 011C }
	JMP  _0x2000007
; .FEND
;
;
;#ifdef _ATMEGA128
;void __AddrToZ24WordToR1R0ByteToSPMCR_SPM(void flash *addr, unsigned int data, unsigned char ctrl)
;{
;#asm
;     ldd  r30,y+3
;     ldd  r31,y+4
;     ldd  r22,y+5
;     out  rampz,r22
;     ldd  r0,y+1
;     ldd  r1,y+2
;     ld   r22,y
;     WR_SPMCR_REG_R22
;     spm
;#endasm
;}
;
;void __AddrToZ24WordToR1R0ByteToSPMCR_SPM_F(void flash *addr, unsigned int data)
;{
;_WAIT_FOR_SPM();
;#asm
;     ldd  r30,y+2
;     ldd  r31,y+3
;     ldd  r22,y+4
;     out  rampz,r22
;     ldd  r0,y+0
;     ldd  r1,y+1
;     //ld   r22,y
;     //WR_SPMCR_REG_R22
;     //spm
;#endasm
;dospm();
;}
;
;void __AddrToZ24ByteToSPMCR_SPM(void flash *addr, unsigned char ctrl)
;{
;#asm
;     ldd  r30,y+1
;     ldd  r31,y+2
;     ldd  r22,y+3
;     out  rampz,r22
;     ld   r22,y
;     WR_SPMCR_REG_R22
;     spm
;#endasm
;}
;
;void __AddrToZ24ByteToSPMCR_SPM_W(void flash *addr)
;{
;_WAIT_FOR_SPM();
;#asm
;     ldd  r30,y+0
;     ldd  r31,y+1
;     ldd  r22,y+2
;     out  rampz,r22
;     //ld   r22,y
;     //WR_SPMCR_REG_R22
;     //spm
;#endasm
;dospmw();
;}
;
;void __AddrToZ24ByteToSPMCR_SPM_E(void flash *addr)
;{
;_WAIT_FOR_SPM();
;#asm
;     ldd  r30,y+0
;     ldd  r31,y+1
;     ldd  r22,y+2
;     out  rampz,r22
;     //ld   r22,y
;     //WR_SPMCR_REG_R22
;     //spm
;#endasm
;dospme();
;}
;
;void __AddrToZ24ByteToSPMCR_SPM_EW(void flash *addr)
;{
;_WAIT_FOR_SPM();
;#asm
;     ldd  r30,y+0
;     ldd  r31,y+1
;     ldd  r22,y+2
;     out  rampz,r22
;     //ld   r22,y
;     //WR_SPMCR_REG_R22
;     //spm
;#endasm
;dospmew();
;}
;#endif
;
;#ifdef _WARNINGS_ON_
;#pragma warn+
;#endif
;
;/*
;
;Bootloader from SD card for AVR ISD SD project
;Version history:
;0.1: Initial
;0.2: Add Support only FAT32
;0.3: Add Support FAT16 DOS
;0.4: Removed debug points
;
;*/
;#include <io.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include <delay.h>
;#include "flash.h"
     #define WR_SPMCR_REG_R22 out 0x37,r22
;#include "Self_programming.h"
;
;#include "spi_sdcard.h"
;
;#define SDBUF_SIZE  512
;#define PAGES_PER_SDBUF (SDBUF_SIZE/PAGESIZE)
;
;
;
;unsigned char result[5], sdBuf[SDBUF_SIZE], testBuf[PAGESIZE], token, SectorsPerCluster;
;unsigned long appStartAdr,adr,SectorsPerFat,fat_begin_lba;
;unsigned long cluster_begin_lba,fat_file_adr,fat_file_next_adr,filesize,readbytes;
;unsigned int RootEntryCnt, appPages, pagesCnt, bytesChecksum, checksumCnt;
;//(unsigned long)fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
;//(unsigned long)cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FA ...
;//(unsigned char)sectors_per_cluster = BPB_SecPerClus;
;//(unsigned long)root_dir_first_cluster = BPB_RootClus;
;//void testWrite();
;void errorSD(unsigned char error_step);
;unsigned long buf2num(unsigned char *buf,unsigned char len);
;unsigned char compbuf(const unsigned char *src,unsigned char *dest);
;void (*app_pointer)(void) = (void(*)(void))0x0000;
;
;enum partition_types{DOS=0,FAT16,FAT32} partition_type;
;
;void main( void ){
; 0002 0027 void main( void ){

	.CSEG
_main:
; .FSTART _main
; 0002 0028 
; 0002 0029   unsigned int i,j;
; 0002 002A   unsigned char rollNum;
; 0002 002B   //partition_types ;
; 0002 002C 
; 0002 002D   /* initialize the USART control register
; 0002 002E    TX and RX enabled, no interrupts, 8 data bits */
; 0002 002F 
; 0002 0030 #ifdef PRINT_DEBUG
; 0002 0031 UCSR0A=0x00;
; 0002 0032 UCSR0B=0x18;
; 0002 0033 UCSR0C=0x06;
; 0002 0034 
; 0002 0035 //UBRR0H=UBRR >> 8;
; 0002 0036 UBRR0L=UBRR & 0xFF;
; 0002 0037 #else
; 0002 0038 
; 0002 0039 // Analog Comparator: Off
; 0002 003A //ACSR=0x80;
; 0002 003B //setup ADC buttons
; 0002 003C //DIDR0=0x00;
; 0002 003D ADMUX=ADC_VREF_TYPE & 0xff;  //0x20 AREF=reference, Left align=only MSB resd., ch=0
;	i -> R16,R17
;	j -> R18,R19
;	rollNum -> R21
	LDI  R30,LOW(32)
	STS  124,R30
; 0002 003E ADCSRA=0x83;    //  Prescaler=8 (1Mhz), Freerun.
	LDI  R30,LOW(131)
	STS  122,R30
; 0002 003F //ADCSRB=0x00;    //
; 0002 0040 #endif
; 0002 0041 
; 0002 0042 
; 0002 0043 LED_OK_DDR.PIN_LED_OK=1;
	SBI  0x4,1
; 0002 0044 LED_OK_ON();
	CBI  0x5,1
; 0002 0045 LED_ERR_DDR.PIN_LED_ERR=1;
	SBI  0x4,0
; 0002 0046 LED_ERR_OFF();
	SBI  0x5,0
; 0002 0047 
; 0002 0048 delay_ms(250);
	LDI  R26,LOW(250)
	LDI  R27,0
	CALL _delay_ms
; 0002 0049 
; 0002 004A i=0;j=0;
	__GETWRN 16,17,0
	__GETWRN 18,19,0
; 0002 004B 
; 0002 004C //putstr("Start\r");
; 0002 004D #ifndef PRINT_DEBUG
; 0002 004E while(1)
_0x4000B:
; 0002 004F {
; 0002 0050     if((TestADCs()==((1<<BUTTON_DOWN)))&&(j>10))
	RCALL _TestADCs
	CPI  R30,LOW(0x10)
	BRNE _0x4000F
	__CPWRN 18,19,11
	BRSH _0x40010
_0x4000F:
	RJMP _0x4000E
_0x40010:
; 0002 0051     {
; 0002 0052         LED_OK_OFF();LED_ERR_OFF();
	SBI  0x5,1
	SBI  0x5,0
; 0002 0053         if(++j>20 && i==0){
	MOVW R30,R18
	ADIW R30,1
	MOVW R18,R30
	SBIW R30,21
	BRLO _0x40016
	CLR  R0
	CP   R0,R16
	CPC  R0,R17
	BREQ _0x40017
_0x40016:
	RJMP _0x40015
_0x40017:
; 0002 0054             break;
	RJMP _0x4000D
; 0002 0055         }
; 0002 0056         i=0;
_0x40015:
	__GETWRN 16,17,0
; 0002 0057     }
; 0002 0058     else
	RJMP _0x40018
_0x4000E:
; 0002 0059     {
; 0002 005A         if(TestADCs()==((1<<BUTTON_STOP))){
	RCALL _TestADCs
	CPI  R30,LOW(0x4)
	BRNE _0x40019
; 0002 005B             LED_OK_TGL(); //toggle OK LED to show initiated STOP button press. Wait for DOWN press
	SBIS 0x5,1
	RJMP _0x4001A
	CBI  0x5,1
	RJMP _0x4001B
_0x4001A:
	SBI  0x5,1
_0x4001B:
; 0002 005C             if(j<11){
	__CPWRN 18,19,11
	BRSH _0x4001C
; 0002 005D                 j++;
	__ADDWRN 18,19,1
; 0002 005E             }
; 0002 005F         }
_0x4001C:
; 0002 0060         else{
	RJMP _0x4001D
_0x40019:
; 0002 0061             j=0;
	__GETWRN 18,19,0
; 0002 0062             LED_ERR_TGL();
	SBIS 0x5,0
	RJMP _0x4001E
	CBI  0x5,0
	RJMP _0x4001F
_0x4001E:
	SBI  0x5,0
_0x4001F:
; 0002 0063             //go to app after 1 sec
; 0002 0064             if(++i>10){
	MOVW R30,R16
	ADIW R30,1
	MOVW R16,R30
	SBIW R30,11
	BRLO _0x40020
; 0002 0065                 LED_OK_OFF();LED_ERR_OFF();
	CALL SUBOPT_0x1
; 0002 0066                 app_pointer();
; 0002 0067 
; 0002 0068             }
; 0002 0069         }
_0x40020:
_0x4001D:
; 0002 006A     }
_0x40018:
; 0002 006B     delay_ms(100);
	LDI  R26,LOW(100)
	LDI  R27,0
	CALL _delay_ms
; 0002 006C     //i++;j++;
; 0002 006D }
	RJMP _0x4000B
_0x4000D:
; 0002 006E #endif
; 0002 006F 
; 0002 0070   if((result[0]=SD_init())!=SD_SUCCESS)
	CALL _SD_init
	STS  _result,R30
	CPI  R30,0
	BREQ _0x40025
; 0002 0071   {
; 0002 0072     errorSD(1);
	LDI  R26,LOW(1)
	RCALL _errorSD
; 0002 0073   }
; 0002 0074   LED_OK_TGL();
_0x40025:
	SBIS 0x5,1
	RJMP _0x40026
	CBI  0x5,1
	RJMP _0x40027
_0x40026:
	SBI  0x5,1
_0x40027:
; 0002 0075   //putchar('i');
; 0002 0076   // read MBR get FAT start sector
; 0002 0077   if((result[0]=SD_readSingleBlock(0, sdBuf, &token))!=SD_SUCCESS)
	CALL SUBOPT_0x2
	CALL SUBOPT_0x3
	BREQ _0x40028
; 0002 0078   {
; 0002 0079     errorSD(2);
	LDI  R26,LOW(2)
	RCALL _errorSD
; 0002 007A   }
; 0002 007B   LED_OK_TGL();
_0x40028:
	SBIS 0x5,1
	RJMP _0x40029
	CBI  0x5,1
	RJMP _0x4002A
_0x40029:
	SBI  0x5,1
_0x4002A:
; 0002 007C   //putchar('1');
; 0002 007D   partition_type=FAT32;
	LDI  R30,LOW(2)
	STS  _partition_type,R30
; 0002 007E   if(sdBuf[0x1C2]<6){
	__GETB2MN _sdBuf,450
	CPI  R26,LOW(0x6)
	BRSH _0x4002B
; 0002 007F     partition_type=DOS;
	LDI  R30,LOW(0)
	RJMP _0x400A5
; 0002 0080   }
; 0002 0081   else if(sdBuf[0x1C2]<8){
_0x4002B:
	__GETB2MN _sdBuf,450
	CPI  R26,LOW(0x8)
	BRSH _0x4002D
; 0002 0082     partition_type=FAT16;
	LDI  R30,LOW(1)
_0x400A5:
	STS  _partition_type,R30
; 0002 0083   }
; 0002 0084 
; 0002 0085 #ifdef PRINT_DEBUG
; 0002 0086   //printhex("2",partition_type,1);
; 0002 0087 #endif
; 0002 0088   //select algoritm for FAT parse
; 0002 0089   //if((compbuf("MSDOS",&sdBuf[3])==0)||(partition_type==7)||(partition_type==6)||(partition_type==0xE)||(partition_type ...
; 0002 008A   if(partition_type != DOS)
_0x4002D:
	LDS  R30,_partition_type
	CPI  R30,0
	BREQ _0x4002E
; 0002 008B   {
; 0002 008C     adr=buf2num(&sdBuf[445+9],4);//FAT start sector. 1 sector = 512 bytes
	__POINTW1MN _sdBuf,454
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(4)
	RCALL _buf2num
	CALL SUBOPT_0x4
; 0002 008D 
; 0002 008E     //load and read FAT ID (1st) sector. Get FAT info. Secors per Cluster and etc..
; 0002 008F     if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS)
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
	BREQ _0x4002F
; 0002 0090     {
; 0002 0091     //putchar('e');//putchar('r');//putchar(result[0]);//putchar(token);
; 0002 0092         errorSD(3);
	LDI  R26,LOW(3)
	RCALL _errorSD
; 0002 0093     }
; 0002 0094     fat_begin_lba=buf2num(&sdBuf[0x0E],2);//reserved sectors
_0x4002F:
	CALL SUBOPT_0x7
	STS  _fat_begin_lba,R30
	STS  _fat_begin_lba+1,R31
	STS  _fat_begin_lba+2,R22
	STS  _fat_begin_lba+3,R23
; 0002 0095     fat_begin_lba+=adr;//first sector of FAT data
	CALL SUBOPT_0x5
	CALL SUBOPT_0x8
	RJMP _0x400A6
; 0002 0096     //printhex("3a",fat_begin_lba,4);
; 0002 0097   }
; 0002 0098   else//DOS parsing
_0x4002E:
; 0002 0099   {
; 0002 009A     fat_begin_lba=buf2num(&sdBuf[0x0E],2);//(reserved sectors)first sector of FAT data
	CALL SUBOPT_0x7
_0x400A6:
	STS  _fat_begin_lba,R30
	STS  _fat_begin_lba+1,R31
	STS  _fat_begin_lba+2,R22
	STS  _fat_begin_lba+3,R23
; 0002 009B     //putchar('2');//putchar('b');
; 0002 009C   }
; 0002 009D 
; 0002 009E   //printhex("3",fat_begin_lba,4);
; 0002 009F 
; 0002 00A0   SectorsPerCluster=sdBuf[0x0D];// 8 sectors per cluster
	__GETBRMN 4,_sdBuf,13
; 0002 00A1   //printhex("SpC",SectorsPerCluster,1);
; 0002 00A2 
; 0002 00A3   //if((partition_type==4)||(partition_type==6)||(partition_type==0xE)) //FAT/12/16
; 0002 00A4   if(partition_type != FAT32)
	LDS  R26,_partition_type
	CPI  R26,LOW(0x2)
	BREQ _0x40031
; 0002 00A5   {
; 0002 00A6       SectorsPerFat=buf2num(&sdBuf[22],2); // 0xF10 for test sdcard
	__POINTW1MN _sdBuf,22
	CALL SUBOPT_0x9
	CALL SUBOPT_0xA
; 0002 00A7       RootEntryCnt= buf2num(&sdBuf[11],2)>>4;
	__POINTW1MN _sdBuf,11
	CALL SUBOPT_0x9
	MOVW R26,R30
	MOVW R24,R22
	LDI  R30,LOW(4)
	CALL __LSRD12
	MOVW R6,R30
; 0002 00A8   }
; 0002 00A9   else{//FAT32
	RJMP _0x40032
_0x40031:
; 0002 00AA       SectorsPerFat=buf2num(&sdBuf[0x24],4); // 0xF10 for test sdcard
	__POINTW1MN _sdBuf,36
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(4)
	RCALL _buf2num
	CALL SUBOPT_0xA
; 0002 00AB       RootEntryCnt=0;
	CLR  R6
	CLR  R7
; 0002 00AC   }
_0x40032:
; 0002 00AD   //printhex("SpF",SectorsPerFat,4);
; 0002 00AE 
; 0002 00AF   //read the FAT fils/directories info from Root Directory cluster (usually 2),Number_of_Reserved_Sectors (usually 0x20) ...
; 0002 00B0   //(unsigned long)fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
; 0002 00B1   //fat_begin_lba=adr+0x20;//first sector of FAT data
; 0002 00B2   //(unsigned long)cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_ ...
; 0002 00B3   //Number_of_FATs always 2. Offset 0x10 8bit
; 0002 00B4   cluster_begin_lba=fat_begin_lba+(2*SectorsPerFat);//number of sector where data begin
	LDS  R30,_SectorsPerFat
	LDS  R31,_SectorsPerFat+1
	LDS  R22,_SectorsPerFat+2
	LDS  R23,_SectorsPerFat+3
	CALL __LSLD1
	CALL SUBOPT_0x8
	STS  _cluster_begin_lba,R30
	STS  _cluster_begin_lba+1,R31
	STS  _cluster_begin_lba+2,R22
	STS  _cluster_begin_lba+3,R23
; 0002 00B5   //printhex("cbl",cluster_begin_lba,4);
; 0002 00B6   //read root dir (sector 2 but always offset 2 too then 0) to find folder 0 FAT reference. and find Flash.dat sector
; 0002 00B7   //lba_addr = cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;
; 0002 00B8   adr=cluster_begin_lba +(2-2)*SectorsPerCluster;
	CALL SUBOPT_0x4
; 0002 00B9   //adr*=512UL;
; 0002 00BA   result[1]=0;
	LDI  R30,LOW(0)
	__PUTB1MN _result,1
; 0002 00BB   LED_OK_TGL();
	SBIS 0x5,1
	RJMP _0x40033
	CBI  0x5,1
	RJMP _0x40034
_0x40033:
	SBI  0x5,1
_0x40034:
; 0002 00BC   for(i=0;i<SectorsPerCluster;i++)
	__GETWRN 16,17,0
_0x40036:
	CALL SUBOPT_0xB
	BRSH _0x40037
; 0002 00BD   {
; 0002 00BE       if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS){
	CALL SUBOPT_0xC
	BREQ _0x40038
; 0002 00BF         errorSD(4);
	LDI  R26,LOW(4)
	RCALL _errorSD
; 0002 00C0       }
; 0002 00C1 #ifdef PRINT_DEBUG
; 0002 00C2       printhex("4",adr,4);
; 0002 00C3 #endif
; 0002 00C4       for(j=0;j<(16);j++)
_0x40038:
	__GETWRN 18,19,0
_0x4003A:
	__CPWRN 18,19,16
	BRSH _0x4003B
; 0002 00C5       {
; 0002 00C6            //putchar('5');
; 0002 00C7            if((result[1]=compbuf("0          ",&sdBuf[j*32]))!=0)
	__POINTW1MN _0x4003D,0
	CALL SUBOPT_0xD
	BRNE _0x4003B
; 0002 00C8            {
; 0002 00C9                 break;
; 0002 00CA            }
; 0002 00CB       }
	__ADDWRN 18,19,1
	RJMP _0x4003A
_0x4003B:
; 0002 00CC       //putchar('6');
; 0002 00CD       if(result[1]!=0)
	__GETB1MN _result,1
	CPI  R30,0
	BREQ _0x4003E
; 0002 00CE       {
; 0002 00CF         //putchar('7');
; 0002 00D0         fat_file_adr =buf2num(&sdBuf[j*32+0x14],2)<<16;
	CALL SUBOPT_0xE
	__ADDW1MN _sdBuf,20
	CALL SUBOPT_0x9
	CALL SUBOPT_0xF
; 0002 00D1         fat_file_adr|=buf2num(&sdBuf[j*32+0x1A],2);
	__ADDW1MN _sdBuf,26
	CALL SUBOPT_0x9
	CALL SUBOPT_0x10
; 0002 00D2         break;
	RJMP _0x40037
; 0002 00D3       }
; 0002 00D4       else
_0x4003E:
; 0002 00D5       {
; 0002 00D6         adr++;
	CALL SUBOPT_0x11
; 0002 00D7       }
; 0002 00D8       LED_OK_TGL();
	SBIS 0x5,1
	RJMP _0x40040
	CBI  0x5,1
	RJMP _0x40041
_0x40040:
	SBI  0x5,1
_0x40041:
; 0002 00D9   }
	__ADDWRN 16,17,1
	RJMP _0x40036
_0x40037:
; 0002 00DA   if(result[1]==0)
	__GETB1MN _result,1
	CPI  R30,0
	BRNE _0x40042
; 0002 00DB   {
; 0002 00DC       //putchar('f');//putchar('0');
; 0002 00DD       errorSD(5);//folder '0' not found
	LDI  R26,LOW(5)
	RCALL _errorSD
; 0002 00DE   }
; 0002 00DF 
; 0002 00E0   adr=cluster_begin_lba+ RootEntryCnt +(fat_file_adr-2)*SectorsPerCluster;
_0x40042:
	CALL SUBOPT_0x12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x13
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDD12
	CALL SUBOPT_0x4
; 0002 00E1   /*
; 0002 00E2   printhex("7",cluster_begin_lba,4);
; 0002 00E3   printhex("7",RootEntryCnt,4);
; 0002 00E4   printhex("7",fat_file_adr,4);
; 0002 00E5   printhex("7",SectorsPerCluster,4);
; 0002 00E6   printhex("7",adr,4);
; 0002 00E7   */
; 0002 00E8 
; 0002 00E9   LED_OK_TGL();
	SBIS 0x5,1
	RJMP _0x40043
	CBI  0x5,1
	RJMP _0x40044
_0x40043:
	SBI  0x5,1
_0x40044:
; 0002 00EA   for(i=0;i<SectorsPerCluster;i++)
	__GETWRN 16,17,0
_0x40046:
	CALL SUBOPT_0xB
	BRLO PC+2
	RJMP _0x40047
; 0002 00EB   {
; 0002 00EC       //putchar('9');
; 0002 00ED #ifdef PRINT_DEBUG
; 0002 00EE       //printhex("8",adr,4);
; 0002 00EF #endif
; 0002 00F0       if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS)
	CALL SUBOPT_0xC
	BREQ _0x40048
; 0002 00F1       {
; 0002 00F2         errorSD(6);
	LDI  R26,LOW(6)
	RCALL _errorSD
; 0002 00F3       }
; 0002 00F4       //putchar('A');
; 0002 00F5       for(j=0;j<(16);j++)
_0x40048:
	__GETWRN 18,19,0
_0x4004A:
	__CPWRN 18,19,16
	BRSH _0x4004B
; 0002 00F6       {
; 0002 00F7            //putchar('B');
; 0002 00F8            if((result[1]=compbuf("FLASH   ",&sdBuf[j*32]))!=0)
	__POINTW1MN _0x4003D,12
	CALL SUBOPT_0xD
	BRNE _0x4004B
; 0002 00F9            {
; 0002 00FA                 //putchar('C');
; 0002 00FB                 break;
; 0002 00FC            }
; 0002 00FD       }
	__ADDWRN 18,19,1
	RJMP _0x4004A
_0x4004B:
; 0002 00FE       if(result[1]!=0)
	__GETB1MN _result,1
	CPI  R30,0
	BREQ _0x4004D
; 0002 00FF       {
; 0002 0100         //putchar('D');
; 0002 0101         //read 1st number of cluster where data placed
; 0002 0102         fat_file_adr =buf2num(&sdBuf[j*32+0x14],2)<<16;
	CALL SUBOPT_0xE
	__ADDW1MN _sdBuf,20
	CALL SUBOPT_0x9
	CALL SUBOPT_0xF
; 0002 0103         fat_file_adr|=buf2num(&sdBuf[j*32+0x1A],2);
	__ADDW1MN _sdBuf,26
	CALL SUBOPT_0x9
	CALL SUBOPT_0x10
; 0002 0104         filesize = buf2num(&sdBuf[j*32+0x1C],8);
	CALL SUBOPT_0xE
	__ADDW1MN _sdBuf,28
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(8)
	RCALL _buf2num
	STS  _filesize,R30
	STS  _filesize+1,R31
	STS  _filesize+2,R22
	STS  _filesize+3,R23
; 0002 0105         break;
	RJMP _0x40047
; 0002 0106       }
; 0002 0107       else
_0x4004D:
; 0002 0108       {
; 0002 0109         adr++;
	CALL SUBOPT_0x11
; 0002 010A       }
; 0002 010B       LED_OK_TGL();
	SBIS 0x5,1
	RJMP _0x4004F
	CBI  0x5,1
	RJMP _0x40050
_0x4004F:
	SBI  0x5,1
_0x40050:
; 0002 010C   }
	__ADDWRN 16,17,1
	RJMP _0x40046
_0x40047:
; 0002 010D 
; 0002 010E   if(result[1]==0)
	__GETB1MN _result,1
	CPI  R30,0
	BRNE _0x40051
; 0002 010F   {
; 0002 0110       //putchar('f');//putchar('0');
; 0002 0111       errorSD(16);//folder '0' not found
	LDI  R26,LOW(16)
	RCALL _errorSD
; 0002 0112   }
; 0002 0113 
; 0002 0114   //check FAT for chain of clusters to read
; 0002 0115   readbytes=0;
_0x40051:
	LDI  R30,LOW(0)
	STS  _readbytes,R30
	STS  _readbytes+1,R30
	STS  _readbytes+2,R30
	STS  _readbytes+3,R30
; 0002 0116   while((fat_file_adr != 0x0FFFFFFFUL) && (fat_file_adr != 0xFFFF))
_0x40052:
	CALL SUBOPT_0x14
	CALL SUBOPT_0x15
	BREQ _0x40055
	CALL SUBOPT_0x14
	CALL SUBOPT_0x16
	BRNE _0x40056
_0x40055:
	RJMP _0x40054
_0x40056:
; 0002 0117   {
; 0002 0118     //read where next cluster from FAT, check that not EOF
; 0002 0119 #ifdef PRINT_DEBUG
; 0002 011A     //printhex("7a",fat_file_adr,4);
; 0002 011B     //printhex("7b",fat_begin_lba,4);
; 0002 011C #endif
; 0002 011D     if((result[0]=SD_readSingleBlock(fat_begin_lba+(fat_file_adr>>8), sdBuf, &token))!=SD_SUCCESS){
	CALL SUBOPT_0x14
	LDI  R30,LOW(8)
	CALL __LSRD12
	CALL SUBOPT_0x8
	CALL SUBOPT_0x6
	BREQ _0x40057
; 0002 011E         errorSD(7);
	LDI  R26,LOW(7)
	RCALL _errorSD
; 0002 011F     }
; 0002 0120     if(partition_type!=FAT32){ //FAT/12/16
_0x40057:
	LDS  R26,_partition_type
	CPI  R26,LOW(0x2)
	BREQ _0x40058
; 0002 0121         fat_file_next_adr=buf2num(&sdBuf[(fat_file_adr<<1)%0x200],2);
	LDS  R30,_fat_file_adr
	LDS  R31,_fat_file_adr+1
	LSL  R30
	ROL  R31
	ANDI R31,HIGH(0x1FF)
	SUBI R30,LOW(-_sdBuf)
	SBCI R31,HIGH(-_sdBuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(2)
	RJMP _0x400A7
; 0002 0122     }
; 0002 0123     else{//FAT32
_0x40058:
; 0002 0124         fat_file_next_adr=buf2num(&sdBuf[(fat_file_adr<<2)%0x200],4);
	LDS  R30,_fat_file_adr
	LDS  R31,_fat_file_adr+1
	CALL __LSLW2
	ANDI R31,HIGH(0x1FF)
	SUBI R30,LOW(-_sdBuf)
	SBCI R31,HIGH(-_sdBuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(4)
_0x400A7:
	RCALL _buf2num
	STS  _fat_file_next_adr,R30
	STS  _fat_file_next_adr+1,R31
	STS  _fat_file_next_adr+2,R22
	STS  _fat_file_next_adr+3,R23
; 0002 0125     }
; 0002 0126 #ifdef PRINT_DEBUG
; 0002 0127     //printhex("7c",fat_file_next_adr,4);
; 0002 0128 #endif
; 0002 0129     //putchar('E');
; 0002 012A     adr=cluster_begin_lba+ RootEntryCnt +(fat_file_adr-2)*SectorsPerCluster;
	CALL SUBOPT_0x12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x13
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDD12
	RCALL SUBOPT_0x4
; 0002 012B     for(i=0;i<SectorsPerCluster;i++)
	__GETWRN 16,17,0
_0x4005B:
	RCALL SUBOPT_0xB
	BRLO PC+2
	RJMP _0x4005C
; 0002 012C     {
; 0002 012D         //read data from next sector of file cluster
; 0002 012E #ifdef PRINT_DEBUG
; 0002 012F         //printhex("8",adr,4);
; 0002 0130 #endif
; 0002 0131         if((result[0]=SD_readSingleBlock(adr, sdBuf, &token))!=SD_SUCCESS){
	RCALL SUBOPT_0xC
	BREQ _0x4005D
; 0002 0132             errorSD(8);
	LDI  R26,LOW(8)
	RCALL _errorSD
; 0002 0133         }
; 0002 0134         //putchar('F');
; 0002 0135         //address 2000 = start adr flash app 3 bytes, flash pages 2 bytes, checksum 2 bytes
; 0002 0136         //app bytes starts from 2048, roll 0x88
; 0002 0137         if(readbytes==0)
_0x4005D:
	CALL SUBOPT_0x17
	CALL __CPD10
	BRNE _0x4005E
; 0002 0138         {
; 0002 0139             for(rollNum=1;rollNum!=0;rollNum++)
	LDI  R21,LOW(1)
_0x40060:
	CPI  R21,0
	BREQ _0x40061
; 0002 013A             {
; 0002 013B                 for(j=0;j<10;j++)
	__GETWRN 18,19,0
_0x40063:
	__CPWRN 18,19,10
	BRSH _0x40064
; 0002 013C                 {
; 0002 013D                     testBuf[j]=(sdBuf[j]<<1)|(sdBuf[j]>>7);  //ROL
	MOVW R30,R18
	SUBI R30,LOW(-_testBuf)
	SBCI R31,HIGH(-_testBuf)
	CALL SUBOPT_0x18
; 0002 013E                     testBuf[j]^=rollNum;//0x88;  //XOR
	SUBI R26,LOW(-_testBuf)
	SBCI R27,HIGH(-_testBuf)
	LD   R30,X
	EOR  R30,R21
	ST   X,R30
; 0002 013F                 }
	__ADDWRN 18,19,1
	RJMP _0x40063
_0x40064:
; 0002 0140                 if(compbuf("[settings]",testBuf))
	__POINTW1MN _0x4003D,21
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(_testBuf)
	LDI  R27,HIGH(_testBuf)
	RCALL _compbuf
	CPI  R30,0
	BRNE _0x40061
; 0002 0141                 {
; 0002 0142                     //printhex("rol=",rollNum,1);
; 0002 0143                     break;
; 0002 0144                 }
; 0002 0145             }
	SUBI R21,-1
	RJMP _0x40060
_0x40061:
; 0002 0146             if(rollNum==0){
	CPI  R21,0
	BRNE _0x40066
; 0002 0147                  errorSD(9);
	LDI  R26,LOW(9)
	RCALL _errorSD
; 0002 0148             }
; 0002 0149         }
_0x40066:
; 0002 014A         for(j=0;j<512;j++)
_0x4005E:
	__GETWRN 18,19,0
_0x40068:
	__CPWRN 18,19,512
	BRSH _0x40069
; 0002 014B         {
; 0002 014C             sdBuf[j]=(sdBuf[j]<<1)|(sdBuf[j]>>7);  //ROL
	MOVW R30,R18
	SUBI R30,LOW(-_sdBuf)
	SBCI R31,HIGH(-_sdBuf)
	CALL SUBOPT_0x18
; 0002 014D             sdBuf[j]^=rollNum;//0x88;  //XOR
	SUBI R26,LOW(-_sdBuf)
	SBCI R27,HIGH(-_sdBuf)
	LD   R30,X
	EOR  R30,R21
	ST   X,R30
; 0002 014E             checksumCnt+=sdBuf[j];
	LDI  R26,LOW(_sdBuf)
	LDI  R27,HIGH(_sdBuf)
	ADD  R26,R18
	ADC  R27,R19
	LD   R30,X
	LDI  R31,0
	LDS  R26,_checksumCnt
	LDS  R27,_checksumCnt+1
	ADD  R30,R26
	ADC  R31,R27
	STS  _checksumCnt,R30
	STS  _checksumCnt+1,R31
; 0002 014F             //if(readbytes>2000)
; 0002 0150                 ////putchar(sdBuf[j]);
; 0002 0151         }
	__ADDWRN 18,19,1
	RJMP _0x40068
_0x40069:
; 0002 0152         readbytes+=512;
	CALL SUBOPT_0x17
	__ADDD1N 512
	STS  _readbytes,R30
	STS  _readbytes+1,R31
	STS  _readbytes+2,R22
	STS  _readbytes+3,R23
; 0002 0153         //read app data
; 0002 0154         if(readbytes>2048)
	CALL SUBOPT_0x19
	__CPD2N 0x801
	BRLO _0x4006A
; 0002 0155         {
; 0002 0156            for(pagesCnt=0;pagesCnt<PAGES_PER_SDBUF;pagesCnt++)
	CLR  R10
	CLR  R11
_0x4006C:
	LDI  R30,LOW(4)
	LDI  R31,HIGH(4)
	CP   R10,R30
	CPC  R11,R31
	BRSH _0x4006D
; 0002 0157            {
; 0002 0158                LED_OK_TGL();
	SBIS 0x5,1
	RJMP _0x4006E
	CBI  0x5,1
	RJMP _0x4006F
_0x4006E:
	SBI  0x5,1
_0x4006F:
; 0002 0159                if(WriteFlashPage(appStartAdr, &sdBuf[pagesCnt*(int)PAGESIZE])==0)
	LDS  R30,_appStartAdr
	LDS  R31,_appStartAdr+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	CALL __LSLW3
	CALL __LSLW4
	SUBI R30,LOW(-_sdBuf)
	SBCI R31,HIGH(-_sdBuf)
	MOVW R26,R30
	CALL _WriteFlashPage
	CPI  R30,0
	BRNE _0x40070
; 0002 015A                {
; 0002 015B                     errorSD(10);
	LDI  R26,LOW(10)
	RCALL _errorSD
; 0002 015C                }
; 0002 015D                appStartAdr+=PAGESIZE;
_0x40070:
	LDS  R30,_appStartAdr
	LDS  R31,_appStartAdr+1
	LDS  R22,_appStartAdr+2
	LDS  R23,_appStartAdr+3
	__ADDD1N 128
	CALL SUBOPT_0x1A
; 0002 015E                appPages--;
	MOVW R30,R8
	SBIW R30,1
	MOVW R8,R30
; 0002 015F                if(appPages==0)
	MOV  R0,R8
	OR   R0,R9
	BRNE _0x40071
; 0002 0160                {
; 0002 0161                     LED_OK_OFF();LED_ERR_OFF();
	RCALL SUBOPT_0x1
; 0002 0162                     //putchar('P');putchar('\r');
; 0002 0163                     app_pointer();
; 0002 0164                }
; 0002 0165            }
_0x40071:
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
	RJMP _0x4006C
_0x4006D:
; 0002 0166         }
; 0002 0167         //read app start adr, num of pages, checksum
; 0002 0168         else if(readbytes>=2000)//Offset=512-48=464
	RJMP _0x40076
_0x4006A:
	RCALL SUBOPT_0x19
	__CPD2N 0x7D0
	BRSH PC+2
	RJMP _0x40077
; 0002 0169         {
; 0002 016A            if(PAGESIZE!=((((unsigned int)sdBuf[478]<<8)|((unsigned int)sdBuf[479]))<<1))
	__GETBRMN 27,_sdBuf,478
	LDI  R26,LOW(0)
	__GETB1MN _sdBuf,479
	LDI  R31,0
	OR   R30,R26
	OR   R31,R27
	LSL  R30
	ROL  R31
	CPI  R30,LOW(0x80)
	LDI  R26,HIGH(0x80)
	CPC  R31,R26
	BREQ _0x40078
; 0002 016B            {
; 0002 016C                errorSD(11);
	LDI  R26,LOW(11)
	RCALL _errorSD
; 0002 016D            }
; 0002 016E            appStartAdr=(unsigned long)sdBuf[464]<<16;
_0x40078:
	__GETB1MN _sdBuf,464
	LDI  R31,0
	CALL __CWD1
	CALL __LSLD16
	RCALL SUBOPT_0x1A
; 0002 016F            appStartAdr|=(unsigned long)sdBuf[465]<<8;
	__GETB1MN _sdBuf,465
	LDI  R31,0
	CALL __CWD1
	MOVW R26,R30
	MOVW R24,R22
	LDI  R30,LOW(8)
	CALL __LSLD12
	RCALL SUBOPT_0x1B
; 0002 0170            appStartAdr|=(unsigned long)sdBuf[466];
	__GETB1MN _sdBuf,466
	LDI  R31,0
	CALL __CWD1
	RCALL SUBOPT_0x1B
; 0002 0171            appPages=(unsigned int)sdBuf[467]<<8;
	__GETBRMN 31,_sdBuf,467
	LDI  R30,LOW(0)
	MOVW R8,R30
; 0002 0172            appPages|=(unsigned int)sdBuf[468];
	__GETB1MN _sdBuf,468
	LDI  R31,0
	__ORWRR 8,9,30,31
; 0002 0173            bytesChecksum=(unsigned int)sdBuf[469]<<8;
	__GETBRMN 31,_sdBuf,469
	LDI  R30,LOW(0)
	MOVW R12,R30
; 0002 0174            bytesChecksum|=(unsigned int)sdBuf[470];
	__GETB1MN _sdBuf,470
	LDI  R31,0
	__ORWRR 12,13,30,31
; 0002 0175            checksumCnt=0;
	LDI  R30,LOW(0)
	STS  _checksumCnt,R30
	STS  _checksumCnt+1,R30
; 0002 0176            //putchar('a');//putchar('d');//putchar('r');//putchar(sdBuf[464]);//putchar(sdBuf[465]);//putchar(sdBuf[466] ...
; 0002 0177         }
; 0002 0178         if((fat_file_next_adr == 0x0FFFFFFFUL) || (fat_file_next_adr == 0xFFFF)){
_0x40077:
_0x40076:
	RCALL SUBOPT_0x1C
	RCALL SUBOPT_0x15
	BREQ _0x4007A
	RCALL SUBOPT_0x1C
	RCALL SUBOPT_0x16
	BRNE _0x40079
_0x4007A:
; 0002 0179             if(readbytes >= filesize)
	LDS  R30,_filesize
	LDS  R31,_filesize+1
	LDS  R22,_filesize+2
	LDS  R23,_filesize+3
	RCALL SUBOPT_0x19
	CALL __CPD21
	BRSH _0x4005C
; 0002 017A             {
; 0002 017B                 break;
; 0002 017C             }
; 0002 017D             else if(i>=(SectorsPerCluster-1))
	MOV  R30,R4
	LDI  R31,0
	SBIW R30,1
	CP   R16,R30
	CPC  R17,R31
	BRLO _0x4007E
; 0002 017E             {
; 0002 017F                 //printhex("12a",readbytes,4);
; 0002 0180                 //printhex("12b",filesize,4);
; 0002 0181                 errorSD(12);
	LDI  R26,LOW(12)
	RCALL _errorSD
; 0002 0182             }
; 0002 0183         }
_0x4007E:
; 0002 0184         adr++;
_0x40079:
	RCALL SUBOPT_0x11
; 0002 0185     }
	__ADDWRN 16,17,1
	RJMP _0x4005B
_0x4005C:
; 0002 0186     fat_file_adr = fat_file_next_adr;
	LDS  R30,_fat_file_next_adr
	LDS  R31,_fat_file_next_adr+1
	LDS  R22,_fat_file_next_adr+2
	LDS  R23,_fat_file_next_adr+3
	STS  _fat_file_adr,R30
	STS  _fat_file_adr+1,R31
	STS  _fat_file_adr+2,R22
	STS  _fat_file_adr+3,R23
; 0002 0187   }
	RJMP _0x40052
_0x40054:
; 0002 0188   while(1);
_0x4007F:
	RJMP _0x4007F
; 0002 0189 }
_0x40082:
	RJMP _0x40082
; .FEND

	.DSEG
_0x4003D:
	.BYTE 0x20
;
;unsigned char compbuf(const unsigned char *src,unsigned char *dest)
; 0002 018C {

	.CSEG
_compbuf:
; .FSTART _compbuf
; 0002 018D     while(*src)
	ST   -Y,R27
	ST   -Y,R26
;	*src -> Y+2
;	*dest -> Y+0
_0x40083:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x40085
; 0002 018E     {
; 0002 018F         if(*src++ != *dest++)
	LD   R0,X+
	STD  Y+2,R26
	STD  Y+2+1,R27
	LD   R26,Y
	LDD  R27,Y+1
	LD   R30,X+
	ST   Y,R26
	STD  Y+1,R27
	CP   R30,R0
	BREQ _0x40086
; 0002 0190             return 0;
	LDI  R30,LOW(0)
	RJMP _0x2000008
; 0002 0191         //src++;dest++;
; 0002 0192         //len--;
; 0002 0193     }
_0x40086:
	RJMP _0x40083
_0x40085:
; 0002 0194     return 1;
	LDI  R30,LOW(1)
	RJMP _0x2000008
; 0002 0195 }
; .FEND
;
;void errorSD(unsigned char error_step)
; 0002 0198 {
_errorSD:
; .FSTART _errorSD
; 0002 0199     unsigned char i;
; 0002 019A #ifdef PRINT_DEBUG
; 0002 019B     printhex("err:",error_step,1);
; 0002 019C #endif
; 0002 019D     PORTC.1=0;
	ST   -Y,R26
	ST   -Y,R17
;	error_step -> Y+1
;	i -> R17
	CBI  0x8,1
; 0002 019E     LED_OK_OFF();
	SBI  0x5,1
; 0002 019F     for(i=0;i<error_step;i++){
	LDI  R17,LOW(0)
_0x4008C:
	LDD  R30,Y+1
	CP   R17,R30
	BRSH _0x4008D
; 0002 01A0         LED_ERR_OFF();
	SBI  0x5,0
; 0002 01A1         delay_ms(300);
	LDI  R26,LOW(300)
	LDI  R27,HIGH(300)
	CALL _delay_ms
; 0002 01A2         LED_ERR_ON();
	CBI  0x5,0
; 0002 01A3         delay_ms(300);
	LDI  R26,LOW(300)
	LDI  R27,HIGH(300)
	CALL _delay_ms
; 0002 01A4     }
	SUBI R17,-1
	RJMP _0x4008C
_0x4008D:
; 0002 01A5 
; 0002 01A6     while(1);
_0x40092:
	RJMP _0x40092
; 0002 01A7 }
; .FEND
;
;unsigned long buf2num(unsigned char *buf,unsigned char len)
; 0002 01AA {
_buf2num:
; .FSTART _buf2num
; 0002 01AB     unsigned long num=0;
; 0002 01AC     //unsigned char i;
; 0002 01AD     for(;len>0;len--)
	ST   -Y,R26
	SBIW R28,4
	LDI  R30,LOW(0)
	ST   Y,R30
	STD  Y+1,R30
	STD  Y+2,R30
	STD  Y+3,R30
;	*buf -> Y+5
;	len -> Y+4
;	num -> Y+0
_0x40096:
	LDD  R26,Y+4
	CPI  R26,LOW(0x1)
	BRLO _0x40097
; 0002 01AE     {
; 0002 01AF         num<<=8;
	CALL __GETD2S0
	LDI  R30,LOW(8)
	CALL __LSLD12
	CALL __PUTD1S0
; 0002 01B0         num|=buf[len-1];
	LDD  R30,Y+4
	LDI  R31,0
	SBIW R30,1
	LDD  R26,Y+5
	LDD  R27,Y+5+1
	ADD  R26,R30
	ADC  R27,R31
	LD   R30,X
	CALL __GETD2S0
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __ORD12
	CALL __PUTD1S0
; 0002 01B1     }
	LDD  R30,Y+4
	SUBI R30,LOW(1)
	STD  Y+4,R30
	RJMP _0x40096
_0x40097:
; 0002 01B2     return num;
	CALL __GETD1S0
	RJMP _0x2000009
; 0002 01B3 }
; .FEND
;
;#ifndef PRINT_DEBUG
;unsigned char TestADCs()
; 0002 01B7 {
_TestADCs:
; .FSTART _TestADCs
; 0002 01B8     unsigned char StateOfButtons=0;
; 0002 01B9     unsigned char adc_data;
; 0002 01BA     ADMUX=(6 | (ADC_VREF_TYPE & 0xff)); //ADC6 ch=6, ADC7 ch=7
	ST   -Y,R17
	ST   -Y,R16
;	StateOfButtons -> R17
;	adc_data -> R16
	LDI  R17,0
	LDI  R30,LOW(38)
	RCALL SUBOPT_0x1D
; 0002 01BB     // Delay needed for the stabilization of the ADC input voltage
; 0002 01BC     delay_us(20);
; 0002 01BD     // Start the AD conversion
; 0002 01BE     ADCSRA|=0x40;
; 0002 01BF     while(ADCSRA&(1<<ADSC));//while(ADC_DONE==FALSE);
_0x40098:
	LDS  R30,122
	ANDI R30,LOW(0x40)
	BRNE _0x40098
; 0002 01C0     adc_data=ADCH;
	LDS  R16,121
; 0002 01C1     if(adc_data<50)
	CPI  R16,50
	BRSH _0x4009B
; 0002 01C2         StateOfButtons|=1<<BUTTON_DOWN;
	ORI  R17,LOW(16)
; 0002 01C3     else if(adc_data<150)
	RJMP _0x4009C
_0x4009B:
	CPI  R16,150
	BRSH _0x4009D
; 0002 01C4         StateOfButtons|=1<<BUTTON_STOP;
	ORI  R17,LOW(4)
; 0002 01C5     ADMUX=(7 | (ADC_VREF_TYPE & 0xff)); //ADC6 ch=6, ADC7 ch=7
_0x4009D:
_0x4009C:
	LDI  R30,LOW(39)
	RCALL SUBOPT_0x1D
; 0002 01C6     // Delay needed for the stabilization of the ADC input voltage
; 0002 01C7     delay_us(20);
; 0002 01C8     // Start the AD conversion
; 0002 01C9     ADCSRA|=0x40;
; 0002 01CA     while(ADCSRA&(1<<ADSC));//while(ADC_DONE==FALSE);
_0x4009E:
	LDS  R30,122
	ANDI R30,LOW(0x40)
	BRNE _0x4009E
; 0002 01CB     adc_data=ADCH;
	LDS  R16,121
; 0002 01CC     if(adc_data<50)
	CPI  R16,50
	BRSH _0x400A1
; 0002 01CD         StateOfButtons|=1<<BUTTON_UP;
	ORI  R17,LOW(8)
; 0002 01CE     else if(adc_data<150)
	RJMP _0x400A2
_0x400A1:
	CPI  R16,150
	BRSH _0x400A3
; 0002 01CF         StateOfButtons|=1<<BUTTON_START;
	ORI  R17,LOW(2)
; 0002 01D0     if(StateOfButtons)
_0x400A3:
_0x400A2:
	CPI  R17,0
	BREQ _0x400A4
; 0002 01D1         return StateOfButtons;
	MOV  R30,R17
	RJMP _0x200000A
; 0002 01D2     return (1<<BUTTONS_RELEASED);
_0x400A4:
	LDI  R30,LOW(1)
_0x200000A:
	LD   R16,Y+
	LD   R17,Y+
	RET
; 0002 01D3 }
; .FEND
;
;#endif
;#include "spi_sdcard.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include <delay.h>
;
;
;
;void SPI_init()
; 0003 0007 {

	.CSEG
_SPI_init:
; .FSTART _SPI_init
; 0003 0008     // set CS, MOSI and SCK to output
; 0003 0009     DDR_SPI |= (1 << CS) | (1 << MOSI) | (1 << SCK);
	IN   R30,0x4
	ORI  R30,LOW(0x2C)
	OUT  0x4,R30
; 0003 000A     PORT_SPI|=(1 << CS);
	SBI  0x5,2
; 0003 000B     // enable pull up resistor in MISO
; 0003 000C     DDR_SPI &= ~(1 << MISO);
	CBI  0x4,4
; 0003 000D     //PORT_SPI &= ~(1 << MISO);
; 0003 000E     PORT_SPI |= (1 << MISO);
	SBI  0x5,4
; 0003 000F 
; 0003 0010     // enable SPI, set as master, and clock to fosc/128
; 0003 0011     SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	LDI  R30,LOW(83)
	OUT  0x2C,R30
; 0003 0012 }
	RET
; .FEND
;
;unsigned char SPI_transfer(unsigned char data)
; 0003 0015 {
_SPI_transfer:
; .FSTART _SPI_transfer
; 0003 0016     // load data into register
; 0003 0017     SPDR = data;
	ST   -Y,R26
;	data -> Y+0
	LD   R30,Y
	OUT  0x2E,R30
; 0003 0018 
; 0003 0019     // Wait for transmission complete
; 0003 001A     while(!(SPSR & (1 << SPIF)));
_0x60003:
	IN   R30,0x2D
	ANDI R30,LOW(0x80)
	BREQ _0x60003
; 0003 001B 
; 0003 001C     // return SPDR
; 0003 001D     return SPDR;
	IN   R30,0x2E
	ADIW R28,1
	RET
; 0003 001E }
; .FEND
;
;
;void SD_powerUpSeq()
; 0003 0022 {
_SD_powerUpSeq:
; .FSTART _SD_powerUpSeq
; 0003 0023     unsigned char i;
; 0003 0024 
; 0003 0025     SPI_init();
	ST   -Y,R17
;	i -> R17
	RCALL _SPI_init
; 0003 0026 
; 0003 0027     // make sure card is deselected
; 0003 0028     CS_DISABLE();
	SBI  0x5,2
; 0003 0029 
; 0003 002A     // give SD card time to power up
; 0003 002B     delay_ms(1);
	LDI  R26,LOW(1)
	LDI  R27,0
	CALL _delay_ms
; 0003 002C 
; 0003 002D     // send 80 clock cycles to synchronize
; 0003 002E     for(i = 0; i < 10; i++)
	LDI  R17,LOW(0)
_0x60007:
	CPI  R17,10
	BRSH _0x60008
; 0003 002F         SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	SUBI R17,-1
	RJMP _0x60007
_0x60008:
; 0003 0032 PORTB |= (1 << 2       );
	RCALL SUBOPT_0x1E
; 0003 0033     SPI_transfer(0xFF);
; 0003 0034 }
	RJMP _0x2000005
; .FEND
;
;unsigned char SD_command(unsigned char cmd, unsigned long arg, unsigned char crc)
; 0003 0037 {
_SD_command:
; .FSTART _SD_command
; 0003 0038     unsigned char res;//,count;
; 0003 0039     // transmit command to sd card
; 0003 003A     SPI_transfer(cmd|0x40);
	ST   -Y,R26
	ST   -Y,R17
;	cmd -> Y+6
;	arg -> Y+2
;	crc -> Y+1
;	res -> R17
	LDD  R30,Y+6
	ORI  R30,0x40
	RCALL SUBOPT_0x1F
; 0003 003B 
; 0003 003C     // transmit argument
; 0003 003D     SPI_transfer((unsigned char)(arg >> 24));
	LDI  R30,LOW(24)
	CALL __LSRD12
	MOV  R26,R30
	RCALL _SPI_transfer
; 0003 003E     SPI_transfer((unsigned char)(arg >> 16));
	__GETD1S 2
	CALL __LSRD16
	RCALL SUBOPT_0x1F
; 0003 003F     SPI_transfer((unsigned char)(arg >> 8));
	LDI  R30,LOW(8)
	CALL __LSRD12
	MOV  R26,R30
	RCALL _SPI_transfer
; 0003 0040     SPI_transfer((unsigned char)(arg));
	LDD  R26,Y+2
	RCALL _SPI_transfer
; 0003 0041 
; 0003 0042     // transmit crc
; 0003 0043     SPI_transfer(crc|0x01);
	LDD  R30,Y+1
	ORI  R30,1
	MOV  R26,R30
	RCALL _SPI_transfer
; 0003 0044 
; 0003 0045     //wait response R1
; 0003 0046     res = SD_readRes1();
	RCALL _SD_readRes1
	MOV  R17,R30
; 0003 0047     /*
; 0003 0048     do {
; 0003 0049      res=SPI_transfer(0xFF);;
; 0003 004A      count++;
; 0003 004B     } while ( ((res&0x80)!=0x00)&&(count<0xff) );
; 0003 004C     */
; 0003 004D     return res;
	LDD  R17,Y+0
_0x2000009:
	ADIW R28,7
	RET
; 0003 004E }
; .FEND
;
;
;unsigned char SD_readRes1()
; 0003 0052 {
_SD_readRes1:
; .FSTART _SD_readRes1
; 0003 0053     unsigned char res1;
; 0003 0054     unsigned int i = 0;
; 0003 0055 
; 0003 0056     // keep polling until actual data received
; 0003 0057     //while((res1 = SPI_transfer(0xFF)) == 0xFF)
; 0003 0058     while(((res1 = SPI_transfer(0xFF))&0x80) != 0x00)
	CALL __SAVELOCR4
;	res1 -> R17
;	i -> R18,R19
	__GETWRN 18,19,0
_0x60009:
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	MOV  R17,R30
	ANDI R30,LOW(0x80)
	BREQ _0x6000B
; 0003 0059     {
; 0003 005A         i++;
	__ADDWRN 18,19,1
; 0003 005B 
; 0003 005C         // if no data received for (254)8 bytes, break
; 0003 005D         if(i > 0x1FF) break;
	__CPWRN 18,19,512
	BRLO _0x60009
; 0003 005E     }
_0x6000B:
; 0003 005F 
; 0003 0060     return res1;
	MOV  R30,R17
	CALL __LOADLOCR4
_0x2000008:
	ADIW R28,4
	RET
; 0003 0061 }
; .FEND
;
;unsigned char SD_goIdleState()
; 0003 0064 {
_SD_goIdleState:
; .FSTART _SD_goIdleState
; 0003 0065     unsigned char res1;
; 0003 0066     // assert chip select
; 0003 0067     SPI_transfer(0xFF);
	RCALL SUBOPT_0x20
;	res1 -> R17
; 0003 0068     CS_ENABLE();
; 0003 0069     //SPI_transfer(0xFF);
; 0003 006A 
; 0003 006B     // send CMD0
; 0003 006C     res1 = SD_command(CMD0, CMD0_ARG, CMD0_CRC);
	LDI  R30,LOW(0)
	ST   -Y,R30
	RCALL SUBOPT_0x2
	LDI  R26,LOW(148)
	RJMP _0x2000004
; 0003 006D 
; 0003 006E     // read response
; 0003 006F     //res1 = SD_readRes1();
; 0003 0070 
; 0003 0071     // deassert chip select
; 0003 0072     SPI_transfer(0xFF);
; 0003 0073     CS_DISABLE();
; 0003 0074     SPI_transfer(0xFF);
; 0003 0075 
; 0003 0076     return res1;
; 0003 0077 }
; .FEND
;
;void SD_readRes7(unsigned char *res)
; 0003 007A {
_SD_readRes7:
; .FSTART _SD_readRes7
; 0003 007B     // read response 1 in R7
; 0003 007C     //res[0] = SD_readRes1();
; 0003 007D 
; 0003 007E     // if error reading R1, return
; 0003 007F     if(res[0] > 1) return;
	RCALL SUBOPT_0x0
;	*res -> Y+0
	LD   R26,X
	CPI  R26,LOW(0x2)
	BRSH _0x2000007
; 0003 0080 
; 0003 0081     // read remaining bytes
; 0003 0082     res[1] = SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	__PUTB1SNS 0,1
; 0003 0083     res[2] = SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	__PUTB1SNS 0,2
; 0003 0084     res[3] = SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	__PUTB1SNS 0,3
; 0003 0085     res[4] = SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	__PUTB1SNS 0,4
; 0003 0086 }
	RJMP _0x2000007
; .FEND
;
;void SD_sendIfCond(unsigned char *res)
; 0003 0089 {
_SD_sendIfCond:
; .FSTART _SD_sendIfCond
; 0003 008A     // assert chip select
; 0003 008B     SPI_transfer(0xFF);
	RCALL SUBOPT_0x21
;	*res -> Y+0
; 0003 008C     CS_ENABLE();
; 0003 008D     //SPI_transfer(0xFF);
; 0003 008E 
; 0003 008F     // send CMD8
; 0003 0090     res[0]=SD_command(CMD8, CMD8_ARG, CMD8_CRC);
	LDI  R30,LOW(8)
	ST   -Y,R30
	__GETD1N 0x1AA
	CALL __PUTPARD1
	LDI  R26,LOW(134)
	RJMP _0x2000006
; 0003 0091 
; 0003 0092     // read response
; 0003 0093     SD_readRes7(res);
; 0003 0094 
; 0003 0095     // deassert chip select
; 0003 0096     SPI_transfer(0xFF);
; 0003 0097     CS_DISABLE();
; 0003 0098     SPI_transfer(0xFF);
; 0003 0099 }
; .FEND
;
;/*
;void SD_readRes3_7(unsigned char *res)
;{
;    // read R1
;    //res[0] = SD_readRes1();
;
;    // if error reading R1, return
;    if(res[0] > 1) return;
;
;    // read remaining bytes
;    res[1] = SPI_transfer(0xFF);
;    res[2] = SPI_transfer(0xFF);
;    res[3] = SPI_transfer(0xFF);
;    res[4] = SPI_transfer(0xFF);
;}
;*/
;
;void SD_readOCR(unsigned char *res)
; 0003 00AD {
_SD_readOCR:
; .FSTART _SD_readOCR
; 0003 00AE     // assert chip select
; 0003 00AF     SPI_transfer(0xFF);
	RCALL SUBOPT_0x21
;	*res -> Y+0
; 0003 00B0     CS_ENABLE();
; 0003 00B1     //SPI_transfer(0xFF);
; 0003 00B2 
; 0003 00B3     // send CMD58
; 0003 00B4     res[0] = SD_command(CMD58, CMD58_ARG, CMD58_CRC);
	LDI  R30,LOW(58)
	ST   -Y,R30
	RCALL SUBOPT_0x2
	LDI  R26,LOW(0)
_0x2000006:
	RCALL _SD_command
	LD   R26,Y
	LDD  R27,Y+1
	ST   X,R30
; 0003 00B5 
; 0003 00B6     // read response
; 0003 00B7     //SD_readRes3_7(res);
; 0003 00B8     SD_readRes7(res);
	RCALL _SD_readRes7
; 0003 00B9 
; 0003 00BA     // deassert chip select
; 0003 00BB     SPI_transfer(0xFF);
	RCALL SUBOPT_0x22
; 0003 00BC     CS_DISABLE();
; 0003 00BD     SPI_transfer(0xFF);
; 0003 00BE }
_0x2000007:
	ADIW R28,2
	RET
; .FEND
;
;unsigned char SD_sendApp()
; 0003 00C1 {
_SD_sendApp:
; .FSTART _SD_sendApp
; 0003 00C2     unsigned char res1;
; 0003 00C3     // assert chip select
; 0003 00C4     SPI_transfer(0xFF);
	RCALL SUBOPT_0x20
;	res1 -> R17
; 0003 00C5     CS_ENABLE();
; 0003 00C6     //SPI_transfer(0xFF);
; 0003 00C7 
; 0003 00C8     // send CMD0
; 0003 00C9     res1 = SD_command(CMD55, CMD55_ARG, CMD55_CRC);
	LDI  R30,LOW(55)
	ST   -Y,R30
	__GETD1N 0x0
	RJMP _0x2000003
; 0003 00CA 
; 0003 00CB     // read response
; 0003 00CC     //res1 = SD_readRes1();
; 0003 00CD 
; 0003 00CE     // deassert chip select
; 0003 00CF     SPI_transfer(0xFF);
; 0003 00D0     CS_DISABLE();
; 0003 00D1     SPI_transfer(0xFF);
; 0003 00D2 
; 0003 00D3     return res1;
; 0003 00D4 }
; .FEND
;
;unsigned char SD_sendOpCond()
; 0003 00D7 {
_SD_sendOpCond:
; .FSTART _SD_sendOpCond
; 0003 00D8     unsigned char res1;
; 0003 00D9     // assert chip select
; 0003 00DA     SPI_transfer(0xFF);
	RCALL SUBOPT_0x20
;	res1 -> R17
; 0003 00DB     CS_ENABLE();
; 0003 00DC     //SPI_transfer(0xFF);
; 0003 00DD 
; 0003 00DE     // send CMD0
; 0003 00DF     res1 =  SD_command(ACMD41, ACMD41_ARG, ACMD41_CRC);
	LDI  R30,LOW(41)
	ST   -Y,R30
	__GETD1N 0x40000000
_0x2000003:
	CALL __PUTPARD1
	LDI  R26,LOW(0)
_0x2000004:
	RCALL _SD_command
	MOV  R17,R30
; 0003 00E0 
; 0003 00E1     // read response
; 0003 00E2     //res1 = SD_readRes1();
; 0003 00E3 
; 0003 00E4     // deassert chip select
; 0003 00E5     SPI_transfer(0xFF);
	RCALL SUBOPT_0x22
; 0003 00E6     CS_DISABLE();
; 0003 00E7     SPI_transfer(0xFF);
; 0003 00E8 
; 0003 00E9     return res1;
	MOV  R30,R17
_0x2000005:
	LD   R17,Y+
	RET
; 0003 00EA }
; .FEND
;
;unsigned char SD_init()
; 0003 00ED {
_SD_init:
; .FSTART _SD_init
; 0003 00EE     unsigned char res[5], cmdAttempts = 0;
; 0003 00EF 
; 0003 00F0     SD_powerUpSeq();
	SBIW R28,5
	ST   -Y,R17
;	res -> Y+1
;	cmdAttempts -> R17
	LDI  R17,0
	RCALL _SD_powerUpSeq
; 0003 00F1 
; 0003 00F2     // command card to idle
; 0003 00F3     while((res[0] = SD_goIdleState()) != 0x01)
_0x6000E:
	RCALL _SD_goIdleState
	STD  Y+1,R30
	CPI  R30,LOW(0x1)
	BREQ _0x60010
; 0003 00F4     {
; 0003 00F5         //putchar('I');putchar('d');
; 0003 00F6         //putchar(res[0]);
; 0003 00F7         cmdAttempts++;
	SUBI R17,-1
; 0003 00F8         if(cmdAttempts > 100) return SD_ERROR;
	CPI  R17,101
	BRLO _0x60011
	LDI  R30,LOW(1)
	RJMP _0x2000002
; 0003 00F9     }
_0x60011:
	RJMP _0x6000E
_0x60010:
; 0003 00FA 
; 0003 00FB 
; 0003 00FC 
; 0003 00FD     // send interface conditions
; 0003 00FE     SD_sendIfCond(res);
	MOVW R26,R28
	ADIW R26,1
	RCALL _SD_sendIfCond
; 0003 00FF 
; 0003 0100     //printhex("If",res[0],1);
; 0003 0101 
; 0003 0102     if(res[0] != 0x01)
	LDD  R26,Y+1
	CPI  R26,LOW(0x1)
	BREQ _0x60012
; 0003 0103     {
; 0003 0104         return SD_ERROR;
	LDI  R30,LOW(1)
	RJMP _0x2000002
; 0003 0105     }
; 0003 0106 
; 0003 0107     // check echo pattern
; 0003 0108     if(res[4] != 0xAA)
_0x60012:
	LDD  R26,Y+5
	CPI  R26,LOW(0xAA)
	BREQ _0x60013
; 0003 0109     {
; 0003 010A         return SD_ERROR;
	LDI  R30,LOW(1)
	RJMP _0x2000002
; 0003 010B     }
; 0003 010C 
; 0003 010D     // attempt to initialize card
; 0003 010E     cmdAttempts = 0;
_0x60013:
	LDI  R17,LOW(0)
; 0003 010F     do
_0x60015:
; 0003 0110     {
; 0003 0111         if(cmdAttempts > 100) return SD_ERROR;
	CPI  R17,101
	BRLO _0x60017
	LDI  R30,LOW(1)
	RJMP _0x2000002
; 0003 0112 
; 0003 0113         // send app cmd
; 0003 0114         res[0] = SD_sendApp();
_0x60017:
	RCALL _SD_sendApp
	STD  Y+1,R30
; 0003 0115 
; 0003 0116         // if no error in response
; 0003 0117         if(res[0] < 2)
	LDD  R26,Y+1
	CPI  R26,LOW(0x2)
	BRSH _0x60018
; 0003 0118         {
; 0003 0119             res[0] = SD_sendOpCond();
	RCALL _SD_sendOpCond
	STD  Y+1,R30
; 0003 011A         }
; 0003 011B 
; 0003 011C         // wait
; 0003 011D         if(res[0] != SD_READY)
_0x60018:
	LDD  R30,Y+1
	CPI  R30,0
	BREQ _0x60019
; 0003 011E             delay_ms(10);
	LDI  R26,LOW(10)
	LDI  R27,0
	CALL _delay_ms
; 0003 011F 
; 0003 0120         cmdAttempts++;
_0x60019:
	SUBI R17,-1
; 0003 0121     }
; 0003 0122     while(res[0] != SD_READY);
	LDD  R30,Y+1
	CPI  R30,0
	BRNE _0x60015
; 0003 0123 
; 0003 0124     // read OCR
; 0003 0125     SD_readOCR(res);
	MOVW R26,R28
	ADIW R26,1
	RCALL _SD_readOCR
; 0003 0126 
; 0003 0127     // check card is ready
; 0003 0128     if(!(res[1] & 0x80)) return SD_ERROR;
	LDD  R30,Y+2
	ANDI R30,LOW(0x80)
	BRNE _0x6001A
	LDI  R30,LOW(1)
	RJMP _0x2000002
; 0003 0129 
; 0003 012A     return SD_SUCCESS;
_0x6001A:
	LDI  R30,LOW(0)
_0x2000002:
	LDD  R17,Y+0
	ADIW R28,6
	RET
; 0003 012B }
; .FEND
;
;#define CMD17                   17
;#define CMD17_CRC               0x00
;#define SD_MAX_READ_ATTEMPTS    1563
;
;/*******************************************************************************
; Read single 512 byte block
; token = 0xFE - Successful read
; token = 0x0X - Data error
; token = 0xFF - Timeout
;*******************************************************************************/
;unsigned char SD_readSingleBlock(unsigned long addr, unsigned char *buf, unsigned char *token)
; 0003 0138 {
_SD_readSingleBlock:
; .FSTART _SD_readSingleBlock
; 0003 0139     unsigned char res1, read;
; 0003 013A     unsigned int i, readAttempts;
; 0003 013B     addr*=512UL;
	ST   -Y,R27
	ST   -Y,R26
	CALL __SAVELOCR6
;	addr -> Y+10
;	*buf -> Y+8
;	*token -> Y+6
;	res1 -> R17
;	read -> R16
;	i -> R18,R19
;	readAttempts -> R20,R21
	__GETD1S 10
	__GETD2N 0x200
	CALL __MULD12U
	__PUTD1S 10
; 0003 013C     // set token to none
; 0003 013D     *token = 0xFF;
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(255)
	ST   X,R30
; 0003 013E 
; 0003 013F     // assert chip select
; 0003 0140     SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
; 0003 0141     CS_ENABLE();
	CBI  0x5,2
; 0003 0142     //SPI_transfer(0xFF);
; 0003 0143 
; 0003 0144     // send CMD17
; 0003 0145     res1 = SD_command(CMD17, addr, CMD17_CRC);
	LDI  R30,LOW(17)
	ST   -Y,R30
	__GETD1S 11
	CALL __PUTPARD1
	LDI  R26,LOW(0)
	RCALL _SD_command
	MOV  R17,R30
; 0003 0146 
; 0003 0147     // read R1
; 0003 0148     //res1 = SD_readRes1();
; 0003 0149 
; 0003 014A     // if response received from card
; 0003 014B     if(res1 != 0xFF)
	CPI  R17,255
	BREQ _0x6001B
; 0003 014C     {
; 0003 014D         // wait for a response token (timeout = 100ms)
; 0003 014E         readAttempts = 0;
	__GETWRN 20,21,0
; 0003 014F         while(++readAttempts != SD_MAX_READ_ATTEMPTS)
_0x6001C:
	MOVW R30,R20
	ADIW R30,1
	MOVW R20,R30
	CPI  R30,LOW(0x61B)
	LDI  R26,HIGH(0x61B)
	CPC  R31,R26
	BREQ _0x6001E
; 0003 0150             if((read = SPI_transfer(0xFF)) != 0xFF) break;
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	MOV  R16,R30
	CPI  R30,LOW(0xFF)
	BREQ _0x6001C
; 0003 0151 
; 0003 0152         // if response token is 0xFE
; 0003 0153         if(read == 0xFE)
_0x6001E:
	CPI  R16,254
	BRNE _0x60020
; 0003 0154         {
; 0003 0155             // read 512 byte block
; 0003 0156             for(i = 0; i < 512; i++) *buf++ = SPI_transfer(0xFF);
	__GETWRN 18,19,0
_0x60022:
	__CPWRN 18,19,512
	BRSH _0x60023
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	SBIW R30,1
	PUSH R31
	PUSH R30
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	POP  R26
	POP  R27
	ST   X,R30
	__ADDWRN 18,19,1
	RJMP _0x60022
_0x60023:
; 0003 0159 SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
; 0003 015A             SPI_transfer(0xFF);
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
; 0003 015B         }
; 0003 015C 
; 0003 015D         // set token to card response
; 0003 015E         *token = read;
_0x60020:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ST   X,R16
; 0003 015F     }
; 0003 0160 
; 0003 0161     // deassert chip select
; 0003 0162     SPI_transfer(0xFF);
_0x6001B:
	RCALL SUBOPT_0x22
; 0003 0163     CS_DISABLE();
; 0003 0164     SPI_transfer(0xFF);
; 0003 0165     if(read==0xFE)
	CPI  R16,254
	BRNE _0x60024
; 0003 0166         return res1;
	MOV  R30,R17
	RJMP _0x2000001
; 0003 0167     else
_0x60024:
; 0003 0168         return SD_ERROR;
	LDI  R30,LOW(1)
; 0003 0169 }
_0x2000001:
	CALL __LOADLOCR6
	ADIW R28,14
	RET
; .FEND
;
;
;#ifdef PRINT_DEBUG
;void putchar(char c)
;{
;    while ((UCSR0A & (1<<UDRE0))==0);
;    UDR0=c;
;}
;
;void printhex(flash char *str,unsigned long var,unsigned char len)
;{
;    unsigned char value;
;    putstr(str);
;    putstr(":0x");
;    for(;len>0;len--)
;    {
;        //0x12345678  len=4
;        value=(var>>((len-1)*8)+4)&0x0F;
;        if(value<10){
;            putchar(value+'0');
;        }
;        else{
;            putchar(value-10+'A');
;        }
;        value=(var>>((len-1)*8))&0x0F;
;        if(value<10){
;            putchar(value+'0');
;        }
;        else{
;            putchar(value-10+'A');
;        }
;    }
;    putchar('\r');
;}
;
;void putstr(flash char *str)
;{
;    while(*str){
;        putchar(*str++);
;    }
;}
;void USART_Flush( void )
;{
;    unsigned char dummy;
;    while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
;}
;
;unsigned char recchar(void)
;{
;    //unsigned char delay=0;
;    // Wait for data to be received
;    //while ( !(UCSR0A & (1<<RXC0)) && ++delay);
;    if( !(UCSR0A & (1<<RXC0)))
;        return 0;
;    else
;        return UDR0;
;}
;#endif

	.DSEG
_result:
	.BYTE 0x5
_sdBuf:
	.BYTE 0x200
_testBuf:
	.BYTE 0x80
_appStartAdr:
	.BYTE 0x4
_adr:
	.BYTE 0x4
_SectorsPerFat:
	.BYTE 0x4
_fat_begin_lba:
	.BYTE 0x4
_cluster_begin_lba:
	.BYTE 0x4
_fat_file_adr:
	.BYTE 0x4
_fat_file_next_adr:
	.BYTE 0x4
_filesize:
	.BYTE 0x4
_readbytes:
	.BYTE 0x4
_checksumCnt:
	.BYTE 0x2
_app_pointer:
	.BYTE 0x2
_partition_type:
	.BYTE 0x1

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x0:
	ST   -Y,R27
	ST   -Y,R26
	LD   R26,Y
	LDD  R27,Y+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1:
	SBI  0x5,1
	SBI  0x5,0
	__CALL1MN _app_pointer,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2:
	__GETD1N 0x0
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:42 WORDS
SUBOPT_0x3:
	LDI  R30,LOW(_sdBuf)
	LDI  R31,HIGH(_sdBuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(5)
	LDI  R27,HIGH(5)
	RCALL _SD_readSingleBlock
	STS  _result,R30
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x4:
	STS  _adr,R30
	STS  _adr+1,R31
	STS  _adr+2,R22
	STS  _adr+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x5:
	LDS  R30,_adr
	LDS  R31,_adr+1
	LDS  R22,_adr+2
	LDS  R23,_adr+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6:
	CALL __PUTPARD1
	RJMP SUBOPT_0x3

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x7:
	__POINTW1MN _sdBuf,14
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(2)
	RJMP _buf2num

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x8:
	LDS  R26,_fat_begin_lba
	LDS  R27,_fat_begin_lba+1
	LDS  R24,_fat_begin_lba+2
	LDS  R25,_fat_begin_lba+3
	CALL __ADDD12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x9:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R26,LOW(2)
	RJMP _buf2num

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xA:
	STS  _SectorsPerFat,R30
	STS  _SectorsPerFat+1,R31
	STS  _SectorsPerFat+2,R22
	STS  _SectorsPerFat+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB:
	MOV  R30,R4
	MOVW R26,R16
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC:
	RCALL SUBOPT_0x5
	RJMP SUBOPT_0x6

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0xD:
	ST   -Y,R31
	ST   -Y,R30
	__MULBNWRU 18,19,32
	SUBI R30,LOW(-_sdBuf)
	SBCI R31,HIGH(-_sdBuf)
	MOVW R26,R30
	RCALL _compbuf
	__PUTB1MN _result,1
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE:
	__MULBNWRU 18,19,32
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xF:
	CALL __LSLD16
	STS  _fat_file_adr,R30
	STS  _fat_file_adr+1,R31
	STS  _fat_file_adr+2,R22
	STS  _fat_file_adr+3,R23
	RJMP SUBOPT_0xE

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x10:
	LDS  R26,_fat_file_adr
	LDS  R27,_fat_file_adr+1
	LDS  R24,_fat_file_adr+2
	LDS  R25,_fat_file_adr+3
	CALL __ORD12
	STS  _fat_file_adr,R30
	STS  _fat_file_adr+1,R31
	STS  _fat_file_adr+2,R22
	STS  _fat_file_adr+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x11:
	LDI  R26,LOW(_adr)
	LDI  R27,HIGH(_adr)
	CALL __GETD1P_INC
	__SUBD1N -1
	CALL __PUTDP1_DEC
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x12:
	MOVW R30,R6
	LDS  R26,_cluster_begin_lba
	LDS  R27,_cluster_begin_lba+1
	LDS  R24,_cluster_begin_lba+2
	LDS  R25,_cluster_begin_lba+3
	CLR  R22
	CLR  R23
	CALL __ADDD12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x13:
	LDS  R30,_fat_file_adr
	LDS  R31,_fat_file_adr+1
	LDS  R22,_fat_file_adr+2
	LDS  R23,_fat_file_adr+3
	__SUBD1N 2
	MOVW R26,R30
	MOVW R24,R22
	MOV  R30,R4
	LDI  R31,0
	CALL __CWD1
	CALL __MULD12U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x14:
	LDS  R26,_fat_file_adr
	LDS  R27,_fat_file_adr+1
	LDS  R24,_fat_file_adr+2
	LDS  R25,_fat_file_adr+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x15:
	__CPD2N 0xFFFFFFF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x16:
	__CPD2N 0xFFFF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x17:
	LDS  R30,_readbytes
	LDS  R31,_readbytes+1
	LDS  R22,_readbytes+2
	LDS  R23,_readbytes+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x18:
	MOVW R22,R30
	LDI  R26,LOW(_sdBuf)
	LDI  R27,HIGH(_sdBuf)
	ADD  R26,R18
	ADC  R27,R19
	LD   R30,X
	LSL  R30
	MOV  R0,R30
	LDI  R26,LOW(_sdBuf)
	LDI  R27,HIGH(_sdBuf)
	ADD  R26,R18
	ADC  R27,R19
	LD   R30,X
	ROL  R30
	LDI  R30,0
	ROL  R30
	OR   R30,R0
	MOVW R26,R22
	ST   X,R30
	MOVW R26,R18
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x19:
	LDS  R26,_readbytes
	LDS  R27,_readbytes+1
	LDS  R24,_readbytes+2
	LDS  R25,_readbytes+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x1A:
	STS  _appStartAdr,R30
	STS  _appStartAdr+1,R31
	STS  _appStartAdr+2,R22
	STS  _appStartAdr+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x1B:
	LDS  R26,_appStartAdr
	LDS  R27,_appStartAdr+1
	LDS  R24,_appStartAdr+2
	LDS  R25,_appStartAdr+3
	CALL __ORD12
	RJMP SUBOPT_0x1A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1C:
	LDS  R26,_fat_file_next_adr
	LDS  R27,_fat_file_next_adr+1
	LDS  R24,_fat_file_next_adr+2
	LDS  R25,_fat_file_next_adr+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x1D:
	STS  124,R30
	__DELAY_USB 53
	LDS  R30,122
	ORI  R30,0x40
	STS  122,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1E:
	SBI  0x5,2
	LDI  R26,LOW(255)
	RJMP _SPI_transfer

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1F:
	MOV  R26,R30
	RCALL _SPI_transfer
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x20:
	ST   -Y,R17
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	CBI  0x5,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x21:
	ST   -Y,R27
	ST   -Y,R26
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	CBI  0x5,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x22:
	LDI  R26,LOW(255)
	RCALL _SPI_transfer
	RJMP SUBOPT_0x1E


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x7D0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__ADDD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	ADC  R23,R25
	RET

__ORD12:
	OR   R30,R26
	OR   R31,R27
	OR   R22,R24
	OR   R23,R25
	RET

__LSLD12:
	TST  R30
	MOV  R0,R30
	MOVW R30,R26
	MOVW R22,R24
	BREQ __LSLD12R
__LSLD12L:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R0
	BRNE __LSLD12L
__LSLD12R:
	RET

__LSRD12:
	TST  R30
	MOV  R0,R30
	MOVW R30,R26
	MOVW R22,R24
	BREQ __LSRD12R
__LSRD12L:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R0
	BRNE __LSRD12L
__LSRD12R:
	RET

__LSLW4:
	LSL  R30
	ROL  R31
__LSLW3:
	LSL  R30
	ROL  R31
__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__LSLD1:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	RET

__LSRD16:
	MOV  R30,R22
	MOV  R31,R23
	LDI  R22,0
	LDI  R23,0
	RET

__LSLD16:
	MOV  R22,R30
	MOV  R23,R31
	LDI  R30,0
	LDI  R31,0
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__MULD12U:
	MUL  R23,R26
	MOV  R23,R0
	MUL  R22,R27
	ADD  R23,R0
	MUL  R31,R24
	ADD  R23,R0
	MUL  R30,R25
	ADD  R23,R0
	MUL  R22,R26
	MOV  R22,R0
	ADD  R23,R1
	MUL  R31,R27
	ADD  R22,R0
	ADC  R23,R1
	MUL  R30,R24
	ADD  R22,R0
	ADC  R23,R1
	CLR  R24
	MUL  R31,R26
	MOV  R31,R0
	ADD  R22,R1
	ADC  R23,R24
	MUL  R30,R27
	ADD  R31,R0
	ADC  R22,R1
	ADC  R23,R24
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	ADC  R22,R24
	ADC  R23,R24
	RET

__GETD1P_INC:
	LD   R30,X+
	LD   R31,X+
	LD   R22,X+
	LD   R23,X+
	RET

__PUTDP1_DEC:
	ST   -X,R23
	ST   -X,R22
	ST   -X,R31
	ST   -X,R30
	RET

__GETD1S0:
	LD   R30,Y
	LDD  R31,Y+1
	LDD  R22,Y+2
	LDD  R23,Y+3
	RET

__GETD2S0:
	LD   R26,Y
	LDD  R27,Y+1
	LDD  R24,Y+2
	LDD  R25,Y+3
	RET

__PUTD1S0:
	ST   Y,R30
	STD  Y+1,R31
	STD  Y+2,R22
	STD  Y+3,R23
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__CPD10:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	RET

__CPD21:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R25,R23
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
