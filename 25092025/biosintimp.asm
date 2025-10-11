segment .text
global main

main:
; set it only once.
MOV    AH, 0Eh    ; select sub-function.
MOV    AL, 'H'    ; ASCII code: 72
INT    10h        ; print it!
MOV    AL, 'e'    ; ASCII code: 101
INT    10h        ; print it!
MOV    AL, 'l'    ; ASCII code: 108
INT    10h        ; print it!
MOV    AL, 'l'    ; ASCII code: 108
INT    10h        ; print it!
MOV    AL, 'o'    ; ASCII code: 111
INT    10h        ; print it!
MOV    AL, '!'    ; ASCII code: 33
INT    10h        ; print it!
fine:   hlt       ; halt

times 510-($-$$)db 0
dw 0xAA55