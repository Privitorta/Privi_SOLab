segment .text
global main

main:   mov dx, 420     ; dx = 420
        mov bx, 240     ; bx = 240
max:    cmp dx, bx      ;ZF = (dx==bx);CF = (dx < bx)
        je fine         ; if(ZF) goto fine
        jg diff         ; if!(ZF || CF) goto diff
        mov ax, dx      ; ax = dx
        mov dx, bx      ; dx = bx
        mov bx, ax      ; bx = ax
diff:   sub dx, bx      ; dx -= bx
        jmp max         ; goto max
fine:   hlt             ; halt/*dx == mcd(420,240)*/

times 510-($-$$)db 0
dw 0xAA55