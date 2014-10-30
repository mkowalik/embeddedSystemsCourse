/*
 * AddTablesProcedure.asm
 *
 *  Created: 2014-10-29 16:20:02
 *   Author: Patrycja, Igor i Micha�
 */ 

; dlugosc liczb w bajtach do dodania
.EQU LEN = 16

; rezerwuj� pamiec dla tablic/liczb ktore bede dodawal
.DSEG
tab1: .BYTE LEN
tab2: .BYTE LEN

.CSEG
; ustawiamy miejsce w pami�ci na 0 gdzie b�dzie umieszczony kod poni�ej
; ustawiamy wektor przerwa�
.ORG 0 
	jmp START
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ
	jmp NO_IRQ

START:
; inicjujemy stos
	ldi r16,high(RAMEND)
	out SPH,r16
	ldi r16,low(RAMEND)
	out SPL,r16

; X - adres tablicy do ktorej kopiujemy
; Y - adres tablicy z ktorej kopiujemy
	LDI XL, low(tab1)
	LDI XH, high(tab1)
	LDI YL, low(tab2)
	LDI YH, high(tab2)

; Z - licznik ile pozosta�o kom�rek do dodania
	LDI ZL, low(LEN)
	LDI ZH, high(LEN)

; wywolujemy procedure DODAJ
	CALL DODAJ

; wirtualna procedura na brak obslugi przerwania
NO_IRQ:
	reti

DODAJ:
; void DODAJ (&L1, &L2, LEN)
; L1 = rejestr X, 
; L2 = rejestr Y, 
; LEN = rejestr Z

; odk�adamy na stos wszystkie p�niej u�ywane rejestry
	PUSH r16
	PUSH r17
	PUSH r18
	PUSH r0
	PUSH r1

; zeruj� rejestr w kt�ym zapami�tam rejestr stanu po pierwszej operacji dodawania
	LDI r16, 0
; rejestry pomocnicze wykorzystywane do dodawania
	LDI r17, 1
	LDI r18, 0

loopBegin:
	CPI	ZL, 0 ; je�li ZL!=0, to wykonaj p�tl�, je�li nie to sprawd� ZH
	BRNE bodyOfLoop 
innerCheck:
	CPI ZH, 0 ; je�li ZH!=0, to wykonaj p�tl�, je�ni nie to zako�cz p�tl�
	BREQ endOfLoop
bodyOfLoop:
	
; g��wna cz�� dodawania, z uwzgl�dnieniem przepe�nienia z poprzedniej operacji
	out SREG, r16 ; przywracam warto�� rejestru stanu z poprzedniej operacji
	LD r0, X
	LD r1, Y
	ADC r0, r1 ; dodaj� to co pod adresem Y do tego co pod adresem X
	ST X, r0 ; zapisuj� wynik do kom�rki pod adresem X
	in r16, SREG ; zapisuj� rejestr stanu. Operacja ST nie modyfikuje go, wi�c mog� to zrobi� teraz

; odejmuj� 1 od pary XL, XH, od XL normalnie, a od XH tylko je�li by�a ustawiona carry flag - wynik dodawania
	ADD XL, r17
	ADC XH, r18

; odejmuj� 1 od pary YL, YH, j.w. - sk�adnik dodawania
	ADD YL, r17
	ADC YH, r18

; odejmuj� 1 od pary ZL, XHm j.w. - iterator
	SUBI ZL, 1
	SBCI ZH, 0

; powr�t na pocz�tek p�tli
	RJMP loopBegin


; w tym momencie wynik powinien by� w pami�ci o adresie X

endOfLoop:
; przywracamy wszystkie uzywane rejestry wczesniej zapisane na stos
	POP r1
	POP r0
	POP r18
	POP r17
	POP r16
; powr�t z funkcji
	RET
