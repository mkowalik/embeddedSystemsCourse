/*
 * addTables.asm
 *
 *  Created: 2014-10-22 17:10:28
 *   Author: Kowalik
 */ 
 
#include "m32def.inc"

; dlugosc liczb w bajtach do dodania
.EQU LEN = 16

; rezerwujê pamiec dla tablic/liczb ktore bede dodawal
.DSEG
tab1: .BYTE LEN
tab1end:
tab2: .BYTE LEN
tab2end:

.CSEG
; ustawiamy miejsce w pamiêci na 0 gdzie bêdzie umieszczony kod poni¿ej
.ORG 0 

; X - adres tablicy do ktorej kopiujemy
; Y - adres tablicy z ktorej kopiujemy
	LDI XL, low(tab1end-1)
	LDI XH, high(tab1end-1)
	LDI YL, low(tab2end-1)
	LDI YH, high(tab2end-1)

; Z - licznik ile pozosta³o komórek do dodania
	LDI ZL, low(LEN)
	LDI ZH, high(LEN)

; zerujê rejestr w któym zapamiêtam rejestr stanu po pierwszej operacji dodawania
	LDI r16, 0

loopBegin:
	CPI	ZL, 0 ; jeœli ZL!=0, to wykonaj pêtlê, jeœli nie to sprawdŸ ZH
	BRNE bodyOfLoop 
innerCheck:
	CPI ZH, 0 ; jeœli ZH!=0, to wykonaj pêtlê, jeœni nie to zakoñcz pêtlê
	BREQ endOfLoop
bodyOfLoop:
	
; g³ówna czêœæ dodawania, z uwzglêdnieniem przepe³nienia z poprzedniej operacji
	out SREG, r16 ; przywracam wartoœæ rejestru stanu z poprzedniej operacji
	LD r0, X
	LD r1, Y
	ADC r0, r1 ; dodajê to co pod adresem Y do tego co pod adresem X
	ST X, r0 ; zapisujê wynik do komórki pod adresem X
	in r16, SREG ; zapisujê rejestr stanu. Operacja ST nie modyfikuje go, wiêc mogê to zrobiæ teraz

; odejmujê 1 od pary XL, XH, od XL normalnie, a od XH tylko jeœli by³a ustawiona carry flag - wynik dodawania
	SUBI XL, 1
	SBCI XH, 0

; odejmujê 1 od pary YL, YH, j.w. - sk³adnik dodawania
	SUBI YL, 1
	SBCI YH, 0

; odejmujê 1 od pary ZL, XHm j.w. - iterator
	SUBI ZL, 1
	SBCI ZH, 0

; powrót na pocz¹tek pêtli
	RJMP loopBegin


; w tym momencie wynik powinien byæ w pamiêci o adresie X


endOfLoop: