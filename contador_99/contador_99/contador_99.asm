; ------------------------------------------------------------------------------
; Project:	Title of the project goes here
; File:		xxx.asm
; Author:	Author Name
; Created:	2017-02-18
; Modified:	2017-02-18
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
.def	cont1			= R17
.def	cont2			= R18
.def	display			= R19
		
.equ	ledsDdr			= DDRD
.equ	regSaida		= PORTD
.equ	saidaSerial		= PD0


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
	LDI		auxReg, 0xFF			; Todos os bits como saida
	OUT		ledsDdr, auxReg
mainLoop:

	PUSH	cont2
	CALL	decodificar
	;POP	display

	;PUSH	display
	CALL	func595
	POP		display

	PUSH	cont1
	CALL	decodificar
	;POP	display

	;PUSH	display
	CALL	func595
	POP		display

	INC		cont2

	CPI		cont2, 10
	BREQ	contador2
	jmp		contar

	contador2:
	clr		cont2
	
	inc		cont1

	cpi		cont1, 10
	brne	contar
	
	clr		cont1

contar:	

	CALL	delay100ms

	JMP		mainLoop

; ------------------------------------------------------------------------------
; Subroutine:	decodificar
; ------------------------------------------------------------------------------
decodificar:
	IN		R31, SPH
	IN		R30, SPL
	LDD		auxReg, Z+3
	cpi		auxReg, 0
	breq	zero
	CPI		auxReg, 1
	breq	um
	CPI		auxReg, 2
	breq	dois
	CPI		auxReg, 3
	breq	tres
	cpi		auxReg, 4
	breq	quatro
	cpi		auxReg, 5
	breq	cinco
	cpi		auxReg, 6
	breq	seis
	cpi		auxReg, 7
	breq	sete
	cpi		auxReg, 8
	breq	oito
	cpi		auxReg, 9
	breq	nove
	JMP		fim_decodificar
	
	zero:
	LDI		auxReg, 0b11000000
	jmp fim_decodificar

	um:
	LDI		auxReg, 0b11111001
	jmp fim_decodificar

	dois:
	LDI		auxReg, 0b10100100
	jmp fim_decodificar

	tres:
	LDI		auxReg, 0b10110000
	jmp fim_decodificar

	quatro:
	LDI		auxReg, 0b10011001
	jmp fim_decodificar

	cinco:
	LDI		auxReg, 0b10010010
	jmp fim_decodificar

	seis:
	LDI		auxReg, 0b10000011
	jmp fim_decodificar

	sete:
	LDI		auxReg, 0b11111000
	jmp fim_decodificar

	oito:
	LDI		auxReg, 0b10000000
	jmp		fim_decodificar

	nove:
	LDI		auxReg, 0b10011000
	jmp		fim_decodificar

fim_decodificar:
	STD		Z+3, auxReg

	RET


; ------------------------------------------------------------------------------
; Subroutine:	func595
; ------------------------------------------------------------------------------

func595:
	IN		R31, SPH
	IN		R30, SPL
	LDD		display, Z+3
	LDI		auxReg, 9

volta2:
	DEC		auxReg
	BREQ    sair
	LSL		display
	BRCS	carry
	CBI		regSaida, saidaSerial
continue:
	SBI		regSaida, 1
	CBI		regSaida, 1
	jmp		volta2
sair:
	SBI		regSaida, 2
	CBI		regSaida, 2
	OUT		SPH, R31
	OUT		SPL, R30
	RET

carry:
	SBI		regSaida, saidaSerial

	JMP		continue

	RET


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