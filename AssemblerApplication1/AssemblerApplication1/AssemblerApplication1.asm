; ------------------------------------------------------------------------------
; Project:	Assembly Led Animation
; File:		assemblyLedAnimation.asm
; Author:	Enzzo Comassetto dos Santos
; Created:	11/08/2017
; Modified:	11/08/2018
; Version:	1.0
; Notes:	What this project does and the clock frequency.
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Include definition files
; ------------------------------------------------------------------------------
.include "m328pdef.inc"

; ------------------------------------------------------------------------------
; Register definitions
; ------------------------------------------------------------------------------
.def	auxReg			= R16
.def	ledsValue		= R17
.def	flags			= R18

.equ	paused			= 0
.equ	ledsDdr			= DDRD
.equ	ledsPort		= PORTD

.equ	buttonDdr		= DDRB
.equ	buttonPort		= PORTB
.equ	buttonPin		= PINB
.equ	ButtonBit		= PB0		

; ------------------------------------------------------------------------------
; Interruption handlers
; ------------------------------------------------------------------------------
.org 0x0000
	JMP resetHandler
.org INT0addr
	JMP int0Handler
.org INT1addr
	JMP int1Handler
.org PCI0addr
	JMP pcint0Handler
.org PCI1addr
	JMP pcint1Handler
.org PCI2addr
	JMP pcint2Handler
.org WDTaddr
	JMP wdtHandler
.org OC2Aaddr
	JMP timer2CompAHandler
.org OC2Baddr
	JMP timer2CompBHandler
.org OVF2addr
	JMP timer2OvfHandler
.org ICP1addr
	JMP timer1CaptureHandler
.org OC1Aaddr
	JMP timer1CompAHandler
.org OC1Baddr
	JMP timer1CompBHandler
.org OVF1addr
	JMP timer1OvfHandler
.org OC0Aaddr
	JMP timer0CompAHandler
.org OC0Baddr
	JMP timer0CompBHandler
.org OVF0addr
	JMP timer0OvfHandler
.org SPIaddr
	JMP spiHandler
.org URXCaddr
	JMP usartRxHandler
.org UDREaddr
	JMP usartDataEmptyHandler
.org UTXCaddr
	JMP usartTxHandler
.org ADCCaddr
	JMP adcHandler
.org ERDYaddr
	JMP eepromHandler
.org ACIaddr
	JMP analogCompHandler
.org TWIaddr
	JMP twiHandler
.org SPMRaddr
	JMP flashHandler
.org INT_VECTORS_SIZE

; ------------------------------------------------------------------------------
; Include other assembly files
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Constants stored in Flash memory
; Note: Variables must be multiples of 2, since memory is organized in 16 bits
; ------------------------------------------------------------------------------

; ------------------------------------------------------------------------------
; Main code
; ------------------------------------------------------------------------------
main:
	LDI		auxReg, 0xFF	; Todos os bits como saida
	OUT		ledsDdr	, auxReg	;
	CBI		buttonDdr, buttonBit
	SBI		buttonPort, buttonBit
	LDI		ledsValue, 0x01 
	OUT		PORTD, ledsValue
mainLoop:
	SBIS	buttonPin, buttonBit
	CALL	processButton

	mov		auxReg, flags
	andi	auxReg, 1 << paused
	BRNE	animationEnd

	CALL	delay100ms
	LSL		ledsValue
	CPI		ledsValue, 0x00
	BRNE	show
	ldi		ledsValue, 0x01
show:
	OUT		ledsPort, ledsValue
	JMP		mainLoop
animationEnd:
	JMP		mainLoop

; ------------------------------------------------------------------------------
; Subroutine:	delay100ms
; ------------------------------------------------------------------------------
delay100ms:

	PUSH	R18
	PUSH	R19
	PUSH	R20
	LDI		R18, 9
	LDI		R19, 30
	LDI		R20, 221
delay100msLoop:
    DEC		R20
    BRNE	delay100msLoop
    DEC		R19
    BRNE	delay100msLoop
    DEC		R18
    BRNE	delay100msLoop
	POP		R20
	POP		R19
	POP		R18
	NOP
	NOP
	NOP
	RET

; ------------------------------------------------------------------------------
; Subroutine:	processButton
; ------------------------------------------------------------------------------
processButton:
	call	delayBounce
processButtonLoop:
	SBIS	buttonPin, buttonBit
	JMP		processButtonLoop
	LDI		auxReg, 1 << paused
	EOR		flags, auxReg
	call	delayBounce
	
	RET
; ------------------------------------------------------------------------------
; Subroutine:	delayBounce
; ------------------------------------------------------------------------------
delayBounce:

	PUSH	R18
	PUSH	R19
	PUSH	R20
	LDI		R18, 1
	LDI		R19, 160
	LDI		R20, 139
delayBounceLoop:
    DEC		R20
    BRNE	delayBounceLoop
    DEC		R19
    BRNE	delayBounceLoop
    DEC		R18
    BRNE	delayBounceLoop
	POP		R20
	POP		R19
	POP		R18
	NOP
	NOP
	NOP
	RET

; ------------------------------------------------------------------------------
; Interrupt handlers
; ------------------------------------------------------------------------------
int0Handler:
int1Handler:
pcint0Handler:
pcint1Handler:
pcint2Handler:
wdtHandler:
timer2CompAHandler:
timer2CompBHandler:
timer2OvfHandler:
timer1CaptureHandler:
timer1CompAHandler:
timer1CompBHandler:
timer1OvfHandler:
timer0CompAHandler:
timer0CompBHandler:
timer0OvfHandler:
spiHandler:
usartRxHandler:
usartDataEmptyHandler:
usartTxHandler:
adcHandler:
eepromHandler:
analogCompHandler:
twiHandler:
flashHandler:
resetHandler:
	LDI		R19, LOW(RAMEND)
	OUT		SPL, R19
	LDI		R19, HIGH(RAMEND)
	OUT		SPH,R19
	JMP		main