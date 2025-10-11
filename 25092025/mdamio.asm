segment .text
global main

QUANTI equ 100                  ; #define QUANTI 100
N equ QUANTI*2-2                ; #define N (QUANTI*2-2)
main:   mov ax, 0xb800          ; /* mov ds, x è vietato */
        mov ds, ax              ; ds = ax
        mov cx, QUANTI          ; /* cx è indice per loop */
        mov bx, N               ; bx = N
ciclo:                          ; do {   
        mov byte[ds:bx], 'm'    ; mem[ds:bx]='m'
        sub bx, 2               ; bx -= 2
        loop ciclo              ; } while(cx!=0)
fine:   hlt                     ; halt/*dx == mcd(420,240)*/

times 510-($-$$)db 0
dw 0xAA55