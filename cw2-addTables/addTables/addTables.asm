/*
 * addTables.asm
 *
 *  Created: 2014-10-22 17:10:28
 *   Author: Kowalik
 */ 
 
#include "m32def.inc"

; dlugosc liczb w bajtach do dodania
.EQU LEN = 16

; rezerwuj� pamiec dla tablic/liczb ktore bede dodawal
.DSEG
tab1: .BYTE LEN
tab1end:
tab2: .BYTE LEN
tab2end:

.CSEG
; ustawiamy miejsce w pami�ci na 0 gdzie b�dzie umieszczony kod poni�ej
.ORG 0 

; X - adres tablicy do ktorej kopiujemy
; Y - adres tablicy z ktorej kopiujemy
	LDI XL, low(tab1end-1)
	LDI XH, high(tab1end-1)
	LDI YL, low(tab2end-1)
	LDI YH, high(tab2end-1)

; Z - licznik ile pozosta�o kom�rek do dodania
	LDI ZL, low(LEN)
	LDI ZH, high(LEN)

; zeruj� rejestr w kt�ym zapami�tam rejestr stanu po pierwszej operacji dodawania
	LDI r16, 0

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
	SUBI XL, 1
	SBCI XH, 0

; odejmuj� 1 od pary YL, YH, j.w. - sk�adnik dodawania
	SUBI YL, 1
	SBCI YH, 0

; odejmuj� 1 od pary ZL, XHm j.w. - iterator
	SUBI ZL, 1
	SBCI ZH, 0

; powr�t na pocz�tek p�tli
	RJMP loopBegin


; w tym momencie wynik powinien by� w pami�ci o adresie X


endOfLoop: