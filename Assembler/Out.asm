.586

.MODEL FLAT, STDCALL

INCLUDELIB		 libucrt.lib
INCLUDELIB		 kernel32.lib
INCLUDELIB		 "..\Debug\STDLIB.lib"
ExitProcess		 PROTO : DWORD

.STACK 4096

printerror		PROTO : DWORD
printnum		PROTO : SDWORD
stringcopy		 PROTO : DWORD, : DWORD
printstr	PROTO : DWORD
strconcat	PROTO : DWORD, : DWORD
stringcompare	 PROTO : DWORD, : DWORD
sumsymbol	PROTO : DWORD, : DWORD
tonumber	PROTO : DWORD
stringlength	PROTO : DWORD
indexof	PROTO : DWORD, : DWORD
userstringcompare	PROTO : DWORD, : DWORD
fii	PROTO : SDWORD, : SDWORD
fss	PROTO : DWORD, : DWORD

.CONST
	Ls000	BYTE	14, " is less than "
	Ls001	BYTE	8, " equals "
	Ls002	BYTE	16, " is bigger than "
	Li003	SDWORD	1
	Li004	SDWORD	5
	Ls005	BYTE	10, "1234567890"
	Ls006	BYTE	12, "123456789  0"

.DATA
	fii@x	SDWORD	0
	fii@y	SDWORD	0
	fii@z	SDWORD	0
	fss@a	BYTE	256 DUP(0)
	fss@b	BYTE	256 DUP(0)
	fss@c	BYTE	256 DUP(0)
	main@x	SDWORD	0
	main@y	SDWORD	0
	main@sa	BYTE	256 DUP(0)
	main@sb	BYTE	256 DUP(0)
	main@sc	BYTE	256 DUP(0)

.CODE
main PROC
;-------------llv-------------
	PUSH	Li003
	PUSH	Li003
	POP 	EDX
	POP 	EAX
	ADD 	EAX, EDX
	PUSH	EAX
	CALL	printnum
;-------------l-------------
	PUSH	Li003
	POP 	EAX
	MOV 	main@x, EAX
;-------------l-------------
	PUSH	Li004
	POP 	EAX
	MOV 	main@y, EAX
;-------------l-------------
	PUSH	OFFSET	Ls005
	PUSH	OFFSET	main@sa
	CALL	stringcopy
;-------------l-------------
	PUSH	OFFSET	Ls006
	PUSH	OFFSET	main@sb
	CALL	stringcopy
;-------------ii@i-------------
	PUSH	main@y
	PUSH	main@x
	CALL	fii
	PUSH	EAX
	CALL	printnum
;-------------ii@i-------------
	PUSH	OFFSET	main@sb
	PUSH	OFFSET	main@sa
	CALL	fss
	PUSH	EAX
	PUSH	OFFSET	main@sc
	CALL	stringcopy
;-------------i-------------
	PUSH	OFFSET	main@sc
	CALL	printstr
;-------------i@i-------------
	PUSH	OFFSET	main@sc
	CALL	stringlength
	PUSH	EAX
	CALL	printnum
;-------------ii@i-------------
	PUSH	OFFSET	main@sb
	PUSH	OFFSET	main@sa
	CALL	userstringcompare
	PUSH	EAX
	CALL	printnum
	PUSH	0
	CALL	ExitProcess
main ENDP

;----------FUNCTION fii---------

fii PROC uses EBX ECX EDX, fii@x_PARAM : SDWORD, fii@y_PARAM : SDWORD
	MOV 	EAX, fii@x_PARAM
	MOV 	fii@x, EAX
	MOV 	EAX, fii@y_PARAM
	MOV 	fii@y, EAX
;-------------iiivv-------------
	PUSH	fii@x
	PUSH	fii@x
	PUSH	fii@y
	POP 	EDX
	POP 	EAX
	ADD 	EAX, EDX
	PUSH	EAX
	POP 	EDX
	POP 	EBX
	IMUL	EBX, EDX
	PUSH	EBX
	POP 	EAX
	MOV 	fii@z, EAX
;-------------i-------------
	PUSH	fii@z
	POP 	EAX
	RET
fii ENDP


;----------FUNCTION fss---------

fss PROC uses EBX ECX EDX, fss@a_OFFSET : DWORD, fss@b_OFFSET : DWORD
	PUSH	fss@a_OFFSET
	PUSH	OFFSET fss@a
	CALL	stringcopy
	PUSH	fss@b_OFFSET
	PUSH	OFFSET fss@b
	CALL	stringcopy
;-------------iiu-------------
	PUSH	OFFSET	fss@a
	PUSH	OFFSET	fss@b
	POP 	EBX
	POP 	EAX
	PUSH	EBX
	PUSH	EAX
									CALL	stringcompare
	MOV 	EBX, 0
	.IF 	(EAX < EBX)
;-------------ilviv-------------
	PUSH	OFFSET	fss@a
	PUSH	OFFSET	Ls000
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@b
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@c
	CALL	stringcopy
;-------------ii@i-------------
	PUSH	OFFSET	fss@b
	PUSH	OFFSET	fss@a
	CALL	indexof
	PUSH	EAX
	CALL	printnum
	.ELSE
;-------------iiu-------------
	PUSH	OFFSET	fss@a
	PUSH	OFFSET	fss@b
	POP 	EBX
	POP 	EAX
	PUSH	EBX
	PUSH	EAX
									CALL	stringcompare
	MOV 	EBX, 0
	.IF 	(EAX == EBX)
;-------------ilviv-------------
	PUSH	OFFSET	fss@a
	PUSH	OFFSET	Ls001
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@b
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@c
	CALL	stringcopy
;-------------i@i-------------
	PUSH	OFFSET	fss@a
	CALL	stringlength
	PUSH	EAX
	CALL	printnum
	JMP 	endIfMarker@88
	.ENDIF
;-------------ilviv-------------
	PUSH	OFFSET	fss@a
	PUSH	OFFSET	Ls002
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@b
	POP 	ECX
	POP 	EBX
	INVOKE	strconcat, EBX, ECX
	PUSH	EAX
	PUSH	OFFSET	fss@c
	CALL	stringcopy
;-------------i@i-------------
	PUSH	OFFSET	fss@a
	CALL	tonumber
	PUSH	EAX
	CALL	printnum
endIfMarker@88:
	.ENDIF
;-------------i-------------
	PUSH	OFFSET	fss@c
	POP 	EAX
	RET
fss ENDP


END main